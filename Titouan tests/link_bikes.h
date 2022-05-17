#ifndef LINK_BIKES_H
#define LINK_BIKES_H

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include<sys/wait.h>


int give_weight(int node_1, int node_2, int height, int width);
void link_bikes(char* data, char* res, int height, int width, int bike_number, int bike_weight);


#endif