#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "macro.h"
#include "tsp.h"
#include "graph_print.h"

int main(int argc, char** argv)
{
    
    struct graph* g = load_graph(argv[1]);
    int len_dest = argc - 2;

    size_t destinations[len_dest];
    for (int i = 0; i < len_dest; i ++)
    {
        size_t d = (size_t)atol(argv[i + 2]);
        if (d >= g->order)
            errx(EXIT_FAILURE, "dest > g->order");
        destinations[i] = d;
    }
    struct node* final = tsp_main(g, destinations, len_dest, TRUE);
    final = final->next;
    
    printf("\n");
    printf("\n");
    print_path_terminal(final, HEIGHT, WIDTH);

}
