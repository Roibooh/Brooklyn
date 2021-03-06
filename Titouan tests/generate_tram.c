#include "auxilary.h"

#define TRAM_WEIGHT 18


//Generating metros nodes and arcs
//Still need to look into bettering this function (upgraded the function in generate_metro_v2)
//size of graph: height * width

//Generating trams nodes and arcs
//size of graph: height * width

int *generate_tram(int height, int width)
{
    srand(time(0));
    FILE *file = fopen("gen_graph/tram.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    int vertical_ways = rand()%(height/8);
    if (vertical_ways == 0)
        vertical_ways = rand()%(height/8);
    int horizontal_ways = rand()%(width/8);
    if (horizontal_ways == 0)
        horizontal_ways = rand()%(width/8);
    int arr_len = vertical_ways*horizontal_ways*height*width/25;
    int *nodes = calloc(sizeof(int), arr_len);
    for (int x = 0; x < arr_len; x++)
    {
        *(nodes+x) = -1;
    }

    int start_width = 0, start_height = 0;
    int mem_w = 0, mem_h = 0;
    int neg, i, random;
    for(i = 0; i < vertical_ways; i++) //Generating vertical tram lines
    {
        neg = rand()%2;
        if (neg)
            start_height = ((i+1)*height/(vertical_ways+1))+rand()%3;
        else
            start_height = ((i+1)*height/(vertical_ways+1))-rand()%3;
        start_width = rand()%5;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_width+5 < width)
        {
            random = rand()%2;
            mem_h = next_node_height(height, start_height, 0, 2);
            mem_w = next_node_width(width, start_width, 4+random, 2);
            print_to_file(width, start_height*width+start_width, mem_h*width+mem_w, TRAM_WEIGHT, file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    for(i = 0; i < horizontal_ways; ++i) //Generating horizontal tram lines
    {
        neg = rand()%2;
        if (neg)
            start_width = ((i+1)*width/(horizontal_ways+1))+rand()%3;
        else
            start_width = ((i+1)*width/(horizontal_ways+1))-rand()%3;
        start_height = rand()%6;
        *(nodes+counter) = start_height*width + start_width;
        counter++;
        while(start_height+5 < height)
        {
            random = rand()%2;
            mem_h = next_node_height(height, start_height, 4+random, 2);
            mem_w = next_node_width(width, start_width, 0, 2);
            regroup(width, nodes, arr_len, &mem_h, &mem_w, 2);
            print_to_file(width, start_height*width+start_width, mem_h*width+mem_w, TRAM_WEIGHT, file);
            start_height = mem_h;
            start_width = mem_w;
            *(nodes+counter) = start_height*width + start_width;
            counter++;
        }
    }
    int seconds = 1;
    wait(&seconds);
    fclose(file);
    return nodes;
}