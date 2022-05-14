#include "auxilary.h"


//Function that returns the weight of an arc
//it is equal to the distance between node start and end time the transport type weight
//width is the width of the graph

int transport_weight(int width, int start, int end, int transport)
{
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
            while(start > end){
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
            while (start < end){ 
                dist++;
                start++;
            }
        }
    }
    return dist*transport;
}


//this function prints an arc into a .txt file
//wght is the type of transport weight is: referenced as macros
//width = width of graph
//arc is from node_1 to node_2

void print_to_file(int width, int node_1, int node_2, int wght, FILE* file)
{
    char w[4], node_i[10], node_j[10];
    int weight = transport_weight(width, node_1, node_2, wght);
    sprintf(node_i, "%i", node_1);
    fputs(node_i, file);
    fputs(" ", file);
    sprintf(node_j, "%i", node_2);
    fputs(node_j, file);
    fputs(" ", file);
    sprintf(w, "%i", weight);
    fputs(w, file);
    fputs("\n", file);
}


//This function returns the new transport's stop height
//it takes the graph's height, the current's stop height
//dist is the distance between every stop, change is the limit of variation

int next_node_height(int height, int start_height, int dist, int change)
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
    if (next >= height)
        return height-1;
    return next;
}


//This function returns the new transport's stop width
//it takes the graph's width, the current's stop width,
//dist is the distance between every stop, change is the limit of variation

int next_node_width(int width, int start_width, int dist, int change)
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
    if (next >= width)
        return width-1;
    return next;
}


//Function that prints in the graph .txt file 

void print_to_walk_file(FILE* file, int node_1, int node_2)
{
    char h[20];
    sprintf(h, "%i", node_1);
    fputs(h, file);
    fputs(" ", file);
    sprintf(h, "%i", node_2);
    fputs(h, file);
    fputs(" 90", file);
}


//regrouping tram stops if one is withing a "gap" lengthed square distance of another
//graph width = width
//nodes = previously placed tram stops
//new tram stop we want to place: its height on the graph = next_height, its width = next_width
//function changes next_height and next_width if close to another node

void regroup(int width, int* nodes, int q_max, int* next_height, int* next_width, int gap)
{
    int y = 0, n;////////////////////////CHECK IS Q_MAX IS NEEDED
    while ((*(nodes+y) != -1) & (y < q_max))
    {
        n = *(nodes+y);
        int n_w = 0;
        int n_h = 0;
        while(n - width > 0){
            n_h++;
            n-=width;
        }
        while(n-1 > -1){
            n_w++;
            n--;
        }
        if ((*next_height - n_h <= gap) & (*next_height - n_h >= -gap)){
            if ((*next_width - n_w <= gap) & (*next_width - n_w >= -gap)){
                *next_height = n_h;
                *next_width = n_w;
                break;
            }
        }
        y++;
    }
}


//regrouping bus stops if one is withing "gap" distance of another
//graph width = width
//nodes = previously placed bus stops
//new bus stop we want to place: its height on the graph = next_height, its width = next_width
//function changes next_height and next_width if close to another node

void regroup_bus(int width, int* nodes, int* next_height, int* next_width, int gap)
{
    int y = 0, n;/////////////////////////////CHANGE TO STOP WHEN REGROUPED FIRST
    while (*(nodes+y) != -1)
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
        while(n > 0)
        {
            n_w++;
            n--;
        }
        //printf("n_h = %i, n_w = %i\n", n_h, n_w);
        if ((*next_height - n_h <= gap) && (*next_height - n_h >= -gap))
        {
            if ((*next_width - n_w == 0) && (*next_width - n_w == 0))
            {  
                printf("condition met in :----------------\n");
                printf("start: next_h = %i, next_w = %i so node: %i\n", *next_height, *next_width, (*next_height)*width+(*next_width));
                *next_height = n_h;
                *next_width = n_w;
                printf("changed to next_h = %i, next_w = %i so node: %i\n", *next_height, *next_width, (*next_height)*width+(*next_width));
                break;
            }
        }
        else if ((*next_height - n_h == 0) && (*next_height - n_h == 0))
        {
            if ((*next_width - n_w <= gap) && (*next_width - n_w >= -gap))
            {  
                printf("condition met in :----------------\n");
                printf("start: next_h = %i, next_w = %i so node: %i\n", *next_height, *next_width, (*next_height)*width+(*next_width));
                *next_height = n_h;
                *next_width = n_w;
                printf("changed to next_h = %i, next_w = %i so node: %i\n", *next_height, *next_width, (*next_height)*width+(*next_width));
                break;
            }
        }
        y++;
    }
}



//This functions gives the coordinates for the next bus stop before all the directions have been covered
// size of graph height * width
// direction of the bus: direction with 0 = north, 1 = east, 2 = south, 3 = west
// cordinates of the current bus stop: height: stop_h; width: stop_w
// returns coords of new bus stop

void next_bus_stop(int height, int width, int direction, int *stop_h, int *stop_w)
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