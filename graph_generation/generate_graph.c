#include "generate_bus.h"
#include "generate_tram.h"
#include "generate_metro.h"
#include "generate_bike.h"
#include "generate_walk.h"
#include "graph_print.h"


#define BIKE_WEIGHT 30
#define METRO_WEIGHT 6
#define TRAM_WEIGHT 18
#define BUS_WEIGHT 24
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3



void generate_graph(int height, int width)
{
    /*int * buses = */generate_bus(height, width);
    /*int bike_count = */generate_bike(height, width);
    //print_generated_bike_graph(21, 21, bike_count);
    create_walk_graph(height, width);
    /*int *metro = */generate_metro_v1(height, width); /////////////////////////////DEBUG AT 24*40 seg fault
    //generate_metro_v1(height, width);
    //int* metro = generate_metro_v2(height, width);
    //print_generated_motorized_graph(height, width, metro, METRO);
    /*int *tram = */generate_tram(height, width);
    //print_generated_motorized_graph(height, width, tram, TRAM);
}