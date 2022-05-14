#ifndef GENERATE_BUS_H
#define GENERATE_BUS_H

#include "auxilary.h"

void build_bus_line(int height, int width, int start_h, int start_w, int direction, FILE *file, int* buses, int *counter);
int* generate_bus(int height, int width);

#endif