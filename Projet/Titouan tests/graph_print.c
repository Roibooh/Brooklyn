#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

#define WALK  "\x1B[31m"
#define NORMAL  "\x1B[0m"


void print_graph(size_t height, size_t width)
{
    size_t y, x;
    char n[5];
    char node[10];
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y)
        {
            if (x*width + y < 10)
            {
                strcpy(node, "( ");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, " )");
            }
            else if (x*width + y < 100)
            {
                strcpy(node, "(");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, " )");
            }
            else
            {
                strcpy(node, "(");
                sprintf(n, "%lu", x*width + y);
                strcat(node, n);
                strcat(node, ")");
            }

            if (y != width-1)
                printf("%s---", node);
            else
                printf("%s\n", node);
        }
        if (x != height-1)
        {
            for(y = 0; y < width; ++y)
            {
                if (y != width-1)
                    printf("  |     ");
                else
                    printf("  |\n");
            }
        }
    }
}



//------------------------------------------------------------------------------



void complete_path(struct node *path, size_t graph_size)
{
    struct node *result = init_node(path->vertex);
    struct graph *g = load_main("main.txt");

    //call dijkstra everytime the transport is not walking or metro
    //then complete de result list 
}


void find_neighbor_arr(int *path, int length, int node, int grid_width, int *neighbor_1, int *neighbor_2)
{
    int counter = 0;
    while (path[counter] != node && counter < length)
    {
        counter++;
    }
    if (counter != 0)
    {
        if (path[counter-1] == node + 1)
            *neighbor_1 = 1;
        if (path[counter-1] == node + grid_width)
            *neighbor_2 = 1;
    }
    if (counter != length-1)
    {
        if (path[counter+1] == node + 1)
            *neighbor_1 = 1;
        if (path[counter+1] == node + grid_width)
            *neighbor_2 = 1;
    }
    //a refaire avec les liste chainés
}


void find_neighbor_list(struct node *path, size_t node, int grid_width, int *neighbor_1, int *neighbor_2)
{
    struct node *pre = NULL;
    struct node *tmp = path;
    while (tmp->next->vertex != node && tmp->next != NULL)
    {
        tmp = tmp->next;
        pre = tmp;
    }
    if (pre != NULL)
    {
        if (pre->vertex == node + 1)
            *neighbor_1 = 1;
        if (pre->vertex == node + grid_width)
            *neighbor_2 = 1;
    }
    if (tmp->next != NULL)
    {
        if (tmp->next->vertex == node + 1)
            *neighbor_1 = 1;
        if (tmp->next->vertex == node + grid_width)
            *neighbor_2 = 1;
    }
}


void print_path(int *path, int path_length, int height, int width)
{
    int y, x;
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y)
        {
            int *neighbor_1 = calloc(sizeof(int), 1);
            int *neighbor_2 = calloc(sizeof(int), 1);
            find_neighbor_list(path, x*width + y, width, neighbor_1, neighbor_2);
            if (x*width + y < 10)
            {
                if (y != width-1)
                    if (*neighbor_1)
                        printf("( %u )%s---%s", x*width + y, WALK, NORMAL);
                    else
                        printf("( %u )---", x*width + y);
                else
                    printf("( %u )\n", x*width + y);
            }
            else if (x*width + y < 100)
            {
                if (y != width-1)
                    if (*neighbor_1)
                        printf("(%u )%s---%s", x*width + y, WALK, NORMAL);
                    else
                        printf("(%u )---", x*width + y);
                else
                    printf("(%u )\n", x*width + y);
            }
            else
            {
                if (y != width-1)
                    if (*neighbor_1)
                        printf("(%u)%s---%s", x*width + y, WALK, NORMAL);
                    else
                        printf("(%u)---", x*width + y);
                else
                    printf("(%u)\n", x*width + y);
            }
        }
        if (x != height-1)
        {
            for(y = 0; y < width; ++y)
            {
                int * neighbor_1 = calloc(sizeof(int), 1);
                int * neighbor_2 = calloc(sizeof(int), 1);
                find_neighbor_list(path, x*width + y, width, neighbor_1, neighbor_2);
                if (*neighbor_2)
                {
                    if (y != width-1)
                        printf("%s  |     %s", WALK, NORMAL);
                    else
                        printf("%s  |\n%s", WALK, NORMAL);
                }
                else
                {
                    if (y != width-1)
                        printf("  |     ");
                    else
                        printf("  |\n");
                }
            }
        }
    }
}


int main()
{
    printf("no prob\n");
    //print_path (path, 10, 4, 8);
}