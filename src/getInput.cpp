#include "heuristic.cpp"
#include <Graph.h>

Heuristic getHeuristic() { return Heuristic::COST; }

Vertex *getStart(Graph *g) {
    Vertex *start;
    bool found;

    std::string userStart = "";
    std::cin >> userStart;

    for(int i = 0; i < g->vertices.size(); i++){
        if(g->vertices[i]->data == userStart){
            found = true;
            start = g->vertices[i];
        }
    }

    if(found){
        return start;
        std::cout << "Departing airport found" << std::endl;
    } else {
        return nullptr;
        std::cout << "Departing airport not found" << std::endl;
    }

    return nullptr;
}

Vertex *getStop(Graph *g);
