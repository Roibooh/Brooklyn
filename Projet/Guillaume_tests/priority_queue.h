#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

struct queue_elt
{
    size_t node;
    size_t priority;
    struct queue_elt* next;
};

void insert(struct queue_elt** queue, size_t node, size_t priority);
size_t extract_min(struct queue_elt** queue);
void change_prio(struct queue_elt** queue, size_t node, size_t new_prio);
void free_queue(struct queue_elt* queue);

#endif
