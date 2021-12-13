/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32
#define INF 9999
struct edge;

int prims(struct graph *g);

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* vertex struct for prims algorithm */
struct primQ{
  int vert;
  int cost;
  int prev;
};

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

/* we need to find the cost of the minimum spanning tree
so we will use prims algorithm (adapted from psuedocode in comp2007
lecture 8) */
int prims(struct graph *g){

  /* total cost of edges in current minimum spanning tree */
  int minCost=0;
  int i;
  /* array of vertices (keeping track of current node, previous node and
  cost) */
  struct primQ elem[g->numVertices];
  /* current vertex */
  struct primQ *current;
  /* create a new priority queue */ 
  struct pq *pq = newPQ();

  /* initialisation of the vertices in the elem array */
  for (i=0; i<g->numVertices; i++){
    elem[i].vert = i;
    /* initialise with no previous node */
    elem[i].prev = -1;
    /* initialise with a cost of infinity */
    elem[i].cost = INF;
    /* enqueue the vertex with its cost as the key(priority) */
    enqueue(pq, &elem[i], elem[i].cost);
  }

  /* pick initial node (datacenter) */
  /* the datacentre should have vertex 0, no previous vertex, and cost of 0 */
  elem[0].vert = 0;
  elem[0].prev = -1;
  elem[0].cost = 0;
  /* queue the datacenter */
  enqueue(pq, &elem[0], elem[0].cost);

  /* continue prims algorithm as long as the queue isn't empy */
  while(!empty(pq)){
    current = deletemin(pq);
    /* keep track of the total cost of the minimum spanning tree */
    minCost += current->cost;
    for(i=0; i<g->numEdges; i++){
      
      /* for each edge if vertex is in Q and edge cost is the lowest, update
      the cost to get to that vertex */

      if((g->edgeList[i]->cost < elem[g->edgeList[i]->end].cost) && 
      g->edgeList[i]->start == current->vert){

        elem[g->edgeList[i]->end].cost = g->edgeList[i]->cost;
        elem[g->edgeList[i]->end].prev = current->vert;

        /* rearrange priority queue */
        updateQ(pq, &elem[g->edgeList[i]->end], g->edgeList[i]->cost);
      }

      if((g->edgeList[i]->cost < elem[g->edgeList[i]->start].cost) && 
      g->edgeList[i]->end == current->vert){

        elem[g->edgeList[i]->start].cost = g->edgeList[i]->cost;
        elem[g->edgeList[i]->start].prev = current->vert;

        /* rearrange priority queue */
        updateQ(pq, &elem[g->edgeList[i]->start], g->edgeList[i]->cost);
      }
    }
  }

  /* free memory allocated for the priority queue */
  freePQ(pq);
  /* return the total cost of minimum spanning tree */
  return minCost;
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);
  if(part == PART_A){
    /* IMPLEMENT 2A SOLUTION HERE */
    solution->antennaTotal = 0;
    solution->cableTotal = 0;
    /* cost of antenna connection is the cost of each antenna multiplied
    by the number of houses */
    solution->antennaTotal = antennaCost * numHouses;
    /* to find cost of cables, we must use prims algorithm to find
    the cost of the minimum spanning tree */
    solution->cableTotal = prims(g);
  } else {
    /* IMPLEMENT 2C SOLUTION HERE */
    solution->mixedTotal = 0;
    /* for this we will add an edge between the datacentre and every house */
    int i;
    for (i=1; i<numHouses+1; i++){
      addEdge(g,0,i,antennaCost);
    }
    /* apply prims algorithm again (same as parta but with the edges between
    the datacentre and the houses) */
    solution->mixedTotal = prims(g);
  }
  return solution;
}