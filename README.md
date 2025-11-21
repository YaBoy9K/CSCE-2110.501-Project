# CSCE-2110.501-Project
Route Search - README

Files included:
 - ProjectRevision3.cpp :  C++ source with interactive menu
 - flight.txt          : input flight data 
 - README.txt      : this file
 - REPORT.pdf      :  project report

Run:
  - Make sure flight.txt is in the same folder as the executable, Or change Line if(!parse_file("flight.txt")) to match the file path
  - Run the program; it will show a menu and ask for inputs interactively.
    Example (after starting the program):
      Choose option 1, then enter source and destination and max connections when prompted.
    
Sample Input/Output:
 Menu:
1) Search route using country/city selection
2) Route from A to D through B and C (order not important)
3) Start at A, visit all reachable cities and return to A (simple DFS tour)
4) Find meeting city for A, B, C minimizing total connections
0) Exit
Choose: 1
List of countrys:
 0)Russia
 1)SAR
 2)United States
 3)South Korea
 4)United Kingdom
 5)Denmark
 6)Singapore
 7)Switzerland
 8)Italy
 9)Japan
 10)France
 11)People's Republic of China
 12)Turkey
 13)Ireland
 14)Norway
 15)Australia
...

Enter valid FROM Country number: 0
List of citys
0)Moscow
1)St. Petersburg

Enter valid FROM City number: 0
List of countrys:
 0)Russia
 1)SAR
 2)United States
 3)South Korea
 4)United Kingdom
 5)Denmark
 6)Singapore
 7)Switzerland
 8)Italy
 9)Japan
 10)France
 11)People's Republic of China
 12)Turkey
 13)Ireland
 14)Norway
 15)Australia
...

Enter valid TO Country number: 3
List of citys
0)Seoul

Enter valid TO City number: 0
Moscow, Russia to Seoul, South Korea
total connection: 1
