#ifndef LOAD_GRAPH_CPP
#define LOAD_GRAPH_CPP 1
#include "ArrayList.h"
#include "HashTable.h"
#include "Tokenizer.h"
#include <Graph.h>
#include <Hashmap.h>
#include <fstream>
#include <iostream>
#include <ostream>

#include <string>

enum TokenType {
    LITERAL_STRING,
    LITERAL_NUMBER,
    IDENTIFIER,
    COMMA,
    EMPTY,
    END_OF_FILE
};
std::string toString(const TokenType &obj);

Graph *loadGraph(std::string path);

void importDefinitions(std::string path, Hashmap<std::string> &map);

uint scanCSVElement(std::string line, uint start);

uint readCSVLine(std::string line, ArrayList<std::string> &elements);

#endif