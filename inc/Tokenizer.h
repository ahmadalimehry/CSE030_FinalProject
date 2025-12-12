#ifndef TOKENIZER_H
#define TOKENIZER_H 1
#define VERBOSE 0
#include <ArrayList.h>
#include <Hashmap.h>
#include <Token.h>
#include <iostream>
#include <ostream>

// We do a subset of regex matching for tokens \(^-^)/
// also, quantifiers are prefixed not postfixed

template <typename T> class Tokenizer {
    Hashmap<T> grammar;
    T null_type;
    T eof_type;

public:
    Tokenizer(Hashmap<T> grammar, T null_type, T eof_type) {
        this->grammar = grammar;
        this->null_type = null_type;
        this->eof_type = eof_type;
    }

    ArrayList<struct Token<T>> tokenize(std::string str) {
        uint i = 0;
        ArrayList<struct Token<T>> out;
        while (i < str.length()) {
            bool found = false;
            struct Token<T> tok;
            for (auto p : grammar.items()) {
                int len = match(p.first, &str[i]);
                if (len > 0) {
                    tok.type = p.second;
                    tok.value = str.substr(i, len);
                    if (tok.type != null_type)
                        out.append(tok);
                    i += len;
                    found = true;
                    break;
                }
            }
            if (found) {
                if (tok.type == eof_type)
                    break;
            } else {
                if (VERBOSE)
                    std::cout << "Ended Before!\n";
                break;
            }
        }
        return out;
    };
    int match(char *re, char *str) {
        char *start = str;
        if (VERBOSE)
            std::cout << "Matching regex: " << re << ", str: " << str << "\n";
        match_state m = _match({re, str, 0});
        return (int)(m.str - start);
    }
    struct match_state {
        char *re;
        char *str;
        char failed;
        char cont = 1;
    };
    match_state _match(match_state state) {
        state.failed = 0;
        if (!*state.str) {
            state.failed = 1;
            return state;
        }
        if (*state.re == '[') {
            state = _match_charset({state.re, state.str, 0, state.cont});
            if (*state.re && state.cont && !state.failed) {
                state = _match(state);
            }
        } else if (*state.re == ']') {
            throw std::logic_error("");
        } else if (*state.re == '+') {
            state.re++;
            match_state m = _match({state.re, state.str, 0});
            if (m.failed) {
                state.re = m.re;
                state.failed = m.failed;
            } else {
                while (*m.str && !m.failed) {
                    m = _match({state.re, m.str, 0, 0});
                }
                state.str = m.str;
            }
            state.re = m.re;
            if (VERBOSE)
                std::cout << "Ended + Qual: re=" << *state.re
                          << ", str=" << *state.str
                          << ", failed=" << bool(state.failed) << "\n";
            if (*state.re && state.cont && !state.failed) {
                state = _match(state);
            }
        } else if (*state.re == '*') {
            state.re++;
            match_state m = {state.re, state.str, 0};
            while (*m.str && !m.failed) {
                m = _match({state.re, m.str, 0, 0});
            }
            state.str = m.str;
            state.re = m.re;
            if (VERBOSE)
                std::cout << "Ended * Qual: re=" << *state.re
                          << ", str=" << *state.str
                          << ", failed=" << bool(state.failed) << "\n";
            if (*state.re && state.cont && !state.failed) {
                state = _match(state);
            }

        } else if (*state.re == '.') {
            state.str++;
            state.re++;
            if (*state.re && state.cont) {
                state = _match(state);
            }
        } else {
            if (*state.str != *state.re) {
                if (VERBOSE)
                    std::cout << "Match failed because \"" << *state.re
                              << "\"and \"" << *state.str << "\" do not match"
                              << "\n";
                state.failed = 1;
                return state;
            }
            if (VERBOSE)
                std::cout << "Caught " << (char)*state.str << "\n";
            state.str++;
            state.re++;
            if (*state.re && state.cont) {
                if (VERBOSE)
                    std::cout << "Continuing\n";
                state = _match(state);
            }
        }

        return state;
    }
    match_state _match_charset(match_state state) {
        if (*state.re != '[') {
            throw std::logic_error("");
        }
        state.re++;
        char inverted = *(state.re) == '^';
        state.re += inverted;
        char *next_valid_range = state.re + 1;
        bool failed;
        if (inverted) {
            failed = false;
            while (*state.re != ']') {
                if (failed) {
                } else if (*state.re == '-' && state.re >= next_valid_range) {
                    // build range
                    char lower = *(state.re - 1);
                    state.re++;
                    char upper = *(state.re);
                    if (upper == '\\') {
                        state.re++;
                        upper = *(state.re);
                    }
                    if (lower > upper) {
                        throw std::logic_error("");
                    }
                    next_valid_range = state.re + 2;
                    if (lower <= *state.str && *state.str <= upper) {
                        // if string in range then we fail
                        failed = true;
                    }
                } else if (*state.re == '\\') {
                    // parse the next character literally
                    state.re++;
                    if (*state.re == *state.str) {
                        failed = true;
                    }
                } else {
                    // parse this character literally
                    if (*state.re == *state.str) {
                        failed = true;
                        continue;
                    }
                }
                state.re++;
            }

        } else {
            failed = true;
            while (*state.re != ']') {
                if (!failed) {
                } else if (*state.re == '-' && state.re >= next_valid_range) {
                    // build range
                    char lower = *(state.re - 1);
                    state.re++;
                    char upper = *(state.re);
                    if (upper == '\\') {
                        state.re++;
                        upper = *(state.re);
                    }
                    if (lower > upper) {
                        throw std::logic_error("invalid charset range order");
                    }
                    if (VERBOSE)
                        std::cout << "Checking Range: [" << lower << ", "
                                  << upper << "]\n";
                    next_valid_range = state.re + 2;
                    if (lower <= *state.str && *state.str <= upper) {
                        // if string not in range then we fail
                        failed = false;
                    }
                } else if (*state.re == '\\') {
                    // parse the next character literally
                    state.re++;
                    if (*state.re == *state.str) {
                        failed = false;
                    }
                } else {
                    // parse this character literally
                    if (*state.re == *state.str) {
                        failed = false;
                        continue;
                    }
                }
                state.re++;
            }
        }
        state.re++;
        state.failed = failed;
        if (VERBOSE)
            std::cout << "Failed:" << bool(failed) << "\n";
        if (!state.failed) {
            state.str++;
        }
        return state;
    }
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const struct Token<T> &token) {
    os << "{" << token.type << ", " << token.value << "}";
    return os;
}
#endif