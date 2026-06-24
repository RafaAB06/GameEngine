#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "sparse_set.h"

#define INVALID_INDEX 0xffffffff//-1 em int

static void ensure_size(SparseSet *set, uint32 id){
    //sparse array
    uint32 sparse_size = set->sparse_size;
    uint32 min_size = id + 1u;
    if(sparse_size < min_size){
        int32 new_size = sparse_size;
        while(sparse_size < min_size){
            new_size *= 2;
        }
        uint32 bytes_size = new_size * sizeof(uint32);
        uint32 *sparse_array = set->sparse;
        uint32 *new_sparse = (uint32 *)malloc(bytes_size);
        int32 i;
        for(i = 0; i<new_size; i++){
            uint32 value = INVALID_INDEX;
            if(i < sparse_size){
                value = sparse_array[i];
            }
            new_sparse[i] = value;
        }
        sparse_array = new_sparse;
        set->sparse = sparse_array;
        set->sparse_size = new_size;

        free(sparse_array);
    }

    uint32 count = set->count;
    uint32 size = set->size;
    //dense arrays
    if(count >= size){
        size *= 2;

        uint32 *dense = set->dense;
        dense = (uint32 *)realloc(dense, size * sizeof(uint32));
        set->dense = dense;

        void *array = set->array;
        array = realloc(array, size * set->item_size);
        set->array = array;

        set->size = size;
    }
}

static void try_shrink(SparseSet *set) {
    if (set->size <= DEFAULT_POOL_SIZE){
        return;
    }

    uint32 new_size = set->size / 2;
    if (set->count >= new_size || new_size < DEFAULT_POOL_SIZE){
        return;
    }    
        
    set->dense = (uint32 *) realloc(set->dense, sizeof(uint32) * new_size);
    set->array = realloc(set->array, set->item_size * new_size);
    set->size = new_size;
}

SparseSet *sparse_set_new(uint32 item_size){
    SparseSet *set = (SparseSet *) malloc(sizeof(SparseSet));
    
    uint32 index_array_size = sizeof(uint32) * DEFAULT_POOL_SIZE;
    uint32 *sparse = (uint32 *)malloc(index_array_size);
    memset(sparse, INVALID_INDEX, index_array_size);

    set->sparse = sparse;
    set->sparse_size = DEFAULT_POOL_SIZE;

    set->dense = (uint32 *)malloc(index_array_size);

    set->array = malloc(item_size * DEFAULT_POOL_SIZE);
    set->size = DEFAULT_POOL_SIZE;
    set->count = 0;
    set->item_size = item_size;

    return set;
}

int32 sparse_set_add(SparseSet *set, uint32 id, void *data){
    ensure_size(set, id);

    uint32 *sparse = set->sparse;
    uint32 current_index = sparse[id];
    if(current_index == INVALID_INDEX){
        uint32 next_index = set->count;
        sparse[id] = next_index;

        uint32 *dense = set->dense;
        dense[next_index] = id;

        uint32 item_size = set->item_size;
        void *array = set->array;
        void *position = ((int8 *)array) + next_index * item_size;
        memcpy(position, data, item_size);

        set->count++; 

        return 1;
    }
    return 0;
}

int32 sparse_set_set(SparseSet *set, uint32 id, void *data){
    uint32 *sparse = set->sparse;
    uint32 index = sparse[id];
    if(index == INVALID_INDEX){
        return 0;
    }

    uint32 item_size = set->item_size;
    void *array = set->array;
    void *position = ((int8 *)array) + index * item_size;
    memcpy(position, data, item_size);
    
    return 1;
}

int32 sparse_set_get(SparseSet *set, uint32 id, void *data){
    uint32 sparse_size = set->sparse_size;

    if(id >= sparse_size) return 0;

    uint32 *sparse = set->sparse;
    uint32 index = sparse[id];

    if(index == INVALID_INDEX) return 0;

    uint32 item_size = set->item_size;
    void *array = set->array;
    void *position = ((int8 *)array) + index * item_size;
    memcpy(data, position, item_size);

    return 1;
}

void sparse_set_rem(SparseSet *set, uint32 id){
    uint32 count = set->count;
    uint32 last_index = count-1;
    uint32 *sparse = set->sparse;

    if(id != last_index){
        uint32 removed_index = sparse[id];

        uint32 *dense = set->dense;
        uint32 moved_id = dense[last_index];
        sparse[moved_id] = removed_index;
        dense[removed_index] = moved_id;

        uint32 item_size = set->item_size;
        void *array = set->array;
        void *moved_position = ((int8 *)array) + last_index * item_size;
        void *removed_position = ((int8 *)array) + removed_index * item_size;
        memcpy(removed_position, moved_position, item_size);
    }
    sparse[id] = INVALID_INDEX;

    set->count--;

    try_shrink(set);
}

INLINE void swap(void *ptr_1, uint32 index_1, void *ptr_2, uint32 index_2, uint32 *sparse, uint32 *dense, int32 item_size){
    uint32 i_1 = dense[index_1];
    uint32 i_2 = dense[index_2];

    sparse[i_1] = index_2;
    sparse[i_2] = index_1;
    dense[index_1] = i_2;
    dense[index_2] = i_1;

    int8 temp[item_size];
    memcpy(temp, ptr_1, item_size);
    memcpy(ptr_1, ptr_2, item_size);
    memcpy(ptr_2, temp, item_size);
}

INLINE int32 partition(SparseSet *set, int32 (* comparator)(void *, void *), int32 min, int32 max){
    int32 pivot = max;
    int32 store = min;
    int32 item_size = set->item_size;
    void *array = set->array;
    void *pivot_position = ((int8 *)array) + pivot * item_size;

    uint32 *sparse = set->sparse;
    uint32 *dense = set->dense;

    int32 i;
    for(i = min; i <= pivot - 1; i++){
        void *position = ((int8 *)array) + i * item_size;
        int32 c_result = comparator(position, pivot_position);

        if(c_result < 0){
            void *store_position = ((int8 *)array) + store * item_size;
            swap(store_position, store, position, i, sparse, dense, item_size);

            store++;
        }
    }
    void *new_pivot_position = ((int8 *)array) + store * item_size;
    swap(new_pivot_position, store, pivot_position, pivot, sparse, dense, item_size);

    return store;
}

static void quick_sort(SparseSet *set, int32 (* comparator)(void *, void *), int32 min, int32 max){
    if(min < max){
        int32 pivot = partition(set, comparator, min, max);
        quick_sort(set, comparator, min, pivot - 1);
        quick_sort(set, comparator, pivot + 1, max);
    }
}

void sparse_set_sort(SparseSet *set, int32 (* comparator)(void *, void *)){
    int32 min = 0, max = set->count - 1;
    quick_sort(set, comparator, min, max);
}

