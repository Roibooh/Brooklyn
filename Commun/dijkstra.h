#ifndef DIJKSTRA_H
#define DIJKSTRA_H

//distances and list_prev should be of length graph->order
void dijkstra(struct graph* graph, int nb_dest, int index_source,\
        size_t* destinations, size_t* distances, size_t* list_prev);
struct node* build_return(struct graph* graph, size_t source, \
        size_t destination, size_t* list_prev);
#endif
