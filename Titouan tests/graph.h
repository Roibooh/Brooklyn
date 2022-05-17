#ifndef GRAPH_H
#define GRAPH_H
#include <stddef.h>
#include "macro.h"

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
//initialiser for struct
struct node* init_node(size_t vertex);
struct graph* init_graph(size_t order);
//destroyer
void free_node(struct node* n);
void free_graph(struct graph* g);

//load a graph stored in file
struct graph* load_graph(const char* file);
//print all edge in a graph
void print_graph(struct graph* graph);

#endif
