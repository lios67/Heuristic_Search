# Heuristic_Search
g++ Heuristic_Search.cpp -std=c++11

./a.out Heuristic_Search.cpp 

enter number from 1-4 for the corrosponding algorithm
1 : A* search using the heuristic function f*(n) = g(n) + h*(n), where h*(n) is the number of tiles out of place (not counting the blank).  

2 :A* search using the Manhattan heuristic function. 

3 :Iterative deepening A* with the Manhattan heuristic function.

4 :Depth-first Branch and Bound with the Manhattan heuristic function.

then enter the data one line of data
1 3 4 8 6 2 7 0 5       //easy
2 8 1 0 4 3 7 6 5       //medium
2 8 1 4 6 3 0 7 5       //hard
5 6 7 4 0 8 3 2 1       //worst
