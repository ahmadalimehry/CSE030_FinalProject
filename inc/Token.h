#ifndef TOKEN_H
#define TOKEN_H
#include <string>

template <typename T> struct Token {
    T type;
    std::string value;
};

#endif
