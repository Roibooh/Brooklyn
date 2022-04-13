#ifndef GRAPH_H
#define GRAPH_H
#include <stddef.h>

//structur for a node
struct node
{
    //index of the node in the node list
    size_t vertex;
    //chained list of adjacent nodes
    struct node* next;
    //weight of the edge between the node and the coresponding adjacent node
    size_t weight;
    int transport;
};

//structut for non orriented weighted graph using an adjacen
struct graph
{
    //number of nodes in the graph
    size_t order;
    //dynamic list of node in the graph
    struct node** adjlists;
};

/*  allocate and create a node for a graph
**  vertex will be the vertex of the node*/
struct node* init_node(size_t vertex);

/*  allocate and create a graph 
**  order is the number of node in the graph*/
struct graph* init_graph(size_t order);

//free the memory allocate for a node and all linked one
void free_node(struct node* n);

//free a graph and call free node for all the node in the graph
void free_graph(struct graph* g);

/*  main fonction to load a graph
**  call load_main and load_transport for each file*/
struct graph* load_graph(const char* file);

void print_graph(struct graph* graph);

#endif
