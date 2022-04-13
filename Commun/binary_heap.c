#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "binary_heap.h"

struct bin_heap* bin_heap_init(size_t n)
{
    /*
    **  allocate and create a bina_heap of n element
    */
    struct bin_heap* bh = malloc(sizeof(struct bin_heap));
    bh->arr = malloc(n * sizeof(struct bin_elt));
    bh->map = malloc(n * sizeof(size_t));
    bh->capacity = n;
    bh->heap_size = 0;
    return bh;
}

void free_bin_heap(struct bin_heap* bh)
{
    /*
    **  free a bin_heap
    */
    free(bh->arr);
    free(bh->map);
    free(bh);
}

size_t parent(size_t i)
{
    //return parent of the node at index i
    return (i - 1) / 2;
}

size_t left(size_t i)
{
    //return left child of the node at index i
    return i * 2 + 1;
}

size_t right(size_t i)
{
    //return the right child of the node at index i
    return i * 2 + 2;
}

void swap(struct bin_elt* x, struct bin_elt* y)
{
    /*
    **  swap two element of the heap
    */
    struct bin_elt tmp = *x;
    *x = *y;
    *y = tmp;
}

void make_heap(struct bin_heap* bh, size_t i)
{
    /*
    **  put the elt in order when it was broken
    **  by swapping the node at index i with it's swallest child
    **  do it until it is smaller than all it's new children
    */
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
    /*
    **  insert a new element in the heap
    **  key and value are the key and value of the new elt
    */
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
    /*
    **  decrease the key of the elt at index i to key
    **  then swap it with it's parent until it is the correct place
    */
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
    /*
    **  remove the minimum from the heap
    **  then call make_heap to put every node in order
    */
    size_t root = bh->arr[0].value;
    bh->arr[0] = bh->arr[bh->heap_size - 1];
    bh->heap_size--;

    make_heap(bh, 0);

    return root;
}

void find(struct bin_heap* bh, size_t new_key, size_t value)
{
    /*
    **  decrease the key of the elt of value value
    **  get it's indexx using the map list
    */
    size_t index = bh->map[value];
    decrease_key(bh, index, new_key);
}

