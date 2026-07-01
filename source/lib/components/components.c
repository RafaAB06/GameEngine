#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../rendering.h"
#include "../collections/sparse_set.h"
#include "../collections/array_list.h"

#include "../collections/array_list.h"
#include "../math/mathf.h"

#include "../types.h"
#include "../engine.h"

#include "id_generator.h"
#include "components.h"

static ComponentPool pools[COMPONENT_COUNT];
static int32 pool_count;

void init_components(){

}

void update_components(){
    int32 i = COMPONENT_NONE + 1;
    for(i; i < COMPONENT_COUNT; i++){
        ComponentPool pool = pools[i];
        if(pool.type == COMPONENT_NONE) continue;

        SparseSet *set = pool.sparse_set;
        int32 count = set->count;
        int32 c_size = pool.component_size;
        void *array = set->array;

        int32 j;
        for(j = 0; j<count; j++){
            void *position = ((int8 *)array) + j * c_size;

            pool.on_update(position);
        }
    }
}

void register_component(ComponentType type, int32 size,
    OnUpdate on_update,
    OnDestroy on_destroy
){
    if(size == 0 || type == COMPONENT_NONE) return;

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE){
        pool.type = type;
        pool.component_size = size;

        pool.sparse_set = sparse_set_new(size);

        pool.on_update = on_update;
        pool.on_destroy = on_destroy;

        pools[type] = pool;//podia ter usado o ponteiro mas acho que assim é mais rapido eu acho(?)
    }

    pool = pools[type];
}

ComponentHandler add_component(uint32 entity_id, ComponentType type, void* component){
    if(type == COMPONENT_NONE) return (ComponentHandler){};

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE) return (ComponentHandler){};

    sparse_set_add(pool.sparse_set, entity_id, component);

    return (ComponentHandler){entity_id, type};
}

void get_component(ComponentHandler handler, void* component){
    ComponentType type = handler.type;
    if(type == COMPONENT_NONE) return;

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE) return;

    SparseSet *set = pool.sparse_set;

    uint32 entity_id = handler.entity_id;
    sparse_set_get(set, entity_id, component);
}

void destroy_component(ComponentHandler handler){
    ComponentType type = handler.type;
    if(type == COMPONENT_NONE) return;

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE) return;
    
    SparseSet *set = pool.sparse_set;

    uint32 entity_id = handler.entity_id;
    int8 component[pool.component_size];
    sparse_set_get(set, entity_id, &component);
    pool.on_destroy(&component);     
    
    sparse_set_rem(set, entity_id);
}

