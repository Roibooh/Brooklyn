#include <stdlib.h>
#include <stdio.h>
#include <err.h>

//typedef for nodes
struct node
{
    //number of the vertex coresponding
    int vertex;
    //list of 
    struct node* next;
}; 

//typedef for Graph
struct graph 
{
    //order: number of nodes
    size_t order;
    //adjlist : list all connected vertice for each vertex
    struct node** adjlists;
};

struct node* init_node(size_t vertex)
{
    struct node* new_node = malloc(sizeof(struct node));
    new_node->vertex = vertex;
    new_node->next = NULL;
 
    return new_node;
}

struct graph* init_graph(size_t order)
{
    struct graph* graph = malloc(sizeof(struct graph));
    graph->order = order;
    graph->adjlists = calloc(order, sizeof(struct node*));
    
    return graph;
}

void add_edge(struct graph *graph, size_t source, size_t destination)
{
    // Add from source to dest
    struct node* new_node = init_node(destination);
    new_node->next = graph->adjlists[source];
    graph->adjlists[source] = new_node;

    // Add from dest to source
    new_node = init_node(source);
    new_node->next = graph->adjlists[destination];
    graph->adjlists[destination] = new_node;
}

void print_graph(struct graph* graph)
{
    for (size_t vertex = 0; vertex < graph->order; vertex++)
    {
        struct node* tmp = graph->adjlists[vertex];
        printf("\n Vertex %lu\n: ", vertex);
        while (tmp)
        {
            printf("%i -> ", tmp->vertex);
            tmp = tmp->next;
        }
        printf("\n");
    }
}

//only use for the first line of a file
size_t to_num_1(const char* line)
{
    int i = 0;
    while (line[i] != 0)
        i++;

}
struct graph* load_graph(const char* path)
{
    struct graph* new_graph;
    FILE* file = fopen(path, "r");
    if (!file)
        errx(-1, "load_graph: file can't be read");

    //int first = 1;
    //char *source, *destination;
    char *line = calloc(50, sizeof(char));
    while (fscanf(file, "%[^\n] ", line) != EOF)
    {
        printf("> %i\n", atoi(line));/*
        if (first)
            new_graph = init_graph((size_t)atoi(line));
        else
            */
    }

    free(line);
}

int main()
{
    struct graph* graph = init_graph(4);
    add_edge(graph, 0, 1);
    add_edge(graph, 0, 2);
    add_edge(graph, 0, 3);
    add_edge(graph, 1, 2);

    load_graph("test.txt");

    return 0;
}
