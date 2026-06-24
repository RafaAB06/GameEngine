#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#define INVALID_INDEX -1

#include "../types.h"

typedef struct{
    void *array;

    int32 size;
    int32 count;

    int32 item_size;
    int32 step_size;
}ArrayList;

ArrayList *array_list_new(int32 item_size);

void array_list_add(ArrayList *list, void *data);
void array_list_get(ArrayList *list, void *data, int32 index);
void array_list_set(ArrayList *list, void *data, int32 index);
void array_list_remove(ArrayList *list, void *data, COMPARATOR_F comparator);
void array_list_remove_at(ArrayList *list, int32 index);
void array_list_free(ArrayList *list);
int32 array_list_index_of(ArrayList *list, void *data, COMPARATOR_F comparator);
int32 array_list_contains(ArrayList *list, void *data, COMPARATOR_F comparator);
int32 array_list_count_of(ArrayList *list, void *data, COMPARATOR_F comparator);
void array_list_sort(ArrayList *list, COMPARATOR_F comparator);
void array_List_bs(ArrayList *list, COMPARATOR_F comparator);
void array_list_clear(ArrayList* list);
void array_list_free(ArrayList *list);

#endif