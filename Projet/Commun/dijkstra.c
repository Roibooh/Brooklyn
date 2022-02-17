#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "graph.h"
#include "priority_queue.h"
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
**  source != destination
*/
struct node* dijkstra(struct graph* graph, size_t source, size_t destination)
{
    size_t* distances = malloc(graph->order * sizeof(size_t));
    size_t* list_prev = malloc(graph->order * sizeof(size_t));
    struct queue_elt* queue = NULL;
    struct node* r_node = NULL; 
    //initialize lists to base value
    distances[source] = 0;
    for (size_t v = 0; v < graph->order; v++)
    {
        if (v != source)
        {
            distances[v] = ULONG_MAX;
            list_prev[v] = ULONG_MAX;
        }
        insert(&queue, v, distances[v]);
    }

    for (size_t i = 0; i < graph->order; i++)
    {
        //get the closest node accessible
        size_t min = extract_min(&queue);
        //if it is the desired destination, can return
        if (min == destination)
        {
            r_node = build_return(graph, source, destination, list_prev);
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
                change_prio(&queue, adj->vertex, tmp);
            }
            adj = adj->next;
        }
    }
    free(distances);
    free_queue(queue);
    free(list_prev);
    return r_node;
}
