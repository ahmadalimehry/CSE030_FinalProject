#include "heuristic.h"
#include <Graph.h>

Heuristic getHeuristic();
Vertex *getStart(Graph *g);
Vertex *getStop(Graph *g);
std::string getNewFlightDataPath(std::string cur_path);