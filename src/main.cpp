#include "display.h"
#include "getInput.h"
#include "heuristic.h"
#include "loadGraph.h"
#include "modifyGraph.h"
#include <FL/fl_draw.H>
#include <Graph.h>
#include <iostream>

using namespace std;

int main() {

    // Application app;
    // return app.run();

    string path = "flights_data/one.txt";

    Graph *g = loadGraph(path);

    clearScreen();
    displayBanner();

    bool running = true;
    while (running) {
        clearScreen();
        showMenu();
        int choice = getMenuChoice();

        if (choice == 1) {
            clearScreen();
            displayAirports(g);
            waitForEnter();
        } else if (choice == 2) {
            Vertex *start = getStart(g);
            cout << "Got Start\n";
            Vertex *stop = getStop(g);
            // clearScreen();
            // int startIdx = chooseAirports(g, "Enter starting airport index:
            // "); clearScreen(); int destIdx =
            //     chooseAirports(g, "Enter destination airport index: ");
            // clearScreen();
            Heuristic pref = getHeuristic();
            g->modifyWeights(pref);
            Waypoint *result = g->ucs(start, stop);
            clearScreen();
            displayRoute(result);
            waitForEnter();
        } else {
            running = false;
        }
    }
}
