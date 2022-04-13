#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

#include "tsp.h"

//data that will be given to the thread, store the data for Dijkstra function
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
    //free the memory allocate to a chained list
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

int* remove_index(int* list, int index_remove, int len)
{
    /*
    **  copy a list without and element at a specific index
    **  return a pointer to the new list
    */
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

void cost(size_t** dis, int index, int* index_list, int len,\
        struct chained_list* r)
{
    /*
    **  calculate the minimum cost for a path by testing every paths
    **  dis is the matrice of distances of the Dijkstra
    **  
    **  it is a recursive fonction, it will call itself 
    **  removing one destination from the list at each recursion
    **  it will find the minimum between all possible dest
    **
    **  store the path in the chained list r
    */

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
    /*
    **  worker function for the threads 
    **  call Dijkstra with the data stored in arg
    */
    struct thread_data* d = (struct thread_data* )arg;
    dijkstra(d->graph, d->nb_dest, d->index_source, d->destinations,\
            d->distances, d->list_prev);
    return NULL;
}

void comput_matrix(struct graph* g, size_t* destinations, int len_dest,\
        size_t** tsp_dists, size_t** paths, int is_no_start)
{
    //create the list of threads
    pthread_t thr[len_dest];
    struct thread_data d[len_dest];

    //here i is the index in destinations where we work
    //call for Dijkstra on a diferent thread for each i to all other dist
    for (int i = is_no_start; i < len_dest + is_no_start; i ++)
    {
        //initialise the data given to the thread
        d[i - is_no_start].graph = g;
        d[i - is_no_start].nb_dest = len_dest;
        d[i - is_no_start].index_source = i - is_no_start;
        d[i - is_no_start].destinations = destinations;
        d[i - is_no_start].distances = malloc(g->order * sizeof(size_t));
        d[i - is_no_start].list_prev = malloc(g->order * sizeof(size_t));
        //create the thread
        int er = pthread_create(&thr[i - is_no_start], NULL, worker,\
                (void*)&(d[i - is_no_start]));
        if (er != 0)
            errx(EXIT_FAILURE, "TSP: Failure while creating thread");
    }

    //wait for all thread to finish, collect the data then free it
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
}

void build_full_path(struct graph* g, size_t* destinations, size_t** paths,\
        int is_no_start, struct node* final, struct chained_list* cl_tsp,\
        int len_dest)
{
    /*
    **  take cl_tsp, the chained list solving the tsp 
    **  build the full path between each point using build_return
    */
    void* to_free;
    struct node* tmp_n = final;
    struct node* tmp2_n;
    
    for (int i = 0; i < len_dest - 1; i ++)
    {
        //build path between two points
        tmp2_n = build_return(g, destinations[cl_tsp->index - is_no_start],\
                destinations[cl_tsp->next->index - is_no_start],\
                paths[cl_tsp->index - is_no_start]);

        /* if it is not the first, skip the first node as it is the same
        ** as the the last node of the previous path
        */
        if(tmp_n != final)
        {
            to_free = tmp2_n;
            tmp2_n = tmp2_n->next;
            free(to_free);
        }
        //put the path at the end of the final result
        tmp_n->next = tmp2_n;
        cl_tsp = cl_tsp->next;
        //go to the end
        while (tmp_n->next != NULL)
            tmp_n = tmp_n->next;
    }
}
struct node* tsp_main(struct graph* g, size_t* destinations, int len_dest,\
        int is_no_start)
{
    /*
    **  main function for the Traveling Saleman Problem to all distinations
    **  is_no_start is a boolean indicating if the starting point is relevant
    **  return a linked list with the full path
    */

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
    
    comput_matrix(g, destinations, len_dest, tsp_dists, paths, is_no_start);
   
    //dummy point 0 from everything will be the starting point
    if (is_no_start == TRUE)
    {
        for (int i = 0; i < len_dest + 1; i ++)
        {
            //Big value to force the final path start at the starting point
            tsp_dists[0][i] = 99999999;
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


    struct chained_list* cl_tsp = cl;
    struct node* final = init_node(ULONG_MAX);
    build_full_path(g, destinations, paths, is_no_start, final, cl_tsp,\
            len_dest);

    print_tsp(tsp_dists, len_dest, destinations, is_no_start);
    
    cl_tsp = cl;
    printf("\n%lu\n", cl_tsp->cost);
    while (cl_tsp != NULL)
    {
        printf("%lu ", destinations[cl_tsp->index - 1]);
        cl_tsp = cl_tsp->next;
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
