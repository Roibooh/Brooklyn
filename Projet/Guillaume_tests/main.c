#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "graph.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(1, "un argument connard");
    struct graph* g = load_graph(argv[1]);
    print_graph(g);
    printf("\n");

    size_t s = 0;
    size_t d = 8;

    struct node* n = dijkstra(g, s, d);
    while (n != NULL)
    {
        printf("%lu --> ", n->vertex);
        n = n->next;
    }
}
