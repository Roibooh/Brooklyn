#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

//init a node numbered "vertex"
struct node* init_node(size_t vertex)
{
    struct node* new_node = malloc(sizeof(struct node));
    new_node->vertex = vertex;
    new_node->next = NULL;
    new_node->weight = 0;
    
    return new_node;
}
//init graph
struct graph* init_graph(size_t order)
{
    struct graph* graph = malloc(sizeof(struct graph));
    graph->order = order;
    graph->adjlists = calloc(order, sizeof(struct node*));
    
    return graph;
}
//free node
void free_node(struct node* n)
{
    if (n->next)
        free_node(n->next);
    free(n);
}
//free graph
void free_graph(struct graph* g)
{
    for (size_t i = 0; i < g->order; i++)
        free_node(g->adjlists[i]);
    free(g->adjlists);
    free(g);
}

void add_edge(struct graph *graph, size_t source, size_t destination, \
        int weight)
{
    // Add from source to dest
    struct node* new_node = init_node(destination);
    new_node->next = graph->adjlists[source];
    new_node->weight = weight;
    graph->adjlists[source] = new_node;

    // Add from dest to source
    new_node = init_node(source);
    new_node->next = graph->adjlists[destination];
    new_node->weight = weight;
    graph->adjlists[destination] = new_node;
}

void print_graph(struct graph* graph)
{
    printf("order: %lu\n", graph->order);
    for (size_t vertex = 0; vertex < graph->order; vertex++)
    {
        struct node* tmp = graph->adjlists[vertex];
        printf("Vertex %lu\n: ", vertex);
        while (tmp)
        {
            printf(" -(%lu)-> %lu", tmp->weight, tmp->vertex);
            tmp = tmp->next;
        }
        printf("\n");
    }
}
//load a graph from a file
struct graph* load_graph(const char* file)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        err(1, "load_graph: error in fopen");
    char word[25];
    struct graph* g;


    size_t source;
    size_t destination;
    int weight;

    int s = 0;
    int d = 0;
    int w = 0;
    while (fscanf(fp, "%99s", word) == 1)
    {
        if (s)
        {
            //source
            s = 0;
            source = atoi(word); //cast to number
        }
        else if (d)
        {
            d = 0;
            destination = atoi(word);
            //destination
        }
        else if (w)
        {
            w = 0;
            weight = atoi(word);
        }
        else
        {
            //first one
            //printf("order: %i\n", atoi(word));
            g = init_graph(atoi(word)); //cast to number
            s = 1;
            d = 1;
            w = 1;
        }
        if (d == 0 && s == 0 && w == 0)
        {
            if (source >= g->order || destination >= g->order)
                errx(1, "load_graph: node number > order");
            s = 1;
            d = 1;
            w = 1;
            //printf("%i --> %i\n", source, destination);
            add_edge(g, source, destination, weight);
        }
    }
    fclose(fp);
    return g;
}
