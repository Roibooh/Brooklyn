#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "binary_heap.h"

struct bin_heap* bin_heap_init(size_t n)
{
    struct bin_heap* bh = malloc(sizeof(struct bin_heap));
    bh->arr = malloc(n * sizeof(struct bin_elt));
    bh->map = malloc(n * sizeof(size_t));
    bh->capacity = n;
    bh->heap_size = 0;
    return bh;
}

void free_bin_heap(struct bin_heap* bh)
{
    free(bh->arr);
    free(bh->map);
    free(bh);
}

size_t parent(size_t n)
{
    return (n - 1) / 2;
}

size_t left(size_t n)
{
    return n * 2 + 1;
}

size_t right(size_t n)
{
    return n * 2 + 2;
}

size_t min(struct bin_heap* bh)
{
    return bh->arr[0].value;
}

void swap(struct bin_elt* x, struct bin_elt* y)
{
    struct bin_elt tmp = *x;
    *x = *y;
    *y = tmp;
}

void make_heap(struct bin_heap* bh, size_t i)
{
    size_t l = left(i);
    size_t r = right(i);
    size_t smallest = i; //value of arr[i] already changed

    if (l < bh->heap_size && bh->arr[l].key < bh->arr[smallest].key)
        smallest = l;
    if (r < bh->heap_size && bh->arr[r].key < bh->arr[smallest].key)
        smallest = r;
    if (smallest != i)
    {
        //now the index of the value that was at i is smallest
        bh->map[bh->arr[i].value] = smallest;
        bh->map[bh->arr[smallest].value] = i;
        
        swap(&(bh->arr[i]), &(bh->arr[smallest]));

        make_heap(bh, smallest);
    }
}


void insert(struct bin_heap* bh, size_t key, size_t value)
{
    struct bin_elt new;
    new.key = key;
    new.value = value;

    size_t i = bh->heap_size; //last elt
    bh->heap_size++;
    bh->arr[i] = new;
    bh->map[value] = i;

    while (i != 0 && bh->arr[parent(i)].key > bh->arr[i].key)
    {
        swap(&(bh->arr[i]), &(bh->arr[parent(i)]));
        
        //index of value is now parent(i)
        bh->map[value] = parent(i);    
        //the value that was at parent is now at if
        bh->map[bh->arr[i].value] = i; 
        
        i = parent(i);
    }
}

void decrease_key(struct bin_heap* bh, size_t i, size_t key)
{
    bh->arr[i].key = key;
    while(i != 0 && bh->arr[parent(i)].key > bh->arr[i].key)
    {
        bh->map[bh->arr[i].value] = parent(i);
        bh->map[bh->arr[parent(i)].value] = i;
        
        swap(&(bh->arr[i]), &(bh->arr[parent(i)]));
        i = parent(i);
    }
}

size_t extract_min(struct bin_heap* bh)
{
    if (bh->heap_size <= 0)
        errx(EXIT_FAILURE, "extract on an empty heap");
    if (bh->heap_size == 1)
    {
        bh->heap_size--;
        return bh->arr[0].value;
    }

    size_t root = bh->arr[0].value;
    bh->arr[0] = bh->arr[bh->heap_size - 1];
    bh->heap_size--;

    make_heap(bh, 0);

    return root;
}

void find(struct bin_heap* bh, size_t new_key, size_t value)
{
    size_t index = bh->map[value];
    decrease_key(bh, index, new_key);
}

