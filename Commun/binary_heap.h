#ifndef BINARY_HEAP_H
#define BINARY_HEAP_h

//an element of the heap
struct bin_elt
{
    //key is the priority of the elt in the heap
    size_t key;
    
    //value is the vertex of the elt in the graph
    size_t value;
};

//structure for the heap
struct bin_heap
{
    //arr is the list to store the elts
    struct bin_elt* arr; 

    //capacity is the max number of elt that the heap can hold
    size_t capacity;

    //heap size is the current number of elt in the heap
    size_t heap_size;

    //list to efficiently get index of a value, at map[v] is index of v in arr
    size_t* map;
};

//allocate and create a bina_heap of n element
struct bin_heap* bin_heap_init(size_t n);

//free a bin_heap
void free_bin_heap(struct bin_heap*);

/*
**  remove the minimum from the heap
**  then call make_heap to put every node in order*/
size_t extract_min(struct bin_heap* bh);

/*  
**  insert a new element in the heap
**  key and value are the key and value of the new elt */
void insert(struct bin_heap* bh, size_t key, size_t value);

/*
**  decrease the key of the elt of value value
**  get it's indexx using the map list*/
void find(struct bin_heap* bh, size_t new_key, size_t value);

#endif
