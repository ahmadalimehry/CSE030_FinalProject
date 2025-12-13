# Flight Route Planner – CSE030 Final Project

**Link to GitHub repository:**  
https://github.com/ahmadalimehry/CSE030_FinalProject

---

## Project Members
- lgarciasanchez@ucmerced.edu  
- johnolivas@ucmerced.edu  
- aramirez435@ucmerced.edu 
- amehry@ucmerced.edu
- smora9@ucmerced.edu

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

Uniform Cost Search (UCS) is used for:
- Cheapest price  
- Shortest travel time  

Breadth-First Search (BFS) is used for:
- Least number of stops  

Flight data (airports and routes) is loaded from external text files, making the graph easy to modify without changing the code.

---

## User Interface
Our project uses a command-line interface that allows user to:
- View all airports  
- Plan a flight  
- Return to the main menu or exit  


The screen is cleared between pages for better user experience.
