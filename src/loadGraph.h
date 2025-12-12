#ifndef LOAD_GRAPH_CPP
#define LOAD_GRAPH_CPP 1
#include "ArrayList.h"
#include "hashmap.h"
#include <Graph.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
uint readCSVLine(std::string line, ArrayList<std::string> &elements);
void importDefinitions(std::string path, Hashmap<std::string> &map);
enum TokenType { LITERAL_STRING, IDENTIFIER, EMPTY, END_OF_FILE };
std::ostream &operator<<(std::ostream &os, const TokenType &token) {
    switch (token) {
    case LITERAL_STRING:
        os << "LITERAL_STRING";
        break;
    case IDENTIFIER:
        os << "IDENTIFIER";
        break;
    case EMPTY:
        os << "EMPTY";
        break;
    case END_OF_FILE:
        os << "END_OF_FILE";
        break;
    }
    return os;
}

typedef struct Token {
    TokenType type;
    std::string value;
} Token_t;
std::ostream &operator<<(std::ostream &os, const Token_t &token) {
    os << "{" << token.type << ", " << token.value << "}";
    return os;
}

class Tokenizer {
    std::string s;
    uint i;

public:
    Tokenizer(std::string input) {
        i = 0;
        s = input;
    }
    bool hasNextToken() { return i < s.length(); }
    struct Token getNextToken() {
        if (i >= s.length())
            return {TokenType::END_OF_FILE, ""};
        char lookahead = s[i];
        if (lookahead == ' ') {
            readEmptyToken();
            return getNextToken();
        } else if (lookahead == '"') {
            return readStringLiteral();
        } else {
            return readIdentifierToken();
        }
    }
    struct Token readEmptyToken() {
        uint start = i;
        while (s[i] == ' ') {
            i++;
        }
        return {TokenType::EMPTY, s.substr(start, i - start)};
    }
    struct Token readStringLiteral() {
        if (!(s[i] == '"' || s[i] == '\'')) {
            throw std::logic_error("Can only read string literal when starting "
                                   "from double quotes, not " +
                                   std::string(&s[i]));
        }
        char start = s[i];
        i++;
        uint start_i = i;

        while (s[i] != start) {
            i++;
        }
        i++;
        return {TokenType::LITERAL_STRING, s.substr(start_i, i - start_i - 1)};
    }
    struct Token readIdentifierToken() {
        uint start_i = i;
        while (s[i] != ' ') {
            i++;
        }
        return {TokenType::IDENTIFIER, s.substr(start_i, i - start_i)};
    }
};

Graph *loadGraph(std::string path) {
    ArrayList<std::string> csv_lines;
    std::string text;
    std::ifstream file(path);
    std::string using_defs_path("");

    while (getline(file, text)) {
        if (text.empty()) {
            continue;
        } else if (text.rfind("//", 0) == 0) { // is a comment
            continue;
        } else if (text.rfind("#use", 0) == 0) { // use preprocessing step
            uint i = 4;
            while (i < text.length()) {
                char c = text[i];
                if (c == ' ') {
                    i++;
                } else {
                    break;
                }
            }
            if (i == text.length()) {
                throw std::logic_error("use preprocessor requires a file path");
            } else if (text[i] != '"') {
                throw std::logic_error(
                    "use path must be surrounded by double quotes '\"'");
            }
            i++;
            while (i < text.length()) {
                if (text[i] != '"') {
                    using_defs_path += text[i];
                    i++;
                } else {
                    break;
                }
            }
            std::cout << "Using Definitions from: " << using_defs_path
                      << std::endl;

        } else {
            csv_lines.append(text);
        }
    }
    // Make a 1-D array of each element in the csv
    ArrayList<std::string> elements;
    for (int i = 0; i < csv_lines.size(); i++) {
        auto line = csv_lines[i];
        uint num = readCSVLine(line, elements);
        if (num != 5) {
            throw std::logic_error(
                std::string("CSV Line: ") + std::to_string(i) +
                std::string("Expected 5 Elements per line, got ") +
                std::to_string(num));
        }
    }

    // Build Graph

    // Add Vertices
    // Get A hashtable that stores aliases to vertex names
    Hashmap<std::string> aliases_to_names;
    if (using_defs_path.length() != 0) {
        importDefinitions(using_defs_path, aliases_to_names);
    } else {
        for (int i = 0; i < elements.size(); i += 5) {
            std::string from = elements[i];
            std::string to = elements[i + 1];
            aliases_to_names.set(from, from);
            aliases_to_names.set(to, to);
        }
    }
    Hashmap<int> names;
    for (auto pair : aliases_to_names.items()) {
        names.set(pair.second, 1);
    }
    Hashmap<Vertex *> name_to_vert;

    Graph *g = new Graph();
    for (std::string name : names.keys()) {
        Vertex *v = new Vertex(std::string(name));
        g->addVertex(v);
        name_to_vert.set(name, v);
    }

    for (int i = 0; i < elements.size(); i += 5) {
        auto s_from = elements[i]; // these are all strings
        auto s_to = elements[i + 1];
        auto s_hours = elements[i + 2];
        auto s_dollars = elements[i + 3];
        auto s_bidirectional = elements[i + 4];
        float hours = std::stof(s_hours);
        int dollars = std::stoi(s_dollars);
        std::cout << "TODO: Change Edges to store all metadata\n";
        bool bidirectional = s_bidirectional.rfind("0") == 0;
        Vertex *from = name_to_vert.get(aliases_to_names.get(s_from));
        Vertex *to = name_to_vert.get(aliases_to_names.get(s_to));
        if (bidirectional) {
            g->addEdge(from, to, dollars, (int)hours);
        } else {
            g->addDirectedEdge(from, to, dollars, (int)hours);
        }
    }

    std::cout << "Loaded Flight Data\n";

    return g;
}

