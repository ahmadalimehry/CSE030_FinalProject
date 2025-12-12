#include "loadGraph.h"
#include "Token.h"
#include <sstream>
#include <string>

std::string toString(const TokenType &obj) {
    std::ostringstream temp;
    temp << obj;
    return temp.str();
}

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
    case LITERAL_NUMBER:
        os << "LITERAL_NUMBER";
        break;
    case COMMA:
        os << "COMMA";
        break;
    }
    return os;
}

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
    // Get A Hashmap that stores aliases to vertex names
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

    std::cout << aliases_to_names << "\n";

    for (int i = 0; i < elements.size(); i += 5) {
        std::string s_from = elements[i]; // these are all strings
        std::string s_to = elements[i + 1];
        std::string s_hours = elements[i + 2];
        std::string s_dollars = elements[i + 3];
        std::string s_bidirectional = elements[i + 4];
        float hours = std::stof(s_hours);
        int dollars = std::stoi(s_dollars);
        bool bidirectional = s_bidirectional.rfind("0") == 0;
        if (!aliases_to_names.contains(s_from)) {
            throw std::logic_error("Unknown Vertex: " + s_from);
        }
        if (!aliases_to_names.contains(s_to)) {
            throw std::logic_error("Unknown Vertex: " + s_to);
        }
        s_from = aliases_to_names.get(s_from);
        s_to = aliases_to_names.get(s_to);
        if (!name_to_vert.contains(s_from)) {
            Vertex *v = new Vertex(std::string(s_from));
            g->addVertex(v);
            name_to_vert.set(s_from, v);
        }
        if (!name_to_vert.contains(s_to)) {
            Vertex *v = new Vertex(std::string(s_to));
            g->addVertex(v);
            name_to_vert.set(s_to, v);
        }
        Vertex *from = name_to_vert.get(s_from);
        Vertex *to = name_to_vert.get(s_to);
        if (bidirectional) {
            g->addEdge(from, to, dollars, (int)hours);
        } else {
            g->addDirectedEdge(from, to, dollars, (int)hours);
        }
    }

    std::cout << "Loaded Flight Data\n";

    return g;
}
bool isInt(std::string str) {
    for (uint i = 0; i < str.size(); i++) {
        if (!('0' <= str[i] && str[i] <= '9')) {
            return false;
        }
    }
    return true;
}

void importDefinitions(std::string path, Hashmap<std::string> &map) {
    std::string fqn = std::string("flights_data/") + path;

    std::ifstream file(fqn);
    std::string line;
    HashTable<std::string> nameset;
    Hashmap<TokenType> grammar({
        {"[a-zA-Z]*[a-zA-Z0-9]", IDENTIFIER},
        {"+[0-9]", LITERAL_NUMBER},
        {",", COMMA},
        {"=", IDENTIFIER},
        {"\"*[^\\\"]\"", LITERAL_STRING},
        {"+ ", EMPTY},
    });
    Tokenizer<TokenType> tokenizer(grammar, EMPTY, END_OF_FILE);
    if (!file) {
        throw std::logic_error("File Not Found: flights_data/" + path);
    }
    while (getline(file, line)) {
        if (line.empty())
            continue;
        if (line.rfind("//", 0) == 0) // is a comment
            continue;
        ArrayList<struct Token<TokenType>> tokens = tokenizer.tokenize(line);
        if (tokens.size() == 1) {
            auto tok = tokens[0];
            std::string value = tok.value.substr(1, tok.value.size() - 2);

            if (tok.type != TokenType::LITERAL_STRING) {
                throw std::logic_error("Non-aliasing declarations must be "
                                       "string literals! (Enclosed in \"\" )");
            }
            if (isInt(value)) {
                throw std::logic_error("Location cannot be an integer!");
            }
            if (map.contains(value)) {
                throw std::logic_error("Double Declaration of '" + value + "'");
            }
            map.set(value, value);
            map.set(std::to_string(nameset.getSize()), value);
            nameset.insert(value);
        } else if (tokens.size() == 3) {
            auto ident = tokens[0];
            auto eq = tokens[1];
            auto str_lit = tokens[2];
            if (ident.type != TokenType::IDENTIFIER) {
                throw std::logic_error(
                    "Expected " + toString(TokenType::IDENTIFIER) + " (got " +
                    toString(ident.type) + ") as first token in line: " + line);
            }
            if (eq.value != "=") {
                throw std::logic_error("Only Assignment Operator Allowed in "
                                       "declarative files, not:\'" +
                                       eq.value + "\' | Line: " + line);
            }
            std::string value;
            if (str_lit.type == TokenType::LITERAL_STRING) {
                value = str_lit.value.substr(1, str_lit.value.size() - 2);
            } else if (str_lit.type == TokenType::IDENTIFIER) {
                if (!map.contains(str_lit.value)) {
                    throw std::logic_error("Unknown Variable: " +
                                           str_lit.value);
                }
                value = map.get(str_lit.value);
            } else {
                throw std::logic_error(
                    "Expected " + toString(TokenType::IDENTIFIER) + " or " +
                    toString(TokenType::LITERAL_STRING) + " (got " +
                    toString(ident.type) + ") as third token in line: " + line);
            }
            if (map.contains(ident.value)) {
                throw std::logic_error("Double Declaration of '" + ident.value +
                                       "'");
            }
            if (isInt(value)) {
                throw std::logic_error("Location cannot be an integer!");
            }

            map.set(ident.value, value);
            if (!map.contains(value)) {
                map.set(value, value);
                map.set(std::to_string(nameset.getSize()), value);
                nameset.insert(value);
            }
        } else {
            throw std::logic_error("Malformed Line: " + line);
        }
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
