#ifndef DISPLAY_H
#define DISPLAY_H

#include <ArrayList.h>
#include <Graph.h>
#include <cstddef>
#include <ios>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

void clearScreen();

int computeCost(Waypoint *end);

void displayBanner();

void showMenu();

int getMenuChoice();

void displayAirports(Graph *g);

int chooseAirports(Graph *g, string prompt);

int choosePreferences();

void displayRoute(Waypoint *end);

void waitForEnter();

#endif
