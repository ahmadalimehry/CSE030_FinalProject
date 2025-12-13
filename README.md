# Flight Route Planner – CSE030 Final Project

**Link to GitHub repository:**  
https://github.com/ahmadalimehry/CSE030_FinalProject

---

## Project Overview
This project is a command-line program that allows users to plan air travel between different airports. Airports are represented as vertices in a graph, and direct flights between airports are represented as edges.

The user selects:
- A starting airport  
- A destination airport  
- A routing preference  

The program then computes and displays the best flight route based on the selected preference.

---

## Routing Options
Our project supports the following features:
- Cheapest price  
- Shortest travel time  
- Least number of stops  

The output includes:
- The full flight itinerary  
- Total travel time  
- Total estimated cost  
- Number of stops  

---

## Implementation Details
The project uses the Graph data structure provided in class.

Flights may be searched by:
- Cheapest price  
- Shortest travel time  
- Least number of stops  

Flight data (airports and routes) is loaded from external CSV-like files, making data easy to modify without changing the code.

Syntax for data files:
- Directive:
    - `#use "path"`
        - `"path"` must point to a valid Definition file in the directory flights_data/
- Comments:
    - All files beginning with `//` are ignored and may be used as comments
- CSV Portion
    - Layout: ```<from>,<to>,<hours[int]>,<cost_dollars[int]>,<directional[0/1]>```
    - `from` & `to`: Location Identifiers may be strings or aliases declared
    - `hours` & `dollars`: Integers which denote their respective values
    - `directional`: `0` if flights may go both ways, `1` if flights only go from `from` to `to`

Syntax for Definition files:
- Defines a small, declarative syntax for declaring string literals and assigning aliases to them.
- 3 Types of Declarations:
    - A standalone literal:
        - ```"San Francisco"```
    - Alias for literal:
        - ```lax = "Los Angeles"``` The string literal is implicitly declared if it does not exist yet.
    - Alias Copy:
        - ```la = lax``` Creates a new alias for the value aliased by `lax`

---

## User Interface
Our project uses a command-line interface that allows user to:
- View all airports  
- Plan a flight  
- Change flight data source
- Return to the main menu or exit  


The screen is cleared between pages for better user experience.
