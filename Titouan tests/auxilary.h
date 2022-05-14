#ifndef AUXILARY_H
#define AUXILARY_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "link_bikes.h"
#include <string.h>
#include <time.h>
#include <math.h>
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

int transport_weight(int width, int start, int end, int transport);
void print_to_file(int width, int node_1, int node_2, int wght, FILE* file);
int next_node_height(int height, int start_height, int dist, int change);
int next_node_width(int width, int start_width, int dist, int change);
void print_to_walk_file(FILE* file, int node_1, int node_2);
void regroup(int width, int* nodes, int q_max, int* next_height, int* next_width, int gap);
void regroup_bus(int width, int* nodes, int* next_height, int* next_width, int gap);
void next_bus_stop(int height, int width, int direction, int *stop_h, int *stop_w);

#endif