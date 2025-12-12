#include <display.h>
using namespace std;

void clearScreen() { cout << "\033[2J\033[1;1H"; }

int computeCost(Waypoint *end) {
    int cost = 0;
    Waypoint *temp = end;

    while (temp != nullptr) {
        cost += temp->weight * 50; // price per hour
        temp = temp->parent;
    }

    return cost;
}

void displayBanner() {
    cout << endl;
    cout << "==========================================" << endl;
    cout << "            FLIGHT ROUTE PLANNER" << endl;
    cout << "==========================================" << endl;
    cout << endl;
}

void showMenu() {

    cout << "Main Menu" << endl;
    cout << "------------------------------------------" << endl;
    cout << "  1) List all airports" << endl;
    cout << "  2) Plan a flight" << endl;
    cout << "  3) Change flight data source " << endl;
    cout << "  4) Exit" << endl;
    cout << "------------------------------------------" << endl;
}

int getMenuChoice() {
    int choice;
    cout << "Enter Choice: ";
    while (!(cin >> choice) || choice < 1 || choice > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter 1-4:";
    }
    return choice;
}

void displayAirports(Graph *g) {
    cout << "\n Available Airports:" << endl;
    cout << "------------------------------------------" << endl;

    for (int i = 0; i < g->vertices.size(); i++) {
        cout << "  " << i << ") " << g->vertices[i]->data << endl;
    }

    cout << endl;
}

int chooseAirports(Graph *g, string prompt) {
    displayAirports(g);

    int index;
    cout << prompt;

    while (!(cin >> index) || index < 0 || index >= g->vertices.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid index. Try Again: ";
    }
    return index;
}

int choosePreferences() {
    cout << "\nChoose search preference: " << endl;
    cout << "------------------------------------------" << endl;
    cout << "  1) Cheapest price" << endl;
    cout << "  2) Fastest travel time" << endl;
    cout << "  3) Least number of stops" << endl;
    cout << "------------------------------------------" << endl;

    int p;
    cout << "Enter Choice: ";

    while (!(cin >> p) || p < 1 || p > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter 1-3";
    }

    return p;
}

void displayRoute(Waypoint *end) {

    cout << endl;
    cout << "================= ROUTE SUMMARY =================" << endl;
    cout << endl;

    if (end == nullptr) {
        cout << "No route found." << endl;
        cout << "==================================================" << endl;
        return;
    }

    ArrayList<Waypoint *> nodes;
    Waypoint *temp = end;

    while (temp != nullptr) {
        nodes.append(temp);
        temp = temp->parent;
    }

    int totalTime = end->partialCost;
    int stops = nodes.size() - 1;

    for (int i = nodes.size() - 1; i >= 0; i--) {
        cout << nodes[i]->vertex->data;

        if (i != 0) {
            cout << "  -->  " << nodes[i - 1]->vertex->data << "  ("
                 << nodes[i - 1]->weight << " hours)";
        }

        cout << endl;
    }

    cout << endl;
    cout << "Total Travel Time: " << totalTime << " hours" << endl;

    int totalCost = computeCost(end);
    cout << "Total Cost:        $" << totalCost << endl;
    cout << "Number of stops:   " << stops << endl;

    cout << "==================================================" << endl;
}

void waitForEnter() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
