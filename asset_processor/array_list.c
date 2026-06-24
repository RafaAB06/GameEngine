#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.h"

#include "array_list.h"

#define DEFAULT_SIZE 32

static inline void check_size(ArrayList *list){
    int32 count = list->count;
    int32 size = list->size;

    if(count >= size){
        int32 new_size = size * 2;

        list->array = realloc(
            list->array,
            new_size * list->item_size
        );

        list->size = new_size;
        return;
    }

    if(count < size / 4 && size > DEFAULT_SIZE){
        int32 new_size = size / 2;

        list->array = realloc(
            list->array,
            new_size * list->item_size
        );

        list->size = new_size;
    }
}

ArrayList *array_list_new(int32 item_size){
    ArrayList *list = (ArrayList *)malloc(sizeof(ArrayList));
    if(!list){
        printf("Cound not allocate array list!");
        return NULL;
    }
    void *array = malloc(item_size * DEFAULT_SIZE);
    if(!array){
        printf("Could not allocated array from array list!");
        return NULL;
    }

    list->array = array;
    list->count= 0;
    list->size = DEFAULT_SIZE;
    list->item_size = item_size;

    return list;
}

void array_list_add(ArrayList *list, void *data){
    check_size(list);

    int8 *array = (int8 *)list->array;

    memcpy(
        array + list->count * list->item_size,
        data,
        list->item_size
    );

    list->count++;
}

void array_list_get(ArrayList *list, void *data, int32 index){
    int32 count = list->count;

    if(index >= count) return;

    int32 item_size = list->item_size;

    void *array = list->array;
    void *postion = ((int8 *)array) + index * item_size;

    memcpy(data, postion, item_size);
}

void array_list_set(ArrayList *list, void *data, int32 index){
    if(index >= list->count) return;

    int32 item_size = list->item_size;

    void *array = list->array;
    void *position = ((int8 *)array) + index * item_size;

    memcpy(position, data, item_size);
}


void array_list_remove(ArrayList *list, void *data, COMPARATOR_F comparator){
    int32 count = list->count, i;
    int32 item_size = list->item_size;
    void *array = list->array;

    for(i = 0; i<count; i++){
        void *position = ((int8 *)array) + i * item_size;

        int32 comparison;
        if(comparator){
            comparison = comparator(data, position);
        }else{
            comparison = memcmp(data, position, item_size);
        }

        if(!comparison){
            array_list_remove_at(list, i);
            return;
        }
    }
}

void array_list_remove_at(ArrayList *list, int32 index){
    int32 count = list->count;

    if(index >= count) return;
    list->count--;

    int32 last_index = count - 1;
    if(last_index == index) return;
    
    int32 item_size = list->item_size;
    void *array = list->array;
    void *position = ((int8 *)array) + index * item_size;
    void *last_position = ((int8 *)array) + last_index * item_size;
    memcpy(position, last_position, item_size);
}

int32 array_list_index_of(ArrayList *list, void *data, COMPARATOR_F comparator){
    int32 count = list->count;
    void *array = list->array;

    int32 item_size = list->item_size;
    int32 i;
    for(i = 0; i<count; i++){
        void *position = ((int8 *)array) + i * item_size;

        int32 comparison;
        if(comparator){
            comparison = comparator(data, position);
        }else{
            comparison = memcmp(data, position, item_size);
        }

        if(!comparison){
            return i;
        }
    }

    return INVALID_INDEX;
}

int32 array_list_contains(ArrayList *list, void *data, COMPARATOR_F comparator){
    int32 index = array_list_index_of(list, data, comparator);

    return index != INVALID_INDEX;
}

int32 array_list_count_of(ArrayList *list, void *data, COMPARATOR_F comparator){
    int32 count = list->count;
    void *array = list->array;

    int32 item_size = list->item_size;
    int32 i, c;
    for(i = 0; i<count; i++){
        void *position = ((int8 *)array) + i * item_size;

        int32 comparison;
        if(comparator){
            comparison = comparator(data, position);
        }else{
            comparison = memcmp(data, position, item_size);
        }

        if(!comparison){
            c++;
        }
    }

    return c;
}

INLINE void swap(void *ptr_1, void *ptr_2, int32 item_size){
    int8 temp[item_size];
    memcpy(temp, ptr_1, item_size);
    memcpy(ptr_1, ptr_2, item_size);
    memcpy(ptr_2, temp, item_size);
}

INLINE int32 partition(ArrayList *list, COMPARATOR_F comparator, int32 min, int32 max){
    int32 pivot = max;
    int32 store = min;
    int32 item_size = list->item_size;
    void *array = list->array;
    void *pivot_position = ((int8 *)array) + pivot * item_size;

    int32 i;
    for(i = min; i <= pivot - 1; i++){
        void *position = ((int8 *)array) + i * item_size;
        
        int32 comparison;
        if(comparator){
            comparison = comparator(position, pivot_position);
        }else{
            comparison = memcmp(position, pivot_position, item_size);
        }

        if(comparison < 0){
            void *store_position = ((int8 *)array) + store * item_size;
            swap(store_position, position, item_size);

            store++;
        }
    }
    void *new_pivot_position = ((int8 *)array) + store * item_size;
    swap(new_pivot_position, pivot_position, item_size);

    return store;
}

void quick_sort(ArrayList *list, COMPARATOR_F comparator, int32 min, int32 max){
    if(min < max){
        int32 pivot = partition(list, comparator, min, max);
        quick_sort(list, comparator, min, pivot - 1);
        quick_sort(list, comparator, pivot + 1, max);
    }
}

void array_list_sort(ArrayList *list, COMPARATOR_F comparator){
    int32 min = 0, max = list->count - 1;
    quick_sort(list, comparator, min, max);
}

void array_List_bs(ArrayList *list, COMPARATOR_F comparator){

}

void array_list_free(ArrayList *list){
    free(list->array);
    free(list);
}
