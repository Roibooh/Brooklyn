#include "auxilary.h"

#define BIKE_WEIGHT 30


//Generating location of bikes randomly, and then linking them up in gen_bikes.txt
//size of graph: height * wodth

int generate_bike(int height, int width)
{
    FILE *file = fopen("gen_graph/gen_bike.txt", "w");
    if(file == NULL)
    {
        err(1, "Could not find or open file!");
    }
    int counter = 0;
    char h[4];
    for(int x = 0; x < height; ++x)
    {
        for(int y = 0; y < width; ++y)
        {
            if ((x%4 == 0) & (y%4 == 0))
            {
                int coord_x = x + rand()%3;
                if (coord_x < height)
                {
                    int coord_y = y + rand()%3;
                    if (coord_y < width)
                    {
                        sprintf(h, "%i", coord_x*width+coord_y);
                        fputs(h, file);
                        fputs("\n", file);
                        counter++;
                    }
                } 
            }
        }
    }
    fclose(file);
    link_bikes("gen_graph/gen_bike.txt", "gen_graph/gen_bikes.txt", height, width, counter, BIKE_WEIGHT);
    return counter;
}