void importDefinitions(std::string path, Hashmap<std::string> &map) {
    std::string fqn = std::string("flights_data/") + path;

    std::ifstream file(fqn);
    std::string line;
    std::cout << "importing from file " << fqn << " \n";

    while (getline(file, line)) {
        if (line.empty())
            continue;
        if (line.rfind("//", 0) == 0) // is a comment
            continue;
        Tokenizer t(line);
        ArrayList<Token_t> tokens;

        while (t.hasNextToken()) {
            tokens.append(t.getNextToken());
        }
        if (tokens.size() == 1) {
            Token_t tok = tokens[0];
            if (tok.type != TokenType::LITERAL_STRING) {
                throw std::logic_error("Non-aliasing declarations must be "
                                       "string literals! (Enclosed in \"\" )");
            }
            if (map.contains(tok.value)) {
                throw std::logic_error("Double Declaration of '" + tok.value +
                                       "'");
            }
            map.set(tok.value, tok.value);
        } else if (tokens.size() == 3) {
            Token_t ident = tokens[0];
            Token_t eq = tokens[1];
            Token_t str_lit = tokens[2];
            if (ident.type != TokenType::IDENTIFIER) {
                throw std::logic_error(
                    "Expected Identifier as first token in line: " + line);
            }
            if (eq.value != "=") {
                throw std::logic_error("Only Assignment Operator Allowed in "
                                       "declarative files, not:\'" +
                                       eq.value + "\' | Line: " + line);
            }
            std::string value;
            if (str_lit.type == TokenType::LITERAL_STRING) {
                value = str_lit.value;
            } else if (str_lit.type == TokenType::IDENTIFIER) {
                if (!map.contains(str_lit.value)) {
                    throw std::logic_error("Unknown Variable: " +
                                           str_lit.value);
                }
                value = map.get(str_lit.value);
            }
            if (map.contains(ident.value)) {
                throw std::logic_error("Double Declaration of '" + ident.value +
                                       "'");
            }
            map.set(ident.value, value);
            if (!map.contains(value)) {
                map.set(value, value);
            }
        } else {
            throw std::logic_error("Malformed Line: " + line);
        }

        // auto eq_i = line.find("=");
        // if (eq_i == std::string::npos) {

        //     std::cout << "line has no =\n";
        // } else {

        //     std::cout << "line has =\n";
        // }
    }
}
uint scanCSVElement(std::string line, uint start) {
    uint i = start;
    if (i >= line.length())
        return 0;
    if (line[i] == '"') {
        i++;
        while (i < line.length()) {
            if (line[i] == '"') {
                i++;
                break;
            }
            i++;
        }

    } else {
        while (i < line.length()) {
            if (line[i] == ',')
                break;
            i++;
        }
    }
    return i;
}
uint readCSVLine(std::string line, ArrayList<std::string> &elements) {
    uint i = 0;
    uint cols = 0;
    while (i < line.length()) {
        uint element_end_i = scanCSVElement(line, i);
        if (line[i] == '"')
            elements.append(line.substr(i + 1, element_end_i - i - 2));
        else
            elements.append(line.substr(i, element_end_i - i));
        i = element_end_i + 1; // +1 for the ','
        cols++;
    }
    return cols;
}

uint getCSVColumns(std::string line) {
    uint i = 0;
    uint cols = 0;
    while (i < line.length()) {
        i = scanCSVElement(line, i);
        i++;
        cols++;
    }
    return cols;
}
#endif