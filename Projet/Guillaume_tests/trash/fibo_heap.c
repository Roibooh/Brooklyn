#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "macro.h"
#include "fibo_heap.h"

struct fibo_heap* init_fibo_heap()
{
    struct fibo_heap* fh = malloc(sizeof(struct fibo_heap));
    fh->n = 0;
    fh->min = NULL;
    fh->phi = 0;
    fh->degree = 0;
    return fh;
}
void free_nodes(struct fibo_node* n)
{
    n->visited = TRUE;
    if (n->left != NULL && n->left->visited == FALSE)
        free_nodes(n->left);
    if (n->right != NULL &&  n->right->visited == FALSE)
        free_nodes(n->right);
    if (n->parent != NULL && n->parent->visited == FALSE)
        free_nodes(n->parent);
    if (n->child != NULL && n->child->visited == FALSE)
        free_nodes(n->child);
    free(n);
}
void free_fibo_heap(struct fibo_heap* fh)
{
    if (fh->min != NULL)
        free_nodes(fh->min);
    free(fh);
}
void insert(struct fibo_heap* fh, size_t value, size_t key)
{
    //initialize new node
    struct fibo_node* node = malloc(sizeof(struct fibo_node));
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->mark = FALSE;
    node->parent = NULL;
    node->child = NULL;
    node->visited = FALSE;
    node->left = node;
    node->right = node;
    //if tree is queue is empty then it is the min
    if (fh->min == NULL)
        fh->min = node;
    else
    {
        //else place it at the left of the min
        fh->min->left->right = node;
        node->right = fh->min;
        node->left = fh->min->left;
        fh->min->left = node;
        //if key is the smallest, node becomes the new min
        if (node->key < fh->min->key)
            fh->min = node;
    }
    //increase number of nodes in
    fh->n++;
}
void fib_heap_link(struct fibo_heap* fh, struct fibo_node* y, \
        struct fibo_node* x)
{
    //remove y from where it was
    y->right->left = y->left;
    y->left->right = y->right;
    if (x->right == x)
        fh->min = x;
    //and put it as x child
    y->left = y;
    y->right = y;
    y->parent = x;
    if (x->child == NULL)
        x->child = y;
    y->right = x->child;
    y->left = x->child->left;
    x->child->left->right = y;
    x->child->left = y;
    //if it is the smallest becomes new child
    if (y->key < x->child->key)
        x->child = y;
    
    x->degree += 1;
}

void consolidate(struct fibo_heap* fh)
{
    size_t node_d;
    int degree_int = (int)fh->n; 
    struct fibo_node* arr[degree_int+1];
    for (int i = 0; i <= degree_int; i++)
        arr[i] = NULL;
    
    struct fibo_node* ptr1 = fh->min;
    struct fibo_node* ptr2;
    struct fibo_node* swap;
    struct fibo_node* ptr4 = ptr1;
    do
    {
        ptr4 = ptr4->right;
        node_d = ptr1->degree;
        while (arr[node_d] != NULL)
        {
            ptr2 = arr[node_d];
            if (ptr1->key > ptr2->key)
            {
                swap = ptr1;
                ptr1 = ptr2;
                ptr2 = swap;
            }
            if (ptr2 == fh->min)
                fh->min = ptr1;
            fib_heap_link(fh, ptr2, ptr1);
            if (ptr1->right == ptr1)
                fh->min = ptr1;
            arr[node_d] = NULL;
            node_d++;
        }
        arr[node_d] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != fh->min);

    fh->min = NULL;
    for (int i = 0; i <= degree_int; i++)
    {
        if (arr[i] != NULL)
        {
            arr[i]->left = arr[i];
            arr[i]->right = arr[i];
            if (fh->min == NULL)
                fh->min = arr[i];
            else
            {
                fh->min->left->right = arr[i];
                arr[i]->right = fh->min;
                arr[i]->left = fh->min->left;
                fh->min->left = arr[i];
                if (arr[i]->key < fh->min->key)
                    fh->min = arr[i];
            }
            if (fh->min == NULL)
                fh->min = arr[i];
            else if (arr[i]->key < fh->min->key)
                fh->min = arr[i];
        }
    }
}
struct fibo_node* extract_min(struct fibo_heap* fh)
{
    //if the tree is empty return NULL
    if (fh->min == NULL)
        errx(1, "extract min NULL");
    struct fibo_node* tmp = fh->min;
    struct fibo_node* ptr;
    ptr = tmp;
    struct fibo_node* x = NULL;
    //if the min has children put them at the top
    if (tmp->child != NULL)
    {
        x = tmp->child;
        do 
        {
            ptr = x->right;
            fh->min->left->right = x;
            x->right = fh->min;
            x->left = fh->min->left;
            fh->min->left = x;
            if (x->key < fh->min->key)
                fh->min = x;
            x->parent = NULL;
            x = ptr;
        } while (ptr != tmp->child);
    }
    //remove the node 
    tmp->left->right = tmp->right;
    tmp->right->left = tmp->left;
    fh->min = tmp->right;

