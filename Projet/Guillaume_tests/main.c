#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "graph.h"

int main(int argc, char** argv)
{
    if (argc != 4)
        errx(1, "les arguments connard");
    struct graph* g = load_graph(argv[1]);
    print_graph(g);
    printf("\n");

    size_t s = atoi(argv[2]);
    size_t d = atoi(argv[3]);

    if (d >= g->order)
        errx(1, "dest > order");

    struct node* n = dijkstra(g, s, d);
    
    while (n != NULL)
    {
        printf("%lu --> ", n->vertex);
        n = n->next;
    }
    printf("\n");
}
