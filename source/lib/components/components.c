#include <stdlib.h>
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

static int32 go_comparer(void *a_ptr, void *b_ptr){
    GameObject a = *(GameObject *)a_ptr;
    GameObject b = *(GameObject *)b_ptr;

    return (a.depth < b.depth) - (a.depth > b.depth);
}

static int32 go_pool_dirty;
static ComponentPool go_pool;
static ID_Generator *go_id_gen;
static ComponentPool pools[COMPONENT_COUNT];
static int32 pool_count;

void init_components(){
    go_id_gen = id_generator_new(1u);

    go_pool.sparse_set = sparse_set_new(sizeof(GameObject));
}

void update_components(){
    if(go_pool_dirty){
        go_pool_dirty = 0;

        sparse_set_sort(go_pool.sparse_set, go_comparer);
    }

    int32 i = COMPONENT_NONE + 1;
    for(i; i < COMPONENT_COUNT; i++){
        ComponentPool pool = pools[i];
        if(pool.type == COMPONENT_NONE) continue;

        SparseSet *set = pool.sparse_set;
        int32 count = set->count;
        int32 c_size = pool.component_size;
        void *array = set->array;

        //printf("%d\n", count);

        int32 j;
        for(j = 0; j<count; j++){
            void *position = ((int8 *)array) + j * c_size;

            pool.on_update(position);
        }
    }
}

void game_object_new(GameObject *obj, const char *name, vec3 position, vec3 rotation, vec3 scale, uint32 parent_id){
    uint32 id = next_id(go_id_gen);
    obj->id = id;

    int32 len = strlen(name);
    char *str = (char *)malloc(len + 1);
    strcpy(str, name);
    obj->name = str;

    obj->position = position;
    obj->rotation = rotation;
    obj->scale = scale;

    GameObject parent;
    int32 sucess = get_game_object(parent_id, &parent);
    uint32 depth = 0;
    if(sucess){
        uint32 parent_depth = parent.depth;
        depth = parent_depth + 1;

        array_list_add(parent.children_ids, &id);
    }
    obj->depth = depth;
    obj->parent = parent_id;
    obj->children_ids = array_list_new(sizeof(uint32));

    SparseSet *set = go_pool.sparse_set;
    sparse_set_add(set, id, obj);

    go_pool_dirty = 1;
}

void destroy_game_object(uint32 go_id){
    int32 i = COMPONENT_NONE + 1;
    for(i; i<COMPONENT_COUNT; i++){
        destroy_component(i, go_id);
    }
    GameObject obj;
    get_game_object(go_id, &obj);

    uint32 parent_id = obj.parent;
    GameObject parent;
    int32 sucess = get_game_object(parent_id, &parent);
    if(sucess){
        array_list_remove(parent.children_ids, &parent_id, NULL);
    }
    free(obj.name);
    sparse_set_rem(go_pool.sparse_set, go_id);

    go_pool_dirty = 1;
}

int32 update_game_object(uint32 id, GameObject *obj){
    return sparse_set_set(go_pool.sparse_set, id, obj);
}

int32 get_game_object(uint32 id, GameObject *obj){
    return sparse_set_get(go_pool.sparse_set, id, obj);
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

int32 add_component(ComponentType type, uint32 go_id, void* component){
    if(type == COMPONENT_NONE) return 0;

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE) return 0;

    return sparse_set_add(pool.sparse_set, go_id, component);
}

int32 destroy_component(ComponentType type, uint32 go_id){
    if(type == COMPONENT_NONE) return 0;

    ComponentPool pool = pools[type];

    if(pool.type == COMPONENT_NONE) return 0;
    
    SparseSet *set = pool.sparse_set;
        
    Renderer renderer;
    sparse_set_get(set, go_id, &renderer);
    pool.on_destroy(&renderer);     
    
    sparse_set_rem(set, go_id);

    return 1;
}

