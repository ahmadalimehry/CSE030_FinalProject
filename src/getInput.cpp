#include "getInput.h"
#include <iostream>
#include <stdexcept>
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
        int userStart ;
        std::cout << "---------------------------------------------" << std::endl;
        for(int i = 0; i < g->vertices.size(); i++){
            std::cout << "(" << i << ") " << g->vertices[i]->data << std::endl;
        }
        std::cout << "(" << g->vertices.size() << ") Exit" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Enter the number of your starting city: ";
        std::cin >> userStart;

        for(int i = 0; i < g->vertices.size(); i++){
            if(userStart == i){
                found = true;
                start = g->vertices[i];
                break;
            } else if(userStart == g->vertices.size()) {
                throw std::logic_error("Going back to main menu");
                found = true;
            }
        }
    }

    std::cout << "Your starting point is: " << start->data << "." << std::endl;
    return start;
}

Vertex *getStop(Graph *g){
    Vertex* stop;
    bool found = false;

    while(found == false){
        int userDest;
        std::cout << "---------------------------------------------" << std::endl;
        for(int i = 0; i < g->vertices.size(); i++){
            std::cout << "(" << i << ") " << g->vertices[i]->data << endl; 
        }
        std::cout << "(" << g->vertices.size() << ") Exit" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Enter the number of your destination city: ";
        std::cin >> userDest;

        for(int i = 0; i < g->vertices.size(); i++){
            if(userDest == i){
                found = true;
                stop = g->vertices[i];
                break;
            } else if(userDest == g->vertices.size()){
                throw std::logic_error("Going back to main menu.");
                found = true;
            }
        }
    }

    std::cout << "Your destination is: " << stop->data << "." << std::endl;
    return stop;
}
