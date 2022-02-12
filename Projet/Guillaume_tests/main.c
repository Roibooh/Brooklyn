#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "dijkstra.h"
#include "graph.h"

int main(int argc, char** argv)
{
    clock_t start = clock();
    if (argc != 4)
        errx(1, "les arguments connard");
    struct graph* g = load_graph(argv[1]);
    //print_graph(g);
    //printf("\n");

    size_t s = atoi(argv[2]);
    size_t d = atoi(argv[3]);

    if (d >= g->order)
        errx(1, "dest > order");

    struct node* n = dijkstra(g, s, d);
    printf("\n");
    while (n != NULL)
    {
        printf("%lu -(%i)-> ", n->vertex, n->transport);
        n = n->next;
    }
    printf("\n");
    clock_t t = clock() - start;
    printf("total time: %lf\n", ((double) t) / CLOCKS_PER_SEC);
}
