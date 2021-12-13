# C-Network-graph

An Internet service provider wants to deliver service to a village. They have two installation
options:  
• Cabled installation. This means connecting the data center via underground cables. Con-
nections can be either made from the data center to a house or between houses. The cost is
equal to the distance between houses (or between a data center and a house).  
• Radio-based installation. This means installing antennas in each house, which then receive
the internet signal through a satellite. The cost of an antenna installation is fixed per house
but can vary from village to village.  
  
The text below describes how a village may be connected:  

 25  
 6 10  
 0 1 40  
 0 2 20  
 1 2 15  
 1 3 68  
 2 4 33  
 2 5 28  
 3 5 19  
 3 6 31  
 4 5 52  
 5 6 77  
  
where:  
• First line contains the antenna cost.  
• Second line shows the number of houses followed by the number of connections in the
village.  
• Remaining lines show connections where the first and second number are the houses (or 0 if
it is the data center) and the third number is the actual distance.  

## Part A
Devise a program in C that given a village map and antenna installation cost in the format de-
scribed above, returns the installation option with lower cost in total. Your program should out-
put the lowercase character c if cabled has the lower installation cost and r if radio-based has the
lower cost instead.

## Part B
An explanation of the algorithm used:

This program works because for the cost of connecting all houses using antennas, it would be the
cost of an antenna multiplied by the number of houses (since we are installing one antenna on each
house). For the cost of connecting all houses with cables, we would need to use a minimum
spanning tree to obtain the lowest cost possible for connected all houses via cables and so I used
Prim’s algorithm to do this (since Prim’s algorithm generates a path connecting all nodes with
minimum total edge weight). We then have the total cost of installing antennas in each house as
well as the total minimum cost of connecting all houses by cables and we will return the option with
the lower total cost.

## Part C
Same as Part A, but now the company updated their infrastructure and it can now provide a mix
of cabled and radio-based installations for the village. This means that a house will have internet
access if any of the following are true:
1. There is a path to the data center,
2. The house has an antenna or
3. There is a path to a house with an antenna.
