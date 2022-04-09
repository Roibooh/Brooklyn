#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

#include "tsp.h"

struct thread_data
{
    struct graph* graph;
    int nb_dest;
    int index_source;
    size_t* destinations;
    size_t* distances;
    size_t* list_prev;
};

//used to store data compute by cost fuction
struct chained_list
{
    size_t cost;
    int index;
    struct chained_list* next;
};
void free_chained_list(struct chained_list* cl)
{
    struct chained_list* tmp;
    while (cl != NULL)
    {
        tmp = cl;
        cl = cl->next;
        free(tmp);
    }
}

//utility function TODO remove in final build
void print_tsp(size_t** dists, int len_dest, size_t* destinations,\
        int is_no_start)
{
    int k;
    printf("                  ");
    for (int i = 0; i < len_dest + is_no_start; i++)
    {
        k = i;
        if (is_no_start && i > 0) {k--;}
        printf("%8lu ", destinations[k]);
    }
    printf("\n\n");
    for (int i = 0; i < len_dest + is_no_start; i++)
    {
        k = i;
        if (is_no_start && i > 0) {k--;}
        printf("%15lu   ", destinations[k]);
        for (int j = 0; j < len_dest + is_no_start; j++)
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
void cost(size_t** dis, int index, int* index_list, int len,\
        struct chained_list* r)
{
    //create return element
    r->cost = ULONG_MAX;
    r->next = NULL;
    r->index = index;
    
    //base case when the set is empty
    if (len == 0)
    {
        r->cost = dis[index][0];
        return;
    }
    struct chained_list* temp;
    size_t temp_min;
    for (int i = 0; i < len; i ++)
    {
        temp = malloc(sizeof(struct chained_list));
        int* cp_list = remove_index(index_list, i, len);

        cost(dis, index_list[i], cp_list, len - 1, temp);
        
        temp_min = temp->cost + dis[index][index_list[i]];
        
        if (temp_min < r->cost)
        {
            free_chained_list(r->next);
            r->cost = temp_min; 
            r->next = temp;
        }
        else
        {
            free_chained_list(temp);
        }
        free(cp_list);
    }
}

void* worker(void* arg)
{
    struct thread_data* d = (struct thread_data* )arg;
    dijkstra(d->graph, d->nb_dest, d->index_source, d->destinations,\
            d->distances, d->list_prev);
    return NULL;
}

struct node* tsp_main(struct graph* g, size_t* destinations, int len_dest,\
        int is_no_start)
{
    void* to_free;
    // + 1 for the dummy point
    //tsp_dists is a distance matrice conaining destinations
    size_t** tsp_dists = malloc((len_dest + is_no_start) * sizeof(size_t*));
    for (int i = 0; i < len_dest +  is_no_start; i ++)
        tsp_dists[i] = malloc((len_dest + is_no_start) * sizeof(size_t));
    
    //paths is a list for all the shortest path between destinations
    size_t** paths = malloc(len_dest * sizeof(size_t*));
    for (int i = 0; i < len_dest; i ++)
        paths[i] = malloc(g->order * sizeof(size_t));
    
    //create the list of threads
    pthread_t thr[len_dest];
    struct thread_data d[len_dest];
    //here i is the index in destinations where we work
    for (int i = is_no_start; i < len_dest + is_no_start; i ++)
    {
        //initialise the data given to the thread
        d[i - is_no_start].graph = g;
        d[i - is_no_start].nb_dest = len_dest;
        d[i - is_no_start].index_source = i - is_no_start;
        d[i - is_no_start].destinations = destinations;
        d[i - is_no_start].distances = malloc(g->order * sizeof(size_t));
        d[i - is_no_start].list_prev = malloc(g->order * sizeof(size_t));

        int er = pthread_create(&thr[i - is_no_start], NULL, worker,\
                (void*)&(d[i - is_no_start]));
        if (er != 0)
            errx(EXIT_FAILURE, "TSP: Failure while creating thread");
    }
    for (int i = is_no_start; i < len_dest + is_no_start; i ++)
    {
        pthread_join(thr[i - is_no_start], NULL);
        memcpy(paths[i - is_no_start], d[i - is_no_start].list_prev,\
                g->order * sizeof(size_t));
        for (int j = is_no_start; j < len_dest + is_no_start; j ++)
        {
            tsp_dists[i][j] = \
                d[i - is_no_start].distances[destinations[j - is_no_start]];
        }
        free(d[i - is_no_start].distances);
        free(d[i - is_no_start].list_prev);
    }


    //dummy point 0 from everything will be the starting point
    if (is_no_start == TRUE)
    {
        for (int i = 0; i < len_dest + 1; i ++)
        {
            tsp_dists[0][i] = 99999999;//1 so it start where I want
            tsp_dists[i][0] = 0;
        }
        tsp_dists[0][1] = 0;
    }
    //data used for the cost function
    struct chained_list* cl = malloc(sizeof(struct chained_list));
    int set[len_dest];
    for (int i = 0; i < len_dest; i ++)
        set[i] = i + 1;

    //computing the cost and the shortest path for the TSP
    //stored in the chained list cl
    cost(tsp_dists, 0, set, len_dest - 1 + is_no_start, cl);
    if (is_no_start == TRUE)
    {
        to_free = cl;
        cl = cl->next;
        free(to_free);
    }
    struct chained_list* tmp_l = cl;

    struct node* final = init_node(ULONG_MAX);
    struct node* tmp_n = final;
    struct node* tmp2_n;
    for (int i = 0; i < len_dest - 1; i ++)
    {
        tmp2_n = build_return(g, destinations[tmp_l->index - is_no_start],\
                destinations[tmp_l->next->index - is_no_start],\
                paths[tmp_l->index - is_no_start]);
        if(tmp_n != final)
        {
            to_free = tmp2_n;
            tmp2_n = tmp2_n->next;
            free(to_free);
        }
        tmp_n->next = tmp2_n;
        tmp_l = tmp_l->next;
        while (tmp_n->next != NULL)
            tmp_n = tmp_n->next;
    }

    print_tsp(tsp_dists, len_dest, destinations, is_no_start);
    
    tmp_l = cl;
    printf("\n%lu\n", tmp_l->cost);
    while (tmp_l != NULL)
    {
        printf("%lu ", destinations[tmp_l->index - 1]);
        tmp_l = tmp_l->next;
    }

    //free all the data used
    free_chained_list(cl);
    for (int i = 0; i < len_dest + is_no_start; i ++)
        free(tsp_dists[i]);
    for (int i = 0; i < len_dest; i ++)
        free(paths[i]);
    free(paths);
    free(tsp_dists);
    return final;
}
