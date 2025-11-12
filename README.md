# CSCE-2110.501-Project
CSCE2110 - Group Project

Files included:
 - routeSearch.cpp   : main C++ source
 - flight.txt        : input data
 - README.txt        : this file
 - REPORT.txt        : short project report

Compile:
  g++ -std=c++11 -O2 routeSearch.cpp -o routeSearch

Usage examples (city names must match exactly those in flight.txt; if they contain spaces, quote them on the command line):

1) Shortest route with connection limit:
  ./routeSearch 1 "London, United Kingdom" "Tokyo, Japan" 3

2) Route from A to D through B and C (order of B and C not important):
  ./routeSearch 2 "A" through "B" and "C" to "D"
Example using real names:
  ./routeSearch 2 "Moscow, Russia" through "Seoul, South Korea" and "Tokyo, Japan" to "Paris, France"

3) Start at A, visit all reachable cities and return to A (DFS-based heuristic):
  ./routeSearch 3 "London, United Kingdom"

4) Three people meet minimizing total connections:
  ./routeSearch 4 "London, United Kingdom" "New York City, United States" "Geneva, Switzerland"
