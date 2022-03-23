#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "tsp.h"

//used to store data compute by cost fuction
struct chained_list
{
    size_t cost;
    int index;
    struct chained_list* next;
};
void free_chained_list(struct chained_list* cl)
{
    if(cl != NULL)
    {
        free_chained_list(cl->next);
        free(cl);
    }
}

//utility function TODO remove in final build
void print_tsp(size_t** dists, int len_dest, size_t* destinations)
{
    int k;
    printf("                  ");
    for (int i = 0; i < len_dest + 1; i++)
    {
        k = i;
        if (i > 0) {k--;}
        printf("%8lu ", destinations[k]);
    }
    printf("\n\n");
    for (int i = 0; i < len_dest + 1; i++)
    {
        k = i;
        if (i > 0) {k--;}
        printf("%15lu   ", destinations[k]);
        for (int j = 0; j < len_dest + 1; j++)
        {
            printf("%8lu ", dists[i][j]);
        }
        printf("\n");
    }
}

//copy a list while removing the designed element from it, 
//return is new list pointer
int* remove_index(int* list, int index_remove, int len)
{
    int* cp_list = malloc((len - 1) * sizeof(int));
    int i = 0;
    while (i != index_remove)
    {
        cp_list[i] = list[i];
        i ++;
    }
    while (i < len - 1)
    {
        cp_list[i] = list[i + 1];
        i ++;
    }
    return cp_list;
}

//apply the Held-Karp algorithm to resolve the TSP
//the shortest path is stored in the return chained list
struct chained_list* cost(size_t** dis, int index, int* index_list, int len)
{
    //create return element
    struct chained_list* r = malloc(sizeof(struct chained_list));
    r->cost = ULONG_MAX;
    r->next = NULL;
    r->index = index;
    
    //base case when the set is empty
    if (len == 0)
    {
        r->cost = dis[index][0];
        return r;
    }

    struct chained_list* temp;
    size_t temp_min;
    for (int i = 0; i < len; i ++)
    {
        int* cp_list = remove_index(index_list, i, len);
        temp = cost(dis, index_list[i], cp_list, len - 1);
        temp_min = temp->cost + dis[index][index_list[i]];
        if (temp_min < r->cost)
        {
            r->cost = temp_min; 
            r->next = temp;
        }
    }
    free(index_list);
    return r;
}

struct node* tsp_main(struct graph* g, size_t* destinations, int len_dest)
{
    //lists for dijkstra algorithm
    size_t* distances = malloc(g->order * sizeof(size_t));
    size_t* list_prev = malloc(g->order * sizeof(size_t));
    // + 1 for the dummy point
    //tsp_dists is a distance matrice conaining destinations
    size_t** tsp_dists = malloc((len_dest + 1) * sizeof(size_t*));
    for (int i = 0; i < len_dest + 1; i ++)
        tsp_dists[i] = malloc((len_dest + 1 ) * sizeof(size_t));
    
    //paths is a list for all the shortest path between destinations
    size_t** paths = malloc(len_dest * sizeof(size_t*));
    for (int i = 0; i < len_dest; i ++)
        paths[i] = malloc(g->order * sizeof(size_t));
    
    //here i is the index in destinations where we work
    for (int i = 1; i < len_dest + 1; i ++)
    {
        dijkstra(g, len_dest, i - 1, destinations, distances, list_prev);
        memcpy(paths[i - 1], list_prev, g->order * sizeof(size_t));
        for (int j = 1; j < len_dest + 1; j ++)
        {
            tsp_dists[i][j] = distances[destinations[j - 1]];
        }
    }
    //dummy point 0 from everything will be the starting point
    for (int i = 0; i < len_dest + 1; i ++)
    {
        tsp_dists[0][i] = 0;
        tsp_dists[i][0] = 0;
    }
    //data used for the cost function
    struct chained_list* cl;
    int* set = malloc(len_dest * sizeof(int));
    for (int i = 0; i < len_dest; i ++)
        set[i] = i + 1;

    //computing the cost and the shortest path for the TSP
    //stored in the chained list cl
    cl = cost(tsp_dists, 0, set, len_dest);
    struct chained_list* tmp_l = cl->next;  

    struct node* final = init_node(ULONG_MAX);
    struct node* tmp_n = final;
    for (int i = 0; i < len_dest - 1; i ++)
    {
        tmp_n->next = build_return(g, destinations[tmp_l->index - 1],\
                destinations[tmp_l->next->index - 1], paths[tmp_l->index - 1]);
        tmp_l = tmp_l->next;
        while (tmp_n->next != NULL)
            tmp_n = tmp_n->next;
    }

    print_tsp(tsp_dists, len_dest, destinations);
    printf("\n%lu\n", cl->cost);
    while (cl != NULL)
    {
        printf("%i ", cl->index);
        cl = cl->next;
    }

    //free all the data used
    free_chained_list(cl);
    free(distances);
    free(list_prev);
    for (int i = 0; i < len_dest + 1; i ++)
        free(tsp_dists[i]);
    free(tsp_dists);

    return final;
}
