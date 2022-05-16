#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#include "ant.h"
#include "graph.h"
#include "macro.h"

const float aalpha = 3.0;
const float bbeta = 5.0;
const float evaporate_rate = 0.999;

const float constant_pheromone = 0.01;
const float base_ratio = 0.05;

const size_t c_size = 20;
const int iterations = 100;

struct possible_list
{
    size_t index;
    struct possible_list* next;
};

void free_possible_list(struct possible_list* pl)
{
    struct possible_list* tmp;
    while (pl != NULL)
    {
        tmp = pl;
        pl = pl->next;
        free(tmp);
    }
}

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


//return a random float betwen 0 and a
float make_rand(float max)
{
    srand((unsigned int)time(NULL));

    double tmp = (double)max;
    double r = ((double)rand()/(double)(RAND_MAX)) * tmp;
    return (float)r;
}

float** duplicate_phero(float** phero, size_t s)
{
    float** new = malloc(s * sizeof(float*));
    for (size_t i = 0; i < s; i++)
    {
        new[i] = malloc(s * sizeof(float));
        for (size_t j = 0; j < s; j ++)
            new[i][j] = phero[i][j];
    }
    return new;
}

float get_proba(size_t i, size_t j, float** pheromone, size_t** matrix)
{
    return pow(pheromone[i][j], aalpha) * pow(matrix[i][j], -bbeta);
}

int contain(struct node* path, size_t i)
{
    while (path != NULL)
    {
        if (path->vertex == i)
            return TRUE;
        path = path->next;
    }
    return FALSE;
}


size_t make_ant_path(size_t index, struct node* path, size_t** matrix,\
        float** pheromone, size_t g_size)
{
    size_t len = 0;
    size_t nb_parcouru = 0;
    float sum_proba;
    struct node* path_header = path;
    
    //list of possible nodes for next step
    //here possible is like a sentinel, the first value is useless
    struct possible_list* possible = calloc(1, sizeof(struct possible_list));
    while (nb_parcouru < g_size - 1) // - 1 because start is already inside
    {
        struct possible_list* tmp_possible = possible;
        sum_proba = 0.0;
        for (size_t j = 0; j < g_size; j++)
        {
            if (contain(path_header, j))
                continue;
            //if not already accessed j is a possible next step
            sum_proba += get_proba(index, j, pheromone, matrix);
            struct possible_list* new_list_elt =\
                    calloc(1, sizeof(struct possible_list));
            new_list_elt->index = j;
            tmp_possible->next = new_list_elt;
            tmp_possible = tmp_possible->next;
        }
        float rand = make_rand(sum_proba);
        float tmp_sum_proba = 0.0;

        tmp_possible = possible->next;
        while (tmp_possible != NULL)
        {
            tmp_sum_proba += get_proba(index, tmp_possible->index,\
                    pheromone, matrix);
            if (rand <= tmp_sum_proba)
            {
                len += matrix[index][tmp_possible->index];
                index = tmp_possible->index;
                
                struct node* new_elt_path = init_node(tmp_possible->index);
                path->next = new_elt_path;
                path = path->next;

                nb_parcouru += 1;
                break;
            }
            tmp_possible = tmp_possible->next;
        }
        free_possible_list(possible->next);
        possible->next = NULL;
    }
    free_possible_list(possible);
    return len;
}

void run_colony(size_t g_size, size_t** matrix, float** pheromone,\
        size_t start)
{
    size_t i;
    size_t j;
 
    size_t best_len = ULONG_MAX;

    //evaporation of the precedent pheromon
    for (i = 0; i < g_size; i ++)
        for (j = 0; j < g_size; j ++)
            pheromone[i][j] *= evaporate_rate;

    float** new_pheromone = duplicate_phero(pheromone, g_size);
    //run each ant
    for (i = 0; i < c_size; i ++)
    {
        struct node* path = init_node(start);
        struct node* path_header = path;
        
        size_t len = make_ant_path(start, path, matrix, pheromone, g_size);
        
        if (len < best_len)
            best_len = len;
        size_t diff_to_best = len - best_len;
        
        float ratio = (float)diff_to_best + base_ratio;
        float pheromone_to_add = constant_pheromone / ratio;

        size_t pre = path->vertex;
        //size_t first = path->vertex;
        path = path->next;

        //simulate phero trace
        while (path->next != NULL)
        {
            //new_pheromone[path->vertex][pre] += pheromone_to_add;
            new_pheromone[pre][path->vertex] += pheromone_to_add;

            pre = path->vertex;
            path = path->next;
        }
        //new_pheromone[path->vertex][pre] += pheromone_to_add;
        new_pheromone[pre][path->vertex] += pheromone_to_add;

        //new_pheromone[path->vertex][first] += pheromone_to_add;
        //new_pheromone[first][path->vertex] += pheromone_to_add;

        free_node(path_header);
    }

    
    //update weight of phero after
    for (i = 0; i < g_size; i++)
    {
        float sum = 0.0;
        for (j = 0; j <  g_size; j++)
        {
            if (i == j)
                continue;
            sum += new_pheromone[i][j];
        }
        for (j = 0; j < g_size; j++)
            pheromone[i][j] = 2 * new_pheromone[i][j] / sum;
    }
    for (size_t i = 0; i < g_size; i++)
        free(new_pheromone[i]);
    free(new_pheromone);
}

size_t get_max(float* line, size_t g_size)
{
    float max = line[0];
    size_t maxi = 0;

    for(size_t i = 0; i < g_size; i++)
    {
        if (line[i] > max)
        {
            maxi = i;
            max = line[i];
        }
    }
    return maxi;
}

struct chained_list* main_ant(size_t g_size, size_t** matrix, size_t start)
{
    float** pheromone = calloc(g_size, sizeof(float*));
    for (size_t i = 0; i < g_size; i ++)
    {
        pheromone[i] = calloc(g_size, sizeof(float));
        for (size_t j = 0; j < g_size; j++)
            if (i != j)
                pheromone[i][j] = 1.0;
    }
    for (int i = 0; i < iterations; i++)
        run_colony(g_size, matrix, pheromone, start);

    struct chained_list* r = malloc(sizeof(struct chained_list));
    r->cost = 0;
    r->index = start;
    r->next = NULL;

    struct chained_list* tmp = r;
    size_t nb_done = 1;
    size_t cost = 0;

    while (nb_done < g_size)
    {
        struct chained_list* new = malloc(sizeof(struct chained_list));
        new->cost = 0;
        new->index = get_max(pheromone[tmp->index], g_size);
        new->next = NULL;
        cost += matrix[tmp->index][new->index];
        tmp->next = new;
        tmp = tmp->next;
        nb_done += 1;
    }

    for (size_t i = 0; i < g_size; i++)
        free(pheromone[i]);
    free(pheromone);
    
    r->cost = cost;
    return r;

 /*   printf("\n_______\n");
    for (size_t i = 0; i < g_size; i++)
    {
        for (size_t j = 0; j < g_size; j++)
        {
            printf("%f ", pheromone[i][j]);
        }
        printf("\n");
    }
   printf("_______\n");
    
    for (size_t i = 0; i < g_size; i++)
    {
        for (size_t j = 0; j < g_size; j++)
        {
            printf("%lu ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("_______\n"); 
*/
  
}
