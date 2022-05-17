#include "auxilary.h"

#define BUS_WEIGHT 24
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


// Builds bus bus lines
// size of graph height * width
// direction of the start of the busline: direction with 0 = north, 1 = east, 2 = south, 3 = west
// cordinates of the current bus stop: height: stop_h; width: stop_w
// returns coords of new bus stop
// file being the .txt where I stock the bus lines
// *buses is an array where i stock the bus line coordinates to make printing easier later.
// *couter counts the amout of bus stops

void build_bus_line(int height, int width, int start_h, int start_w, int direction, FILE *file, int* buses, int *counter)
{
    int next_h = start_h;
    int next_w = start_w;
    int mem_h;
    int mem_w;
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
    regroup_bus(width, buses, &next_h, &next_w, 1);
    print_to_file(width, start_h*width+start_w, next_h*width+next_w, BUS_WEIGHT, file);
    *(buses+(*counter)) = (next_h*width+next_w);
    (*counter)++;
    while ((next_h != start_h) || (next_w != start_w))
    {
        if (done)
        {
            //last part, finishing up the bus line
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
                weight = transport_weight(width, mem_h*width+mem_w, next_h*width+next_w, BUS_WEIGHT);
                sprintf(node_i, "%i", mem_h*width+mem_w);
                fputs(node_i, file);
                fputs(" ", file);
                sprintf(node_j, "%i", next_h*width+next_w);
                fputs(node_j, file);
                fputs(" ", file);
                sprintf(wght, "%i", weight);
                fputs(wght, file);
                fputs("\n", file);
                j--;
                *(buses+(*counter)) = (next_h*width+next_w);
                (*counter)++;
            }
            if (i != 1)
            {
                weight = transport_weight(width, next_h*width+next_w, start_h*width+start_w, BUS_WEIGHT);
                sprintf(node_i, "%i", next_h*width+next_w);
                fputs(node_i, file);
                fputs(" ", file);
                sprintf(node_j, "%i", start_h*width+start_w);
                fputs(node_j, file);
                fputs(" ", file);
                sprintf(wght, "%i", weight);
                fputs(wght, file);
                fputs("\n", file);
                next_h = start_h;
                next_w = start_w;
                *(buses+(*counter)) = (next_h*width+next_w);
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
                //not yet visited all directions
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
                            if (n == 1){
                                full = 1;
                            }
                            else {
                                if (next_h < 5){
                                    full = 1;
                                } 
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
                                else{
                                    full = 1;
                                }
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
                            if (w == 1){
                                full = 1;
                            }
                            else {
                                if (next_w < 5){
                                    full = 1;
                                }
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
                                else{
                                    full = 1;
                                }
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
                            if (n == 1){
                                full = 1;
                            }
                            else {
                                if (next_h < 5){
                                    full = 1;
                                }
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
                                else{
                                    full = 1;
                                }
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
                    sprintf(node_i, "%i", direction);
                    fputs(node_i, file);
                    fputs(" - ", file);
                    next_bus_stop(height, width, direction, &next_h, &next_w);
                    regroup_bus(width, buses, &next_h, &next_w, 1);
                    print_to_file(width, mem_h*width+mem_w, next_h*width+next_w, BUS_WEIGHT, file);
                    *(buses+(*counter)) = (next_h*width+next_w);
                    (*counter)++;
                    }
            }
            else
            {///////////////////////////////////////////////////////////////////////////////////ALL DIRECTIONS VISITED, JUST MOVING FORWARD///////////////////
                int sign = rand()%2;
                switch (direction)
                {
                case NORTH:
                    if (next_h>5)
                    {
                        if (rand()%(next_h/5) == 0){
                            done = 1;
                        }
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
                    else{
                        done = 1;
                    }
                    break;
                case EAST:
                    if (next_w < width-5)
                    {
                        if (rand()%((width-next_w)/5) == 0){
                            done = 1;                      
                        } 
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
                    else{
                        done = 1;
                    }
                    break;
                case SOUTH:
                    if (next_h < height-5)
                    {
                        if (rand()%((height-next_h)/5) == 0){
                            done = 1;
                        }
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
                    else{
                        done = 1;
                    }
                    break;
                case WEST:
                    if (next_w > 5)
                    {
                        if (rand()%(next_w/5) == 0){
                            done = 1;
                        }
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
                    regroup_bus(width, buses, &next_h, &next_w, 1);
                    print_to_file(width, mem_h*width+mem_w, next_h*width+next_w, BUS_WEIGHT, file);
                    *(buses+(*counter)) = (next_h*width+next_w);
                    (*counter)++;
                }
            }
        }
    }
}




////////////////////////////////////////////////////////////MAIN FUNCTION///////////////
int* generate_bus(int height, int width)
{
    srand(time(0));
    FILE *file = fopen("gen_graph/gen_bus.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    //just finding the amount of bus lines to put in
    int q_buses = 0;
    int * buses = calloc(width*height/4, sizeof(int));
    for (int x = 0; x < width*height/4; x++)
    {
        *(buses+x) = -1;
    }
    int counter = 0;
    if (width > 7)
    {
        if (height > 6)
        {
            int h_cpy = height;
            int w_cpy = width;
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
    //building all the bus lines
    //printf("calling function with q_buses = %i\n", q_buses);
    q_buses = 1;////////////////////////////////////////////////////////////////REMOVE LATER
    int start_h = 0;
    int start_w = 0;
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
