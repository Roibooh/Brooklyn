#ifndef GRAPH_PRINT_H
#define GRAPH_PRINT_H

#include <stddef.h>
#include "graph.h"
#include "dijkstra.h"

void print_graph_into_file(size_t height, size_t width);
void print_list(struct node* path);
void complete_btb_path(size_t origin, size_t dest, size_t g_width, struct node *res, int transport);
struct node* complete_path(struct node *path, size_t graph_width);
void find_neighbor_list(struct node *path, size_t node, int grid_width, int *neighbor_1, int *neighbor_2);
void symbol(char *s, int size, int* metro);
void print_path_terminal(struct node *path_incomplete, int height, int width);

#endif