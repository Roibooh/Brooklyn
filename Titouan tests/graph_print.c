#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#include "graph.h"

#define CWALK  "\x1B[31m" // red
#define CMETRO  "\x1B[34m" // blue
#define CBIKE  "\x1B[33m" // yellow
#define CBUS  "\x1B[32m" // green
#define CTRAM  "\x1B[35m" // magenta
#define CNORMAL  "\x1B[0m" // default



void print_graph_into_file(size_t height, size_t width)
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

int is_part(size_t* arr, int length, size_t node)
{
    for(int i = 0; i < length; i++)
    {
        if (*(arr+i) == node)
        {
            return 1;
        }
    }
    return 0;
}

void print_generated_bike_graph(size_t height, size_t width, int q_transport)
{
    FILE *read_bike = fopen("gen_bike.txt", "r");
    if(read_bike == NULL)
        err(1, "Unable to open file!");
    size_t *bikes = malloc(sizeof(size_t) * q_transport);
    char chunk[8];
    size_t chunk_i;
    int counter = 0;
    while(fgets(chunk, sizeof(chunk), read_bike) != NULL && counter<q_transport)
    {
        chunk_i = atoi(chunk);
        *(bikes + counter) = chunk_i;
        counter++;
    }

    size_t y, x;
    char n[15];
    char node[30];
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y){
            int is_bike = is_part(bikes, q_transport, x*width+y);
            if (x*width + y < 10)
                strcpy(node, "( ");
            else
                strcpy(node, "(");
            if (is_bike)
                sprintf(n, "%s%lu%s", CBIKE, x*width + y, CNORMAL);
            else
                sprintf(n, "%lu", x*width + y);
            strcat(node, n);
            if (x*width + y >= 100)
                strcat(node, ")");
            else
                strcat(node, " )");
            if (y != width-1)
                printf("%s---", node);
            else
                printf("%s\n", node);
        }
        if (x != height-1){
            for(y = 0; y < width; ++y){
                if (y != width-1)
                    printf("  |     ");
                else
                    printf("  |\n");
            }
        }
    }
}



int is_part_2(int* arr, size_t node, size_t len)
{
    size_t i = 0;
    while((*(arr+i) != -1) & (i < len))
    {
        if (*(arr+i) == (int)node)
            return 1;
        i++;
    }
    return 0;
}



void print_generated_motorized_graph(size_t height, size_t width, int* metros, int type)
{
    size_t y, x;
    char n[15];
    char node[30];
    char* color;
    size_t noeud;
    switch (type)
    {
        case METRO:
            color = CMETRO;
            break;
        case TRAM:
            color = CTRAM;
            break;
        case BUS:
            color = CBUS;
            break;
        default:
            color = CNORMAL;
            break;
    }
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y){
            noeud = x*width+y;
            int is_motor = is_part_2(metros, noeud, height*width);
            if (noeud%1000 < 10)
                strcpy(node, "( ");
            else
                strcpy(node, "(");
            if (is_motor)
                sprintf(n, "%s%lu%s", color, noeud%1000, CNORMAL);
            else
                sprintf(n, "%lu", noeud%1000);
            strcat(node, n);
            if (noeud%1000 >= 100)
                strcat(node, ")");
            else
                strcat(node, " )");
            if (y != width-1)
                printf("%s---", node);
            else
                printf("%s\n", node);
        }
        if (x != height-1){
            for(y = 0; y < width; ++y){
                if (y != width-1)
                    printf("  |     ");
                else
                    printf("  |\n");
            }
        }
    }
}



//------------------------------------------------------------------------------



void print_list(struct node* path)
{
    printf("0 is Walk\n1 is Bus\n2 is Metro\n3 is Bike\n4 is Tram\n\n");
    struct node* tmp = path;
    printf("%li", tmp->vertex);
    while(tmp->next!=NULL)
    {
        tmp = tmp->next;
        printf(" -(%i)-> %li", tmp->transport, tmp->vertex);
    }
    printf("\n");
    struct node* tmp2 = path;
    printf("\n%li", tmp2->vertex);
    while(tmp2->next!=NULL)
    {
        tmp2 = tmp2->next;
        printf(" ---> %li", tmp2->vertex);
    }
    printf("\n");
}


