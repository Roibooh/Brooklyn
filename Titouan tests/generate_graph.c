#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "link_bikes.h"
#include "graph_print.h"
#include <string.h>
#include <time.h>
#include <math.h>


#define BIKE_WEIGHT 30
#define METRO_WEIGHT 6
#define TRAM_WEIGHT 18
#define BUS_WEIGHT 24
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


int transport_weight(size_t w, int start, int end, int transport)
{
    int width = (int)w;
    int dist = 0;
    if (start < end){
        while(start+width < end){
            dist++;
            start+=width;
        }
        if (abs(end-start) < width/2){
            while(start < end){
                dist++;
                start++;
            }
        }
        else{
            dist++;
            start+=width;
            while(start > end){ ///pas sur si +1
                dist++;
                start--;
            }
        }
        
    }
    if (start > end){
        while(start-width > end){
            dist++;
            start-=width;
        }
        if (abs(end-start) < width/2){
            while(start > end){
                dist++;
                start--;
            }
        }
        else{
            dist++;
            start-=width;
            while (start < end){ //+1?
                dist++;
                start++;
            }
        }
    }
    return dist*transport;
}


int next_node_height(size_t height, int start_height, int dist, int change)
{
    int neg = rand()%2;
    int add = rand()%change;
    int next = 0;
    if (neg == 0)
        next = start_height+add+dist;
    else
        next = start_height-add+dist;
    
    if (next < 0)
        return 0;
    if (next >= (int)height)
        return height-1;
    return next;
}


int next_node_width(size_t width, int start_width, int dist, int change)
{
    int neg = rand()%2;
    int add = rand()%change;
    int next = 0;
    if (neg == 0)
        next = start_width+add+dist;
    else
        next = start_width-add+dist;
    if (next < 0)
        return 0;
    if (next >= (int)width)
        return width-1;
    return next;
}



