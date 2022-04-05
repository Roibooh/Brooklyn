#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "graph.h"
#include "binary_heap.h"
#include "macro.h"

struct node* build_node(struct graph* graph, size_t source, size_t dest)
{
    struct node* r = init_node(dest);
    struct node* tmp = graph->adjlists[source];
    while (tmp->vertex != dest)
    {
        tmp = tmp->next;
    }
    r->weight = tmp->weight;
    r->transport = tmp->transport;
    return r;
}
struct node* build_return(struct graph* graph, size_t source, \
        size_t destination, size_t* list_prev)
{
    struct node* null = NULL;
    struct node** last = &null;
    struct node** cur;
    struct node* tmp;
    size_t cur_index = list_prev[destination];
    while(destination != source)
    {
        tmp = build_node(graph, cur_index, destination);
        tmp->next = *last;
        cur = &tmp;
        *last = *cur;

        destination = cur_index;
        cur_index = list_prev[destination];
    }
    tmp = init_node(source);
    tmp->next = *last;
    return tmp;
}
/* 
**  nb_dest >= 2
*/
void dijkstra(struct graph* graph, int nb_dest, int index_source,\
        size_t* destination, size_t* distances, size_t* list_prev)
{
    int len_dest = nb_dest;
    struct bin_heap* bh = bin_heap_init(graph->order);

    //initialize lists to base value
    distances[destination[index_source]] = 0;
    for (size_t v = 0; v < graph->order; v++)
    {
        if (v != destination[index_source])
        {
            distances[v] = ULONG_MAX;
            list_prev[v] = ULONG_MAX;
        }
        insert(bh, distances[v], v);
    }

    while (bh->heap_size != 0)
    {
        int i;
        //get the closest node accessible
        size_t min = extract_min(bh);
        //if it is one the desired destination
        for (i = 0; i < len_dest; i++)
            if (min == destination[i])
                break;
        if (i != len_dest)
        {
            nb_dest -= 1;
            //if all dest has been seen break
            if (nb_dest == 0)
                break;
        }
        //get the nodes adjacent to vertex_min_dist
        struct node* adj = graph->adjlists[min];
        //goes through all nodes adjacent to verrtex_min_dist
        while (adj)
        {
            //compute theoritical distance
            size_t tmp = distances[min] + adj->weight;
            //if it is better than what is already known, update
            if (tmp < distances[adj->vertex])
            {
                distances[adj->vertex] = tmp;
                list_prev[adj->vertex] = min;
                find(bh, tmp, adj->vertex);
            }
            adj = adj->next;
        }
    }
    free_bin_heap(bh);
}
