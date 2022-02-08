#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

void create_walk_graph(size_t height, size_t width, int walking)
{
    FILE *file = fopen("main.txt", "w");
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