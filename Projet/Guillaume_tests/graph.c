#include <stdlib.h>
#include <stdio.h>
#include <err.h>

//typedef for nodes
struct node
{
    //number of the vertex coresponding
    int vertex;
    //chained list of adjnodes 
    struct node* next;
    //weight associated with the next adj edge
    int weight;
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
    new_node->weight = 0;
    return new_node;
}

struct graph* init_graph(size_t order)
{
    struct graph* graph = malloc(sizeof(struct graph));
    graph->order = order;
    graph->adjlists = calloc(order, sizeof(struct node*));
    
    return graph;
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
            printf(" -(%i)-> %i", tmp->weight, tmp->vertex);
            tmp = tmp->next;
        }
        printf("\n");
    }
}

struct graph* load_graph(const char* file)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        err(1, "load_graph: error in fopen");
    char word[100];
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

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(1, "wrong number of args");
    
    struct graph* g = load_graph(argv[1]);
    
    print_graph(g);

    return 0;
}
