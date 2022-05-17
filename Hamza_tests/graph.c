#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "macro.h"
#include "graph.h"

struct node* init_node(size_t vertex)
{
    /*
    **  allocate and create a node for a graph
    **  vertex will be the vertex of the node
    */
    struct node* new_node = malloc(sizeof(struct node));
    new_node->vertex = vertex;
    new_node->next = NULL;
    new_node->weight = 0;
    new_node->transport = UNDEFINED;

    return new_node;
}
struct graph* init_graph(size_t order)
{
    /*
    **  allocate and create a graph 
    **  order is the number of node in the graph
    */
    struct graph* graph = malloc(sizeof(struct graph));
    graph->order = order;
    graph->adjlists = calloc(order, sizeof(struct node*));
    
    return graph;
}
void free_node(struct node* n)
{
    /*
    **  free the memory allocate for a node and all linked one
    */
    struct node* tmp;

    while (n != NULL)
    {
        tmp = n;
        n = n->next;
        free(tmp);
    }
}
void free_graph(struct graph* g)
{
    /*
    ** free a graph and call free node for all the node in the graph
    */
    for (size_t i = 0; i < g->order; i++)
        free_node(g->adjlists[i]);
    free(g->adjlists);
    free(g);
}

void add_edge(struct graph *graph, size_t source, size_t destination, \
        size_t weight, int transport, int directioned)
{
    /*
    ** add and edge in graph
    ** the edge will go frome source to destination
    ** weight is the cost of the edge
    ** transport indeicate the way of transport used
    ** directionned is a boolean true if it is a one way edge
    */
    // Add from source to dest
    struct node* new_node = init_node(destination);
    new_node->next = graph->adjlists[source];
    new_node->weight = weight;
    new_node->transport = transport;
    graph->adjlists[source] = new_node;
    
    if (directioned == FALSE)
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
struct graph* load_main(const char* file)
{
    /*
    **  load the main file of the graph
    **  it contains the order of the graph and
    **  it also contains the edges used when walking
    **
    **  a line of the file store the data as such
    **  s d w
    **  s is the source
    **  d the destination
    **  w the weight
    **  each line is an edge
    */
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
            source = atol(word); //cast to number
        }
        else if (d)
        {
            d = 0;
            destination = atol(word);
            //destination
        }
        else if (w)
        {
            w = 0;
            weight = atol(word);
        }
        else
        {
            //first one
            g = init_graph(atol(word)); //cast to number
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
            add_edge(g, source, destination, weight, WALK, FALSE);
        }
    }
    fclose(fp);
    return g;
}
void load_transport(struct graph* g, const char* file, int transport)
{
    /*
    **  Same as load_main but for other transports
    **  they are stored in different files
    **  the data are stored in the way
    */
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
            source = atol(word);
        }
        else if (d)
        {
            d = 0;
            destination = atol(word);
        }
        else if (w)
        {
            w = 0;
            weight = atol(word);
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
    fclose(fp);
}
struct graph* load_graph(const char* path)
{
    /*
    **  main fonction to load a graph
    **  call load_main and load_transport for each file
    */
    char s[100];
    sprintf(s, "%smain.txt", path);
    struct graph* g = load_main(s);
    
    sprintf(s, "%sbikes.txt", path);
    load_transport(g, s, BIKE);

    sprintf(s, "%sbus.txt", path);
    load_transport(g, s, BUS);
    
    sprintf(s, "%smetro.txt", path);
    load_transport(g, s, METRO);

    sprintf(s, "%stram.txt", path);
    load_transport(g, s, TRAM);

    return g;
}
