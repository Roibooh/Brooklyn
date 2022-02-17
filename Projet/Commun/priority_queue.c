#include <stdio.h>
#include <err.h>
#include "priority_queue.h"

void insert(struct queue_elt** queue, size_t node, size_t priority)
{ 
    struct queue_elt* new_elt = malloc(sizeof(struct queue_elt));
    new_elt->priority = priority;
    new_elt->node = node;
    struct queue_elt* tmp = *queue;
    if (tmp == NULL)
    {
        new_elt->next = NULL;
        *queue = new_elt;
    }
    else
    {
        if (tmp->priority >= priority)
        {
            new_elt->next = tmp;
            *queue = new_elt;
        }
        else
        {
            while (tmp->next != NULL && tmp->next->priority < priority)
                tmp = tmp->next;
            new_elt->next = tmp->next;
            tmp->next = new_elt;
        }
    }
}

size_t extract_min(struct queue_elt** queue)
{
    struct queue_elt* tmp = *queue;
    size_t r = tmp->node;
    *queue = tmp->next;
    return r;
}

void change_prio(struct queue_elt** queue, size_t node, size_t new_prio)
{
    struct queue_elt* tmp = *queue;
    if (tmp == NULL)
        errx(1,"change_prio: empty queue");
    if (tmp->next == NULL || tmp->node == node)
    {
        if (tmp->node == node)
            tmp->priority = new_prio;
        else
            errx(1, "change_prio: elt does not exist");
    }
    else
    {
        while (tmp->next->node != node)
        {
            tmp = tmp->next;
        }
        tmp->next = tmp->next->next;
        insert(queue, node, new_prio);
    }
}

void free_queue(struct queue_elt* queue)
{
    if (queue != NULL)
    {
        struct queue_elt* tmp;
        while (queue != NULL)
        {
            tmp = queue;
            queue = queue->next;
            free(tmp);
        }
    }
}
