#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "graph.h"
#include "graph_print.h"
#include "macro.h"

int main(int argc, char** argv)
{
    if (argc != 4)
        errx(1, "Usage : *directory of the graph* *source* *destination*");
    struct graph* g = load_graph(argv[1]);

    size_t s = atol(argv[2]);
    size_t d = atol(argv[3]);

    if (d >= g->order)
        errx(1, "dest > order");

    struct node* retour = dijkstra(g, s, d);
    size_t sum = 0;
    struct node* tmp = retour;
    while(tmp != NULL)
    {
        sum += tmp->weight;
        tmp = tmp->next;
    }
    printf("Temps total du trajet: %lu\n\n", sum);

    print_path_terminal(retour, HEIGHT, WIDTH);
}
