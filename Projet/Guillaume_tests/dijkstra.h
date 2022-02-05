#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stddef.h>
#include "graph.h"

struct node* dijkstra(struct graph* graph, size_t source, size_t destination);

#endif
