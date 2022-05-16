#ifndef ANT_H
#define ANT_H

#include "graph.h"

struct chained_list
{
    size_t cost;
    size_t index;
    struct chained_list* next;
};

void free_chained_list(struct chained_list* cl);

struct chained_list* main_ant(size_t g_size, size_t** matrix, size_t start);

#endif