struct node* complete_btb_path(size_t origin, size_t dest, size_t g_width, 
struct node *result, int transport)
{
    struct node* res = result;
    if(origin < dest)
    {
        while(origin+g_width < dest)
        {
            origin+=g_width;
            struct node* n = init_node(origin);
            n->transport = transport;
            res->next = n;
            res = res->next;
        }
        if(origin%g_width < dest%g_width)
        {
            while(origin < dest)
            {
                origin+=1;
                struct node* n = init_node(origin);
                n->transport = transport;
                res->next = n;
                res = res->next;
            }
        }
        else
        {
            origin+=g_width;
            struct node* n = init_node(origin);
            n->transport = transport;
            res->next = n;
            res = res->next;
            while(origin > dest)
            {
                origin-=1;
                struct node* n = init_node(origin);
                n->transport = transport;
                res->next = n;
                res = res->next;
            }
        }
    }
    else
    {
        while(origin-g_width > dest)
        {
            origin-=g_width;
            struct node* n = init_node(origin);
            n->transport = transport;
            res->next = n;
            res = res->next;
        }
        if(origin%g_width > dest%g_width)
        {
            while(origin > dest)
            {
                origin-=1;
                struct node* n = init_node(origin);
                n->transport = transport;
                res->next = n;
                res = res->next;
            }
        }
        else
        {
            origin-=g_width;
            struct node* n = init_node(origin);
            n->transport = transport;
            res->next = n;
            res = res->next;
            while(origin < dest)
            {
                origin+=1;
                struct node* n = init_node(origin);
                n->transport = transport;
                res->next = n;
                res = res->next;
            }
        }
    }
    return result->next;
}



struct node* complete_path(struct node *path, size_t graph_width)
{
    struct node *tmp = path;    
    struct node *result = init_node(path->vertex);
    struct node *res = result;
    size_t origin;
    while(tmp->next != NULL)
    {
        origin = tmp->vertex;
        tmp = tmp->next;

        switch (tmp->transport)
        {
            case WALK:
            {
                struct node* n = init_node(tmp->vertex);
                n->transport = WALK;
                res->next = n;
                res = res->next;
                break;
            }
            case BIKE:
            {
                res->next = complete_btb_path(origin, tmp->vertex, 
                graph_width, res, BIKE);
                while(res->next != NULL)
                    res = res->next;
                break;
            }
            case TRAM:
            {
                res->next = complete_btb_path(origin, tmp->vertex, 
                graph_width, res, TRAM);
                while(res->next != NULL)
                    res = res->next;
                break;
            }
            case BUS:
            {
                res->next = complete_btb_path(origin, tmp->vertex, 
                graph_width, res, BUS);
                while(res->next != NULL)
                    res = res->next;
                break;
            }
            case METRO:
            {
                struct node* n = init_node(tmp->vertex);
                n->transport = METRO;
                res->next = n;
                res = res->next;
                break;
            }
            default:
                break;
        }
    }
    return result;
}



void find_neighbor_list(struct node *path, size_t node, int grid_width, int *n1_tr, int *n2_tr, int* metro)
{
    /////////////////////////////////////////////////pas oublier de tester plusieurs neouds car chemin peut revenir 2 fois sur un chemin
    struct node *pre = NULL;
    struct node *tmp = path;
    while (tmp != NULL && tmp->vertex != node)
    {
        pre = tmp;
        tmp = tmp->next;
    }
    if (pre != NULL)
    {
        if (pre->vertex == node + 1 && tmp != NULL){
            *n1_tr = tmp->transport;
        }
        if (pre->vertex == node + grid_width && tmp != NULL){
            *n2_tr = tmp->transport;
        }
        if (pre->next != NULL && pre->next->transport == METRO){
            *metro = 1;
        }
    }
    if (tmp != NULL && tmp->next != NULL)
    {
        if (tmp->next->vertex == node + 1){
            *n1_tr = tmp->next->transport;
        }
        if (tmp->next->vertex == node + grid_width){
            *n2_tr = tmp->next->transport;
        }
        if (tmp->next->transport == METRO || tmp->transport == METRO){
            *metro = 1;
        }
    }
}



