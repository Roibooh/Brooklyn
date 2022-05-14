#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

#define TRUE 1
#define FALSE 0

#define UNDEFINED -1
#define WALK 0
#define BUS 1
#define METRO 2
#define BIKE 3
#define TRAM 4

//init a node numbered "vertex"
struct node* init_node(size_t vertex)
{
    struct node* new_node = malloc(sizeof(struct node));
    new_node->vertex = vertex;
    new_node->next = NULL;
    new_node->weight = 0;
    new_node->transport = UNDEFINED;

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
        size_t weight, int transport, int directioned)
{
    // Add from source to dest
    struct node* new_node = init_node(destination);
    new_node->next = graph->adjlists[source];
    new_node->weight = weight;
    new_node->transport = transport;
    graph->adjlists[source] = new_node;
    
    if (directioned == TRUE)
    {
        // Add from dest to source
        new_node = init_node(source);
        new_node->next = graph->adjlists[destination];
        new_node->weight = weight;
        new_node->transport = transport;
        graph->adjlists[destination] = new_node;
    }
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
//load the main graph from a file
struct graph* load_main(const char* file)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        err(1, "load_main: error in fopen");
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
            add_edge(g, source, destination, weight, WALK, TRUE);
        }
    }
    fclose(fp);
    return g;
}
void load_transport(struct graph* g, const char* file, int transport)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        err(1, "load_transport: error in fopen");
    
    char word[25];
    size_t source;
    size_t destination;
    int weight;

    int s = 1;
    int d = 1;
    int w = 1;
    while (fscanf(fp, "%99s", word) == 1)
    {
        if (s)
        {
            s = 0;
            source = atoi(word);
        }
        else if (d)
        {
            d = 0;
            destination = atoi(word);
        }
        else if (w)
        {
            w = 0;
            weight = atoi(word);
        }
        if (d == 0 && s == 0 && w == 0)
        {
            if (source >= g->order || destination >= g->order)
                errx(1, "load_graph: node number > order");
            s = 1;
            d = 1;
            w = 1;
            add_edge(g, source, destination, weight, transport, \
                    transport == BUS);
        }
    }
}
struct graph* load_graph(const char* path)
{
    char s[100];
    sprintf(s, "%smain", path);
    struct graph* g = load_main(s);
    
    sprintf(s, "%smetro", path);
    load_transport(g, s, METRO);

    return g;
}
