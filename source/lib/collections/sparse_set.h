#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include "../types.h"

#define DEFAULT_POOL_SIZE 16

typedef struct{
    uint32 *sparse;
    uint32 sparse_size;

    uint32 *dense;
    void *array;
    uint32 size;
    uint32 count;
    uint32 item_size;
}SparseSet;

SparseSet *sparse_set_new(uint32 item_size);

int32 sparse_set_add(SparseSet *set, uint32 id, void *data);
int32 sparse_set_set(SparseSet *set, uint32 id, void *data);
int32 sparse_set_get(SparseSet *set, uint32 id, void *data);
void sparse_set_rem(SparseSet *set, uint32 id);
void sparse_set_sort(SparseSet *set, int32 (* comparator)(void *, void *));

#endif