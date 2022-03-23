#ifndef TSP_H
#define TSP_H

#include "graph.h"
#include "dijkstra.h"

struct node* tsp_main(struct graph* g, size_t* destinations, int len_dest);

#endif
