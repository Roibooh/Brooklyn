#ifndef GRAPH_PRINT_H
#define GRAPH_PRINT_H

#include <stddef.h>
#include "graph.h"
#include "dijkstra.h"

void print_graph_into_file(size_t height, size_t width);
int is_part(size_t* arr, size_t length, size_t node);
void print_generated_bike_graph(size_t height, size_t width, int q_transport);
int is_part_2(int* arr, size_t node);
void print_generated_motorized_graph(size_t height, size_t width, int* metros, int type);
void print_list(struct node* path);
void complete_btb_path(size_t origin, size_t dest, size_t g_width, struct node *res, int transport);
struct node* complete_path(struct node *path, size_t graph_width);
void find_neighbor_list(struct node *path, size_t node, int grid_width, int *neighbor_1, int *neighbor_2);
void symbol(char *s, int size, int* metro);
void print_path_terminal(struct node *path_incomplete, int height, int width);

#endif