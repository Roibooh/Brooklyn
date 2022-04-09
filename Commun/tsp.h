#ifndef TSP_H
#define TSP_H

#include "graph.h"
#include "dijkstra.h"
#include "macro.h"

//is_no_start : bool true if the starting point for tsl is NOT important
struct node* tsp_main(struct graph* g, size_t* destinations, int len_dest,\
        int is_no_start);

#endif
