#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "dijkstra.h"
#include "graph.h"

void print_tsp(size_t** dists, int len_dest, size_t* destinations)
{
    printf("           ");
    for (int i = 0; i < len_dest; i++)
        printf("%8lu ", destinations[i]);
    printf("\n\n");
    for (int i = 0; i < len_dest; i++)
    {
        printf("%8lu   ", destinations[i]);
        for (int j = 0; j < len_dest; j++)
        {
            printf("%8lu ", dists[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    clock_t start = clock();
    
    struct graph* g = load_graph(argv[1]);
    int len_dest = argc - 2;

    size_t *distances = malloc(g->order * sizeof(size_t));
    size_t *list_prev = malloc(g->order * sizeof(size_t));
    size_t destinations[len_dest];
    
    size_t** tsp_dists = malloc(len_dest * sizeof(size_t*));
    for (int i = 0; i < len_dest; i ++)
        tsp_dists[i] = malloc(len_dest * sizeof(size_t));
    

    for (int i = 0; i < len_dest; i ++)
        destinations[i] = (size_t)atol(argv[i + 2]);


    printf("\n");
    for (int i = 0; i < len_dest; i ++)
    {
        dijkstra(g, len_dest, i, destinations, distances, list_prev);
        for (int j = 0; j < len_dest; j ++)
        {
            tsp_dists[i][j] = distances[destinations[j]];
        }
    }

    print_tsp(tsp_dists, len_dest, destinations);

    /*
    while (n != NULL)
    {
        printf("-(%i)->%lu", n->transport, n->vertex);
        n = n->next;
    }
    printf("\n");
    */

    for (int i = 0; i < len_dest; i ++)
        free(tsp_dists[i]);
    free(tsp_dists);
    free(distances);
    free(list_prev);

    clock_t t = clock() - start;
    printf("total time: %lf\n", ((double) t) / CLOCKS_PER_SEC);
    
}
