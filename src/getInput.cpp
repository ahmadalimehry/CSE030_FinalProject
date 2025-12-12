#include "getInput.h"
#include "display.h"
#include <exc.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
using namespace std;

Heuristic getHeuristic() {
    cout << "\nChoose a search preference:\n";
    cout << "0. Back to main menu\n";
    cout << "1. Cheapest price\n";
    cout << "2. Shortest travel time\n";
    cout << "3. Least stops\n";
    cout << "Enter choice (0-3): ";

    int choice;
    cin >> choice;

    if (choice == 0) {
        throw GoToMainMenu();
    }

    switch (choice) {
    case 1:
        return COST;
    case 2:
        return TIME;
    case 3:
        return STOPS;
    default:
        cout << "Invalid input. Defaulting to least stops.\n";
        return STOPS;
    }
}

Vertex *getStart(Graph *g) {
    Vertex *start;
    bool found = false;
    int graphSize = g->vertices.size();

    while (found == false) {
        int userStart;
        std::cout << "---------------------------------------------"
                  << std::endl;
        for (int i = 0; i < graphSize; i++) {
            std::cout << "(" << i << ") " << g->vertices[i]->data << std::endl;
        }
        std::cout << "(" << graphSize << ") Exit" << std::endl;
        std::cout << "---------------------------------------------"
                  << std::endl;
        std::cout << "Enter the number of your starting city: ";
        std::cin >> userStart;

        for (int i = 0; i < graphSize; i++) {
            if (userStart == i) {
                found = true;
                start = g->vertices[i];
                break;
            } else if (userStart == graphSize) {
                throw GoToMainMenu();
                found = true;
            }
        }
    }

    std::cout << "Your starting point is: " << start->data << "." << std::endl;
    return start;
}

Vertex *getStop(Graph *g) {
    Vertex *stop;
    bool found = false;
    int graphSize = g->vertices.size();

    while (found == false) {
        int userDest;
        std::cout << "---------------------------------------------"
                  << std::endl;
        for (int i = 0; i < graphSize; i++) {
            std::cout << "(" << i << ") " << g->vertices[i]->data << endl;
        }
        std::cout << "(" << graphSize << ") Exit" << std::endl;
        std::cout << "---------------------------------------------"
                  << std::endl;
        std::cout << "Enter the number of your destination city: ";
        std::cin >> userDest;

        for (int i = 0; i < graphSize; i++) {
            if (userDest == i) {
                found = true;
                stop = g->vertices[i];
                break;
            } else if (userDest == graphSize) {
                throw GoToMainMenu();
                found = true;
            }
        }
    }

    std::cout << "Your destination is: " << stop->data << "." << std::endl;
    return stop;
}

string getNewFlightDataPath(string cur_path) {
    clearScreen();
    cout << "---------------------------------------------\n";
    cout << "Current Path: " << cur_path << "\n";
    cout << "---------------------------------------------\n";
    cout << "Enter [x] to Cancel.\n";
    cout << "Enter Path: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string new_path;
    cin >> new_path;

    while (1) {
        if (new_path == "x") {
            throw GoToMainMenu();
        } else if (!std::ifstream(new_path)) {
            cout << "File not found.\n";
        } else {
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cin.clear();
        cout << "Enter Path: ";
        cin >> new_path;
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return new_path;
}