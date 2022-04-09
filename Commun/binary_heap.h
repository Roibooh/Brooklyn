#ifndef BINARY_HEAP_H
#define BINARY_HEAP_h

struct bin_elt
{
    size_t key;
    size_t value;
};

struct bin_heap
{
    struct bin_elt* arr;
    size_t capacity; //max nb elt
    size_t heap_size; //current nb elt
    size_t* map; //at map[v] is the index of v in arr
};

struct bin_heap* bin_heap_init(size_t n);
void free_bin_heap(struct bin_heap*);

size_t extract_min(struct bin_heap* bh);
void insert(struct bin_heap* bh, size_t key, size_t value);
void find(struct bin_heap* bh, size_t new_key, size_t value);

#endif
