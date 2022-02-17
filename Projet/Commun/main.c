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

    size_t s = atol(argv[2]);
    size_t d = atol(argv[3]);

    if (d >= g->order)
        errx(1, "dest > order");

    struct node* retrour = dijkstra(g, s, d);
}
