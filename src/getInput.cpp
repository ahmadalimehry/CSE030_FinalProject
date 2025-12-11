#include "getInput.h"
#include <iostream>
using namespace std;

Heuristic getHeuristic() {
    cout << "\nChoose a search preference:\n";
    cout << "1. Cheapest price\n";
    cout << "2. Shortest travel time\n";
    cout << "3. Least stops\n";
    cout << "Enter choice (1-3): ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: return COST;
        case 2: return TIME;
        case 3: return STOPS;
        default:
            cout << "Invalid input. Defaulting to least stops.\n";
            return STOPS;
    }
}




Vertex *getStart(Graph *g) {
    Vertex *start;
    bool found = false;

    while(found == false){
        std::string userStart = "";
        std::cout << "Please enter your starting airport: ";
        std::cin >> userStart;

        for(int i = 0; i < g->vertices.size(); i++){
            if(g->vertices[i]->data == userStart){
                found = true;
                start = g->vertices[i];
            }
        }
    }

    return start;
}

Vertex *getStop(Graph *g){
    Vertex* stop;
    bool found = false;

    while(found == false){
        std::string userDest = "";
        std::cout << "Please enter your destination: ";
        std::cin >> userDest;

        for(int i = 0; i < g->vertices.size(); i++){
            if(g->vertices[i]->data == userDest){
                found = true;
                stop = g->vertices[i];
            }
        }
    }

    return stop;
}