    //if it was the last then it is empty now
    if (tmp == tmp->right && tmp->child == NULL)
        fh->min = NULL;
    else
    {
        fh->min = tmp->right;
        consolidate(fh);
    }
    fh->n -= 1;
    return tmp;
}

void cut(struct fibo_heap* fh, struct fibo_node* node, \
        struct fibo_node* parent)
{
    if (node == node->right)
        parent->child = NULL;
    
    node->left->right = node->right;
    node->right->left = node->left;
    if (node == parent->child)
        parent->child = node->right;
    parent->degree -= 1;

    node->left = node;
    node->right = node;
    fh->min->left->right = node;
    node->right = fh->min;
    node->left = fh->min->left;
    fh->min->left = node;

    node->parent = NULL;
    node->mark = TRUE;
}

void cascading_cut(struct fibo_heap* fh, struct fibo_node* parent)
{
    struct fibo_node* tmp;
    tmp = parent->parent;
    if (tmp != NULL)
    {
        if ( parent->mark == FALSE)
            parent->mark = TRUE;
        else
        {
            cut(fh, parent, tmp);
            cascading_cut(fh, tmp);
        }
    }
}

void decrease_priority(struct fibo_heap* fh, struct fibo_node* node, \
                size_t priority)
{
    struct fibo_node* parent;
    node->key = priority;
    parent = node->parent;
    if (parent != NULL && node->key < parent->key)
    {
        cut(fh, node, parent);
        cascading_cut(fh, parent);
    }
    if (node->key < fh->min->key)
        fh->min = node;
}

void find(struct fibo_heap* fh, struct fibo_node* mini, size_t value, \
        size_t new_priority)
{
    struct fibo_node* found = NULL;
    struct fibo_node* tmp = mini;
    tmp->visited = TRUE;
    struct fibo_node* found_ptr = NULL;
    if (tmp->value == value)
    {
        printf("!!! found!!!");
        found_ptr = tmp;
        found = found_ptr;
        decrease_priority(fh, found, new_priority);
    }
    if (found_ptr == NULL)
    {
        if (tmp->child != NULL)
            find(fh, tmp->child, value, new_priority);
        if (tmp->right->visited != TRUE)
            find(fh, tmp->right, value, new_priority);
    }
    tmp->visited = FALSE;
    found = found_ptr;
}

void display(struct fibo_heap* fh)
{
    struct fibo_node* ptr = fh->min;
    if (ptr == NULL)
    {
        printf("Heap is empty");
        return;
    }
    printf("root nodes : ");
    do
    {
        printf("%lu", ptr->value);
        ptr = ptr->right;
        if (ptr != fh->min)
            printf("-->");
    } while (ptr != fh->min && ptr->right != NULL);
    printf("\n");
}
