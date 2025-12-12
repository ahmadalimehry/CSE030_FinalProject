#include "CachedString.h"
#include "Tokenizer.h"
#include "display.h"
#include "getInput.h"
#include "heuristic.h"
#include "loadGraph.h"
#include <Graph.h>
#include <exc.h>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

using namespace std;
int main() {
    CachedString path("cache", "flights_data/one.txt");
    Graph *g = loadGraph(path.get());
    clearScreen();
    displayBanner();

    bool running = true;
    while (running) {
        clearScreen();
        showMenu();
        int choice = getMenuChoice();
        try {

            if (choice == 1) {
                clearScreen();
                displayAirports(g);
                waitForEnter();
            } else if (choice == 2) {
                Vertex *start = getStart(g);
                Vertex *stop = getStop(g);
                Heuristic pref = getHeuristic();
                g->modifyWeights(pref);
                Waypoint *result = g->ucs(start, stop);
                clearScreen();
                displayRoute(result);
                waitForEnter();
            } else if (choice == 3) {
                path.set(getNewFlightDataPath(path.get()));
                g = loadGraph(path.get());
            } else {
                running = false;
            }
        } catch (const GoToMainMenu &ignored) {
            // we catch this to get back to menu
        }
    }
}