void create_walk_graph(size_t height, size_t width, int walking)
{
    srand(time(0));
    FILE *file = fopen("gen_main.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }

    char h[4];
    char w[4];
    char m[4];
    char walk[4];

    size_t max = height*width;
    sprintf(m, "%lu", max);
    fputs(m, file);
    fputs("\n", file);

    sprintf(walk, "%i", walking);

    for(size_t h_counter = 0; h_counter < height-1; ++h_counter)
    {
        for(size_t w_counter = 0; w_counter < width-1; ++w_counter)
        {
            sprintf(h, "%lu", h_counter*width+w_counter);
            fputs(h, file);
            fputs(" ", file);
            sprintf(w, "%lu", h_counter*width+w_counter+1);
            fputs(w, file);
            fputs(" ", file);
            fputs(walk, file);
            fputs("\n", file);

            sprintf(h, "%lu", h_counter*width+w_counter);
            fputs(h, file);
            fputs(" ", file);
            sprintf(w, "%lu", (h_counter+1)*width+w_counter);
            fputs(w, file);
            fputs(" ", file);
            fputs(walk, file);
            fputs("\n", file);
        }
    }

    for(size_t w_counter = 0; w_counter < width-1; ++w_counter)
    {
        sprintf(h, "%lu", (height-1)*width+w_counter);
        fputs(h, file);
        fputs(" ", file);
        sprintf(w, "%lu", (height-1)*width+w_counter+1);
        fputs(w, file);
        fputs(" ", file);
        fputs(walk, file);
        fputs("\n", file);
    }

    for(size_t h_counter = 0; h_counter < height-1; ++h_counter)
    {

        sprintf(h, "%lu", h_counter*width+(width-1));
        fputs(h, file);
        fputs(" ", file);
        sprintf(w, "%lu", (h_counter+1)*width+(width-1));
        fputs(w, file);
        fputs(" ", file);
        fputs(walk, file);
        if (h_counter != height-2)
            fputs("\n", file);
    }

    fclose(file);
}



int generate_bike(size_t height, size_t width)
{
    FILE *file = fopen("gen_bike.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    char h[4];

    for(size_t x = 0; x < height; ++x)
    {
        for(size_t y = 0; y < width; ++y)
        {
            if ((x%4 == 0) & (y%4 == 0))
            {
                size_t coord_x = x + rand()%3;
                if (coord_x < height)
                {
                    size_t coord_y = y + rand()%3;
                    if (coord_y < width)
                    {
                        sprintf(h, "%lu", coord_x*width+coord_y);
                        fputs(h, file);
                        fputs("\n", file);
                        counter++;
                    }
                } 
            }
        }
    }
    fclose(file);
    link_bikes("gen_bike.txt", "gen_bikes.txt", (int)height, (int)width, counter, BIKE_WEIGHT);
    return counter;
}


//-------------------------------------------------------------------------


void regroup(int width, int* nodes, int* next_height, int* next_width, int gap)
{
    int y = 0, n;
    while (*(nodes+y) != 0)
    {
        //printf("node %i\n", *(nodes+y));
        n = *(nodes+y);
        int n_w = 0;
        int n_h = 0;
        while(n - width > 0)
        {
            n_h++;
            n-=width;
        }
        while(n-1 > -1)
        {
            n_w++;
            n--;
        }
        //printf("n_h = %i, n_w = %i\n", n_h, n_w);
        if ((*next_height - n_h < gap) & (*next_height - n_h > -gap))
        {
            if ((*next_width - n_w < gap) & (*next_width - n_w > -gap))
            {  
                //printf("condition met in :----------------\n");
                //printf("next_h = %i, next_w = %i\n", *next_height, *next_width);
                *next_height = n_h;
                *next_width = n_w;
                //printf("changed to next_h = %i, next_w = %i\n", *next_height, *next_width);
                break;
            }
        }
        y++;
    }
}



int *generate_metro(size_t height, size_t width)
{
    srand(time(0));
    FILE *file = fopen("gen_metro.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    int vertical_ways = rand()%(height/10);
    if (vertical_ways == 0)
        vertical_ways = rand()%(height/10);
    int horizontal_ways = rand()%(width/10);
    if (horizontal_ways == 0)
        horizontal_ways = rand()%(width/10);
    int arr_len = vertical_ways*horizontal_ways*(height/7)*(width/7);
    int *nodes = calloc(sizeof(int), arr_len);
    for (int x = 0; x < arr_len; x++)
    {
        *(nodes+x) = -1;
    }

    int start_width = 0, start_height = 0;
    int mem_w = 0, mem_h = 0;
    int weight, neg, i;
    char w[4], node_i[4], node_j[4];
    for(i = 0; i < vertical_ways; i++)
    {
        neg = rand()%2;
        if (neg)
            start_height = ((i+1)*height/(vertical_ways+1))+rand()%3;
        else
            start_height = ((i+1)*height/(vertical_ways+1))-rand()%3;
        start_width = rand()%5;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_width+5 < (int)width)
        {
            mem_h = next_node_height(height, start_height, 0, 3);
            mem_w = next_node_width(width, start_width, 7, 3);
            weight = transport_weight(width, start_height*width+start_width, mem_h*width+mem_w, METRO_WEIGHT);
            sprintf(node_i, "%lu", start_height*width+start_width);
            fputs(node_i, file);
            fputs(" ", file);
            sprintf(node_j, "%lu", mem_h*width+mem_w);
            fputs(node_j, file);
            fputs(" ", file);
            sprintf(w, "%i", weight);
            fputs(w, file);
            fputs("\n", file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    for(i = 0; i < horizontal_ways; ++i)
    {
        neg = rand()%2;
        if (neg)
            start_width = ((i+1)*width/(horizontal_ways+1))+rand()%3;
        else
            start_width = ((i+1)*width/(horizontal_ways+1))-rand()%3;
        start_height = rand()%6;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_height+5 < (int)height)
        {
            mem_h = next_node_height(height, start_height, 7, 3);
            mem_w = next_node_width(width, start_width, 0, 3);
            regroup((int)width, nodes, &mem_h, &mem_w, 3);
            weight = transport_weight(width, start_height*width+start_width, mem_h*width+mem_w, METRO_WEIGHT);

            sprintf(node_i, "%lu", start_height*width+start_width);
            fputs(node_i, file);
            fputs(" ", file);
            sprintf(node_j, "%lu", mem_h*width+mem_w);
            fputs(node_j, file);
            fputs(" ", file);
            sprintf(w, "%i", weight);
            fputs(w, file);
            fputs("\n", file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    fclose(file);
    return nodes;
}


//-------------------------------------------------------------------------


int *generate_tram(size_t height, size_t width)
{
    srand(time(0));
    FILE *file = fopen("gen_tram.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    int vertical_ways = rand()%(height/10);
    if (vertical_ways == 0)
        vertical_ways = rand()%(height/10);
    int horizontal_ways = rand()%(width/10);
    if (horizontal_ways == 0)
        horizontal_ways = rand()%(width/10);
    int arr_len = vertical_ways*horizontal_ways*(height/5)*(width/5);
    int *nodes = calloc(sizeof(int), arr_len);
    for (int x = 0; x < arr_len; x++)
    {
        *(nodes+x) = -1;
    }

    int start_width = 0, start_height = 0;
    int mem_w = 0, mem_h = 0;
    int weight, neg, i, random;
    char w[4], node_i[4], node_j[4];
    for(i = 0; i < vertical_ways; i++)
    {
        neg = rand()%2;
        if (neg)
            start_height = ((i+1)*height/(vertical_ways+1))+rand()%3;
        else
            start_height = ((i+1)*height/(vertical_ways+1))-rand()%3;
        start_width = rand()%5;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_width+5 < (int)width)
        {
            random = rand()%2;
            mem_h = next_node_height(height, start_height, 0, 2);
            mem_w = next_node_width(width, start_width, 4+random, 2);
            weight = transport_weight(width, start_height*width+start_width, mem_h*width+mem_w, TRAM_WEIGHT);
            sprintf(node_i, "%lu", start_height*width+start_width);
            fputs(node_i, file);
            fputs(" ", file);
            sprintf(node_j, "%lu", mem_h*width+mem_w);
            fputs(node_j, file);
            fputs(" ", file);
            sprintf(w, "%i", weight);
            fputs(w, file);
            fputs("\n", file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    for(i = 0; i < horizontal_ways; ++i)
    {
        neg = rand()%2;
        if (neg)
            start_width = ((i+1)*width/(horizontal_ways+1))+rand()%3;
        else
            start_width = ((i+1)*width/(horizontal_ways+1))-rand()%3;
        start_height = rand()%6;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_height+5 < (int)height)
        {
            random = rand()%2;
            mem_h = next_node_height(height, start_height, 4+random, 2);
            mem_w = next_node_width(width, start_width, 0, 2);
            regroup((int)width, nodes, &mem_h, &mem_w, 2);
            weight = transport_weight(width, start_height*width+start_width, mem_h*width+mem_w, TRAM_WEIGHT);

            sprintf(node_i, "%lu", start_height*width+start_width);
            fputs(node_i, file);
            fputs(" ", file);
            sprintf(node_j, "%lu", mem_h*width+mem_w);
            fputs(node_j, file);
            fputs(" ", file);
            sprintf(w, "%i", weight);
            fputs(w, file);
            fputs("\n", file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    fclose(file);
    return nodes;
}


//-------------------------------------------------------------------------


void next_bus_stop(size_t height, size_t width, int direction, size_t *stop_h, size_t *stop_w)
{
    int sign = rand()%2;
    switch (direction)
    {
    case NORTH:
        *stop_h -= 4 + (rand()%3)-1;
        if (sign == 0){
            if (*stop_w > 0)
                *stop_w -= rand()%2;
        }
        else{
            if (*stop_w < width-1)
                *stop_w += rand()%2;
        }
        break;
    case EAST:
        *stop_w += 4 + (rand()%3)-1;
        if (sign == 0){
            if (*stop_h > 0)
                *stop_h -= rand()%2;
        }
        else{
            if (*stop_h < height-1)
                *stop_h += rand()%2;
        }
        break;
    case SOUTH:
        *stop_h += 4 + (rand()%3)-1;
        if (sign == 0){
            if (*stop_w > 0)
                *stop_w -= rand()%2;
        }
        else{
            if (*stop_w < width-1)
                *stop_w += rand()%2;
        }
        break;
    case WEST:
        *stop_w -= 4 + (rand()%3)-1;
        if (sign == 0){
            if (*stop_h > 0)
                *stop_h -= rand()%2;
        }
        else{
            if (*stop_h < height-1)
                *stop_h += rand()%2;
        }
        break;
    default:
        break;
    }
}



void build_bus_line(size_t height, size_t width, size_t start_h, size_t start_w, int direction, FILE *file, int* buses, int *counter)
{
    size_t next_h = start_h;
    size_t next_w = start_w;
    size_t mem_h;
    size_t mem_w;
    int weight;
    char wght[4], node_i[4], node_j[4];
    int n = 0; int e = 0; int s = 0; int w = 0; int full = 0; int done = 0;
    switch (direction)
    {
    case NORTH:
        next_h -= 4 + rand()%2;
        n = 1;
        break;
    case EAST:
        next_w += 4 + rand()%2;
        e = 1;
        break;
    case SOUTH:
        next_h += 4 + rand()%2;
        s = 1;
        break;
    case WEST:
        next_w -= 4 + rand()%2;
        w = 1;
        break;
    default:
        break;
    }
    weight = transport_weight(width, (int)start_h*width+start_w, (int)next_h*width+next_w, BUS_WEIGHT);
    //printf("weight between %li and %li in direction %i = %i\n", start_h*width+start_w, next_h*width+next_w, direction, weight);
    sprintf(node_i, "%lu", start_h*width+start_w);
    fputs(node_i, file);
    fputs(" ", file);
    sprintf(node_j, "%lu", next_h*width+next_w);
    fputs(node_j, file);
    fputs(" ", file);
    sprintf(wght, "%i", weight);
    fputs(wght, file);
    fputs("\n", file);
    *(buses+(*counter)) = (int)(next_h*width+next_w);
    (*counter)++;
    //printf("nexth = %lu and nextw = %lu --- starth = %lu and startw = %lu\n", next_h, next_w, start_h, start_w);
    while ((next_h != start_h) || (next_w != start_w))
    {
        //printf("--------------------\n");
        if (done)
        {
            //printf("done -> last part\n");
            //printf("out of while from src_h: %lu, src_w: %lu and dst_h: %lu, dst_h: %lu\n", next_h, next_w, start_h, start_w);
            int difference_h = start_h - next_h;
            int difference_w = start_w - next_w;
            int i;
            if (abs(difference_h) > abs(difference_w))
                i = abs(difference_h)/3;
            else
                i = abs(difference_w)/3;
            int j = i;
            while(j > 0)
            {

                mem_h = next_h;
                mem_w = next_w;
                next_h += difference_h/i;
                next_w += difference_w/i;
                weight = transport_weight(width, (int)mem_h*width+mem_w, (int)next_h*width+next_w, BUS_WEIGHT);
                sprintf(node_i, "%li", mem_h*width+mem_w);
                fputs(node_i, file);
                fputs(" ", file);
                sprintf(node_j, "%li", next_h*width+next_w);
                fputs(node_j, file);
                fputs(" ", file);
                sprintf(wght, "%i", weight);
                fputs(wght, file);
                fputs("\n", file);
                j--;
                *(buses+(*counter)) = (int)(next_h*width+next_w);
                (*counter)++;
                //printf("in the from src_h: %li, src_w: %li and dst_h: %li, dst_h: %li\n", next_h, next_w, start_h, start_w);
            }
            if (i != 1)
            {
                weight = transport_weight(width, (int)next_h*width+next_w, (int)start_h*width+start_w, BUS_WEIGHT);
                sprintf(node_i, "%lu", next_h*width+next_w);
                fputs(node_i, file);
                fputs(" ", file);
                sprintf(node_j, "%lu", start_h*width+start_w);
                fputs(node_j, file);
                fputs(" ", file);
                sprintf(wght, "%i", weight);
                fputs(wght, file);
                fputs("\n", file);
                next_h = start_h;
                next_w = start_w;
                *(buses+(*counter)) = (int)(next_h*width+next_w);
                (*counter)++;
            }
        }
        else
        {
            //----------------------------------------------------------not yet visited all directions-----------------------
            mem_h = next_h;
            mem_w = next_w;
            /////////////////////////////////////once full, still needs to move forward a little bit
            /////////////////////////////////////cause now it moves forward 3 directions then full = 1 and we end the path
            if (!full)
            {
                int change = 0;
                //printf("not yet visited all directions\n");
                //fputs("- ", file);
                switch (direction)
                {
                case NORTH:
                    change = 0;
                    if (next_h > 5)
                    {
                        if (rand()%(next_h/5) == 0)
                            change = 1;
                    }
                    else
                        change = 1;

                    if (change)
                    {    
                        if (e == 1) {
                            if (w == 1)
                                full = 1;
                            else {
                                if (next_w < 5)
                                    full = 1;
                                else {
                                    direction = WEST;
                                    w = 1;
                                }
                            }
                        }
                        else 
                        {
                            if (w==1){
                                if (next_w > width-5){
                                    direction = EAST;
                                    e = 1;
                                }
                                else
                                    full = 1;
                            }
                            else{
                                direction = (rand()%2)*2 +1;
                                if (direction == EAST){
                                    if (next_w > width-5){
                                        direction = WEST;
                                        w = 1;
                                    }
                                }
                                else{
                                    if (next_w < 5){
                                        direction = EAST;
                                        e = 1;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case EAST:
                    change = 0;
                    if (next_w < width-5)
                    {
                        if (rand()%(width-next_w/5) == 0)
                            change = 1;
                    }
                    else
                        change = 1;

                    if (change)
                    {    
                        if (s == 1) {
                            if (n == 1)
                                full = 1;
                            else {
                                if (next_h < 5)
                                    full = 1;
                                else {
                                    direction = NORTH;
                                    n = 1;
                                }
                            }
                        }
                        else 
                        {
                            if (n==1){
                                if (next_h > height-5){
                                    direction = SOUTH;
                                    s = 1;
                                }
                                else
                                    full = 1;
                            }
                            else{
                                direction = (rand()%2)*2;
                                if (direction == SOUTH){
                                    if (next_h > height-5){
                                        direction = NORTH;
                                        n = 1;
                                    }
                                }
                                else{
                                    if (next_h < 5){
                                        direction = SOUTH;
                                        s = 1;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case SOUTH:
                    change = 0;
                    if (next_h < height-5)
                    {
                        if (rand()%(height-next_h/5) == 0)
                            change = 1;
                    }
                    else
                        change = 1;

                    if (change)
                    {    
                        if (e == 1) {
                            if (w == 1)
                                full = 1;
                            else {
                                if (next_w < 5)
                                    full = 1;
                                else {
                                    direction = WEST;
                                    w = 1;
                                }
                            }
                        }
                        else 
                        {
                            if (w==1){
                                if (next_w > width-5){
                                    direction = EAST;
                                    e = 1;
                                }
                                else
                                    full = 1;
                            }
                            else{
                                direction = (rand()%2)*2 +1;
                                if (direction == EAST){
                                    if (next_w > width-5){
                                        direction = WEST;
                                        w = 1;
                                    }
                                }
                                else{
                                    if (next_w < 5){
                                        direction = EAST;
                                        e = 1;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case WEST:
                    change = 0;
                    if (next_w > 5)
                    {
                        if (rand()%(next_w/5) == 0)
                            change = 1;
                    }
                    else
                        change = 1;

                    if (change)
                    {    
                        if (s == 1) {
                            if (n == 1)
                                full = 1;
                            else {
                                if (next_h < 5)
                                    full = 1;
                                else {
                                    direction = NORTH;
                                    n = 1;
                                }
                            }
                        }
                        else 
                        {
                            if (n==1){
                                if (next_h > height-5){
                                    direction = SOUTH;
                                    s = 1;
                                }
                                else
                                    full = 1;
                            }
                            else{
                                direction = (rand()%2)*2;/////////////////////////////////////////////
                                if (direction == SOUTH){/////////////////add if(s ==1) if wanna boucle
                                    if (next_h > height-5){///////////////////////////////////////////
                                        direction = NORTH;
                                        n = 1;
                                    }
                                }
                                else{
                                    if (next_h < 5){
                                        direction = SOUTH;
                                        s = 1;
                                    }
                                }
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
                if (!full){
                    //printf("n = %i, e = %i, s = %i, w = %i\n", n, e, s, w);
                    //printf("direction is: %i\n", direction);
                    next_bus_stop(height, width, direction, &next_h, &next_w);
                    //printf("from src_h: %lu, src_w: %lu and next_h: %lu, next_w: %lu\n", mem_h, mem_w, next_h, next_w);
                    weight = transport_weight(width, (int)mem_h*width+mem_w, (int)next_h*width+next_w, BUS_WEIGHT);
                    sprintf(node_i, "%lu", mem_h*width+mem_w);
                    fputs(node_i, file);
                    fputs(" ", file);
                    sprintf(node_j, "%lu", next_h*width+next_w);
                    fputs(node_j, file);
                    fputs(" ", file);
                    sprintf(wght, "%i", weight);
                    fputs(wght, file);
                    fputs("\n", file);
                    *(buses+(*counter)) = (int)(next_h*width+next_w);
                    (*counter)++;
                    }
            }
            else
            {
                //printf("all directions visited\n");
                /*fputs("-----------------------------\n", file);
                fputs("next_h = ", file);
                sprintf(wght, "%lu", next_h);
                fputs(wght, file);
                fputs(", next_w = ", file);
                sprintf(wght, "%lu", next_w);
                fputs(wght, file);
                fputs("\n", file);*/
                //fputs("+ ", file);
                int sign = rand()%2;
                switch (direction)
                {
                case NORTH:
                    if (next_h>5)
                    {
                        if (rand()%(next_h/5) == 0)
                            done = 1;
                        else{
                            next_h -= 4 + (rand()%3)-1;
                            if (sign == 0){
                                if (next_w > 0)
                                    next_w -= rand()%2;
                            }
                            else{
                                if (next_w < width-1)
                                    next_w += rand()%2;
                            }
                        }
                    }
                    else
                        done = 1;
                    break;
                case EAST:
                    if (next_w < width-5)
                    {
                        if (rand()%((width-next_w)/5) == 0)
                            done = 1;                        
                        else{
                            next_w += 4 + (rand()%3)-1;
                            if (sign == 0){
                                if (next_h > 0)
                                    next_h -= rand()%2;
                            }
                            else{
                                if (next_h < height-1)
                                    next_h += rand()%2;
                            }
                        }
                    }
                    else
                        done = 1;
                    break;
                case SOUTH:
                    if (next_h < height-5)
                    {
                        if (rand()%((height-next_h)/5) == 0)
                            done = 1;
                        else{
                            next_h += 4 + (rand()%3)-1;
                            if (sign == 0){
                                if (next_w > 0)
                                    next_w -= rand()%2;
                            }
                            else{
                                if (next_w < width-1)
                                    next_w += rand()%2;
                            }
                        }
                    }
                    else
                        done = 1;
                    break;
                case WEST:
                    if (next_w > 5)
                    {
                        if (rand()%(next_w/5) == 0)
                            done = 1;
                        else{
                            next_w -= 4 + (rand()%3)-1;
                            if (sign == 0){
                                if (next_h > 0)
                                    next_h -= rand()%2;
                            }
                            else{
                                if (next_h < height-1)
                                    next_h += rand()%2;
                            }
                        }
                    }
                    else
                        done = 1;
                    break;
                default:
                    break;
                }
                if (!done)
                {
                    //printf("direction = %i\n", direction);
                    //printf("from src_h: %lu, src_w: %lu and dst_h: %lu, dst_w: %lu\n", mem_h, mem_w, next_h, next_w);
                    weight = transport_weight(width, (int)mem_h*width+mem_w, (int)next_h*width+next_w, BUS_WEIGHT);
                    sprintf(node_i, "%lu", mem_h*width+mem_w);
                    fputs(node_i, file);
                    fputs(" ", file);
                    sprintf(node_j, "%lu", next_h*width+next_w);
                    fputs(node_j, file);
                    fputs(" ", file);
                    sprintf(wght, "%i", weight);
                    fputs(wght, file);
                    fputs("\n", file);
                    *(buses+(*counter)) = (int)(next_h*width+next_w);
                    (*counter)++;
                    /*fputs("next_h = ", file);
                    sprintf(wght, "%lu", next_h);
                    fputs(wght, file);
                    fputs(", next_w = ", file);
                    sprintf(wght, "%lu", next_w);
                    fputs(wght, file);
                    fputs("\n", file);
                    fputs("-----------------------------\n", file);*/
                }
            }
        }
    }
}


int* generate_bus(size_t height, size_t width)
{
    srand(time(0));
    FILE *file = fopen("gen_bus.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    size_t q_buses = 0;
    int * buses = calloc(width*height/4, sizeof(int));
    for (size_t x = 0; x < width*height/4; x++)
    {
        *(buses+x) = -1;
    }
    int counter = 0;
    if (width > 7)
    {
        if (height > 6)
        {
            size_t h_cpy = height;
            size_t w_cpy = width;
            while(h_cpy > 1)
            {
                q_buses++;
                h_cpy/=10;
                if ((h_cpy < 5) && (h_cpy > 1))
                    q_buses--;
            }
            while (w_cpy > 1)
            {
                q_buses++;
                w_cpy/= 10;
                if ((w_cpy < 7) && (w_cpy > 1))
                    q_buses--;
            }
            q_buses *= 2;
            q_buses += (rand()%2 -1);
            if (height<15)
                q_buses--;
            if (width<15)
                q_buses--;
        }
    }
    printf("calling function with q_buses = %li\n", q_buses);
    size_t start_h = 0;
    size_t start_w = 0;
    while(q_buses > 0)
    {
        start_h = rand() % height;
        start_w = rand() % width;
        int direction = rand() % 4;
        if ((start_h < 6) && (direction == NORTH))
            direction = SOUTH;
        if ((start_h > height - 6) && (direction == SOUTH))
            direction = NORTH;
        if ((start_w < 6) && (direction == WEST))
            direction = EAST;
        if ((start_w > width - 6) && (direction == EAST))
            direction = WEST;
        fputs("new line -----------\n", file);
        build_bus_line(height, width, start_h, start_w, direction, file, buses, &counter);
        q_buses--;
    }
    return buses;
}
/////////////////////////////FIX DES FOIS YA X X AVEC POID 0 QUI APARAIT A LA FIN


void generate_graph(size_t height, size_t width)
{
    int * buses = generate_bus(height, width);
    int counter = 0;
    while(*(buses+counter) != -1)
    {
        printf("%i\n", *(buses+counter));
        counter++;
    }
    print_generated_motorized_graph(height, width, buses, BUS);
    //reate_walk_graph(height, width, 0);
    //int bike_count = generate_bike(height, width);
    //print_generated_bike_graph(21, 21, bike_count);
    //int * metro = generate_metro(height, width);
    //int * tram = generate_tram(height, width);
    //print_generated_motorized_graph(height, width, metro, METRO);
    //print_generated_motorized_graph(height, width, tram, TRAM);
    //int * trams = generate_tram(height, width);
    //print_generated_tram_graph(height, width, trams);
}