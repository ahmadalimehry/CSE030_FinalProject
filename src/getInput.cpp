#include "heuristic.cpp"
#include <Graph.h>

Heuristic getHeuristic() { return Heuristic::COST; }
Vertex *getStart(Graph *g);
Vertex *getStop(Graph *g);
