#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/*
**  Base function for the Dijkstra algorithm
**      graph: graph to be searched
**      nb_dest: number of dest that has to be found
**      index_source: index of the source node in destinations
**      destinations: list of node to be found
**      distances: list to store the distance from source
**      list_prev: list to store the previous node in the path 
**          from source
*/
void dijkstra(struct graph* graph, int nb_dest, int index_source,\
        size_t* destinations, size_t* distances, size_t* list_prev);


/* 
** Create a linked list made of nodes
**  The list will go from source to dest and contains the full path
**  stored in list_prev
*/
struct node* build_return(struct graph* graph, size_t source, \
        size_t destination, size_t* list_prev);

#endif
