#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>
#include <bobcat_ui/window.h>
#include <string>

using namespace bobcat;
using namespace std;

Application::Application() {
    // App's constructor
    initData();
    initInterface();
}

void Application::handleClick(bobcat::Widget *sender) {

    results->clear();
    window->redraw();

    int startIndex = start->value();
    int destIndex = dest->value();

    Waypoint *path = g.ucs(cities[startIndex], cities[destIndex]);

    system("clear");

    if (path) {
        Waypoint *temp = path;
        int y = results->y() + 10;
        int stops = 0;


        while (temp != nullptr) {
            TextBox* cityBox;

            cityBox = new TextBox(40,y,300,25,temp->vertex->data);
            cityBox ->labelfont(Fl_Font(FL_BOLD));
            cityBox->box(FL_UP_BOX);
            results->add(cityBox);
            y+=30;

            if (temp->parent != nullptr){
                TextBox* timeBox;
                timeBox = new TextBox(60,y,260,20, "Flight Time: " + to_string(temp->weight)+ "hours");
                timeBox ->color(FL_DARK_BLUE);
                timeBox ->box(FL_FLAT_BOX);
                results->add(timeBox);
                y+= 25;
                stops ++;
            }
            temp = temp->parent;
        }

        TextBox* stopBox;
        stopBox = new TextBox(40, y +10, 300,25, "Total Stops: " + to_string(stops));
        stopBox->labelfont(Fl_Font(FL_BOLD));
        stopBox->color(Fl_Color(FL_DARK_RED));
        stopBox ->box(FL_FLAT_BOX);
        results->add(stopBox);


        
    }
    window->redraw();
}


void Application::initData() {
    cities.append(new Vertex("San Francisco"));  // 0
    cities.append(new Vertex("New York"));       // 1
    cities.append(new Vertex("Rio De Janeiro")); // 2
    cities.append(new Vertex("Paris"));          // 3
    cities.append(new Vertex("Johannesburg"));   // 4
    cities.append(new Vertex("Moscow"));         // 5
    cities.append(new Vertex("Sydney"));         // 6
    cities.append(new Vertex("Tokyo"));          // 7
    cities.append(new Vertex("Beijing"));        // 8

    for (int i = 0; i < cities.size(); i++) {
        g.addVertex(cities[i]);
    }

    g.addEdge(cities[0], cities[1], 6);
    g.addEdge(cities[1], cities[2], 13);
    g.addEdge(cities[1], cities[3], 7);
    g.addEdge(cities[1], cities[4], 14);
    g.addEdge(cities[1], cities[5], 15);
    g.addEdge(cities[1], cities[6], 40);
    g.addEdge(cities[2], cities[3], 11);
    g.addEdge(cities[2], cities[8], 18);
    g.addEdge(cities[3], cities[6], 17);
    g.addEdge(cities[4], cities[7], 16);
    g.addEdge(cities[4], cities[6], 11);
    g.addEdge(cities[6], cities[7], 10);
    g.addEdge(cities[6], cities[8], 3);
    g.addEdge(cities[8], cities[5], 8);
}

void Application::initInterface() {
    window = new Window(100, 100, 400, 400, "Flight Planner");
    window->color(Fl_Color(FL_WHITE));

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    start->color(Fl_Color(FL_WHITE));

    dest = new Dropdown(20, 100, 360, 25, "Destination");
    dest->color(Fl_Color(FL_WHITE));

    for (int i = 0; i < cities.size(); i++) {
        start->add(cities[i]->data);
        dest->add(cities[i]->data);
    }

    search = new Button(100, 155, 200, 30, "Search");
    search->color(Fl_Color(FL_DARK_BLUE));
    search->labelcolor(Fl_Color(FL_WHITE));
    search->labelfont(Fl_Font(FL_BOLD));

    ON_CLICK(search, Application::handleClick);

    results = new Fl_Scroll(20, 210, 360, 170, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);
    results->color(FL_LIGHT2);

    window->show();
}
