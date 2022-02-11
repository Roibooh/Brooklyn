#ifndef FIBO_HEAP_H
#define FIBO_HEAP_h

struct fibo_node
{
    size_t key;
    size_t value;
    size_t degree;
    struct fibo_node* left;
    struct fibo_node* right;
    struct fibo_node* parent;
    struct fibo_node* child;
    int mark;
    int visited;
};

struct fibo_heap
{
    int n;
    struct fibo_node* min;
    int phi;
    int degree;
};

struct fibo_heap* init_fibo_heap();
void free_fibo_heap(struct fibo_heap* fh);

struct fibo_node* extract_min(struct fibo_heap* fh);
void insert(struct fibo_heap* fh, size_t value, size_t priority);
void find(struct fibo_heap* fh, struct fibo_node* node, size_t value, \
        size_t new_priority);
#endif
