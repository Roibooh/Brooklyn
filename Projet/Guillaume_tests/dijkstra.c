#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "graph.h"
#include "priority_queue.h"
#include "macro.h"

//return the closest node not marked
size_t get_min_in_Q(int* vertex_set_Q, size_t* distances, size_t order)
{
    size_t min_dist = ULONG_MAX;
    size_t vertex_min_dist = 0;

    for (size_t i = 0; i < order; i++)
    {
        if (vertex_set_Q[i] && distances[i] < min_dist)
        {
            min_dist = distances[i];
            vertex_min_dist = i;
        }
    }
    return vertex_min_dist;
}

void add_node(struct node* n, size_t destination)
{
    struct node* nn = init_node(destination);
    n->next = nn;
}

void reverse(struct node** n)
{
    struct node* pre = NULL;
    struct node* cur = *n;
    struct node* nex = NULL;
    while (cur != NULL)
    {
        nex = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nex;
    }
    *n = pre;
}

struct node* dijkstra(struct graph* graph, size_t source, size_t destination)
{
    size_t* distances = malloc(graph->order * sizeof(size_t));
    size_t* list_prev = malloc(graph->order * sizeof(size_t));
    struct queue_elt* queue = NULL;
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
        //printf("extracted: %lu, now: %li\n", min, queue->node);
        //if it is the desired destination, can return
        if (min == destination)
        {
            printf("found !\n");
            struct node* r_n = init_node(destination);
            struct node* tmp = r_n;
            destination = list_prev[destination];
            while (distances[destination] != 0)
            {
                add_node(tmp, destination);
                tmp = tmp->next;
                destination = list_prev[destination];
            }
            add_node(tmp, 0);
            
            reverse(&r_n);
            free(distances);
            free(list_prev);
            free_queue(queue);
            return r_n;
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
                //printf("change prio of %lu to %lu\n", adj->vertex, tmp);
                change_prio(&queue, adj->vertex, tmp);
                
            }
            adj = adj->next;
        }
    }
    free(distances);
    free_queue(queue);
    free(list_prev);
    return NULL;
}
