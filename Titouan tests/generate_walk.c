#include "auxilary.h"


//Building a .txt file with al the arcs of the base graph (without transports)
// size of graph: height * width

void create_walk_graph(int height, int width)
{
    srand(time(0));
    FILE *file = fopen("gen_graph/gen_main.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    char m[4];
    int max = height*width;
    sprintf(m, "%i", max);
    fputs(m, file);
    fputs("\n", file);
    for(int h_counter = 0; h_counter < height-1; ++h_counter)
    {
        for(int w_counter = 0; w_counter < width-1; ++w_counter)
        {
            print_to_walk_file(file, h_counter*width+w_counter, h_counter*width+w_counter+1);
            fputs("\n", file);
            print_to_walk_file(file, h_counter*width+w_counter, (h_counter+1)*width+w_counter);
            fputs("\n", file);
        }
    }
    for(int w_counter = 0; w_counter < width-1; ++w_counter)
    {
        print_to_walk_file(file, (height-1)*width+w_counter, (height-1)*width+w_counter+1);
        fputs("\n", file);
    }
    for(int h_counter = 0; h_counter < height-1; ++h_counter)
    {
        print_to_walk_file(file, h_counter*width+(width-1), (h_counter+1)*width+(width-1));
        if (h_counter != height-2)
            fputs("\n", file);
    }
    fclose(file);
}