void symbol(char *s, int size, int* metro)
{
    char i[5];
    sprintf(i, "%i", size);
    if (size < 10)
    {
        if (*metro){
            strcpy(s, CMETRO);
            strcat(s, "( ");
        }
        else
            strcpy(s, "( ");
        strcat(s,i);
        strcat(s, " )");
        if (*metro)
            strcat(s, CNORMAL);
    }
    else if (size < 100)
    {
        if (*metro){
            strcpy(s, CMETRO);
            strcat(s, "(");
        }
        else
            strcpy(s, "(");
        strcat(s,i);
        strcat(s, " )");
        if (*metro)
            strcat(s, CNORMAL);
    }
    else
    {
        if (*metro){
            strcpy(s, CMETRO);
            strcat(s, "(");
        }
        else
            strcpy(s, "(");
        strcat(s,i);
        strcat(s, ")");
        if (*metro)
            strcat(s, CNORMAL);
    } 
}



void print_path_terminal(struct node *path_incomplete, int height, int width)
{
    struct node* path = complete_path(path_incomplete, width);
    print_list(path);
    printf("\n\n");
    int y, x;
    char s[20];
    int *n1_tr = calloc(sizeof(int), 1);
    int *n2_tr = calloc(sizeof(int), 1);
    int *is_metro = calloc(sizeof(int), 1);
    for(x = 0; x < height; ++x)
    {
        for(y = 0; y < width; ++y)
        {
            *is_metro = 0;
            *n1_tr = UNDEFINED;
            *n2_tr = UNDEFINED;
            find_neighbor_list(path, x*width + y, width, n1_tr, n2_tr, is_metro);
            symbol(s, x*width + y, is_metro);
            if (y != width-1)
            {
                switch (*n1_tr)
                {
                case WALK:
                    printf("%s%s---%s", s, CWALK, CNORMAL);
                    break;
                case TRAM:
                    printf("%s%s---%s", s, CTRAM, CNORMAL);
                    break;
                case BIKE:
                    printf("%s%s---%s", s, CBIKE, CNORMAL);
                    break;
                case BUS:
                    printf("%s%s---%s", s, CBUS, CNORMAL);
                    break;
                default:
                    printf("%s---", s);
                }
            }
            else
                printf("%s\n", s);
        }
        if (x != height-1)
        {
            for(y = 0; y < width; ++y)
            {
                *n1_tr = UNDEFINED;
                *n2_tr = UNDEFINED;
                *is_metro = 0;
                find_neighbor_list(path, x*width + y, width, n1_tr, n2_tr, is_metro);
                if (y != width-1)
                {
                    switch (*n2_tr)
                    {
                        case WALK:
                            printf("%s  |     %s", CWALK, CNORMAL);
                            break;
                        case TRAM:
                            printf("%s  |     %s", CTRAM, CNORMAL);
                            break;
                        case BIKE:
                            printf("%s  |     %s", CBIKE, CNORMAL);
                            break;
                        case BUS:
                            printf("%s  |     %s", CBUS, CNORMAL);
                            break;                    
                        default:
                            printf("  |     ");
                    }
                }
                else
                {
                    switch (*n2_tr)
                    {
                        case WALK:
                            printf("%s  |\n %s", CWALK, CNORMAL);
                            break;
                        case TRAM:
                            printf("%s  |\n%s", CTRAM, CNORMAL);
                            break;
                        case BIKE:
                            printf("%s  |\n%s", CBIKE, CNORMAL);
                            break;
                        case BUS:
                            printf("%s  |\n%s", CBUS, CNORMAL);
                            break;                    
                        default:
                            printf("  |\n");
                    }
                }
                
            }
        }
    }
    free(n1_tr);
    free(n2_tr);
    free(is_metro);
}