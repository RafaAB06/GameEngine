#include "entities.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "components/id_generator.h"

static ID_Generator *gen;
static EntityPool *pool;

static int32 entity_comparer(void *a_ptr, void *b_ptr){
    uint32 a = *(uint32 *)a_ptr;
    uint32 b = *(uint32 *)b_ptr;

    uint32 depth_a = 0;
    uint32 depth_b = 0;

    return (depth_a > depth_b) - (depth_a < depth_b);
}

void init_entities(){
    gen = id_generator_new(0);

    pool = malloc(sizeof(EntityPool));
    pool->count = 0;
    
    memset(pool->sparse, INVALID_ENTITY_INDEX, sizeof(pool->sparse));
    memset(pool->dense, INVALID_ENTITY_ID, sizeof(pool->dense));
}

void update_entities(){
    uint32 count = pool->count, i;  
    for(i = 0; i < count; i++){//id iniciam em 1 né
        uint32 dirty = pool->dirties[i];
        if(dirty){
            Transform *transform = &pool->transforms[i];
        
            mat4 local_matrix = mat4_trs(transform->position, transform->rotation, transform->scale);
            pool->world_matrices[i] = local_matrix;

            pool->dirties[i] = 0;
        }
    }
}

uint32 entity_new(const char *name, vec3 position, vec3 rotation, vec3 scale, uint32 parent_id){
    int32 count = pool->count;
    if(count == MAX_ENTITIES) {printf("MAX ENTITIES LIMIT REACHED!!!!!"); return 0;}
    int32 index = count;
    uint32 id = next_id(gen);

    uint32 *sparse = pool->sparse;
    uint32 *dense = pool->dense;

    sparse[id] = index;
    dense[index] = id;

    Transform transform = {
        .position = position,
        .rotation = rotation,
        .scale = scale,
        .parent = parent_id,
        .depth = 0,
    };
    
    if(parent_id){
        Transform parent_tranform;
        get_entity_data(parent_id, ENTITY_TRANSFORM, &parent_tranform);

        transform.depth = parent_tranform.depth + 1;

        ArrayList *parent_children;
        get_entity_data(parent_id, ENTITY_CHILDREN, &parent_children);
        array_list_add(parent_children, &id);
    }

    mat4 local = MAT4_IDENTITY;
    mat4 world = MAT4_IDENTITY;

    ArrayList *children = array_list_new(sizeof(uint32));

    int32 len = strlen(name);
    char *name_str = (char *)malloc(len + 1);
    strcpy(name_str, name);

    pool->transforms[index] = transform;
    pool->local_matrices[index] = local;
    pool->world_matrices[index] = world;
    pool->children_lists[index] = children;
    pool->names[index] = name_str;
    pool->dirties[index] = 1u;
    pool->count++;

    return id;
}

void entity_destroy(uint32 id){
    uint32 *sparse = pool->sparse;
    uint32 *dense = pool->dense;

    uint32 index = sparse[id];
    if(index == INVALID_ENTITY_INDEX) return;

    uint32 last_index = pool->count - 1;
    if(last_index){
        uint32 last_id = dense[last_index];

        pool->transforms[index] = pool->transforms[last_index];

        sparse[id] = INVALID_ENTITY_INDEX;
        sparse[last_id] = index;
        dense[index] = last_id;
    }else{
        sparse[id] = INVALID_ENTITY_INDEX;
    }

    pool->count--;
    add_unused_id(gen, id);
}

void get_entity_data(uint32 id, EntityDataType type, void *data){
    uint32 *sparse = pool->sparse;
    uint32 index = sparse[id];
    if(index == INVALID_ENTITY_INDEX) return;

    void* ptr;
    uint32 size;

    switch(type){
        case ENTITY_TRANSFORM:
            ptr = &(pool->transforms[index]);
            size = sizeof(Transform);
            break;

        case ENTITY_LOCAL:
            ptr = &(pool->local_matrices[index]);
            size = sizeof(mat4);
            break;

        case ENTITY_WORLD:
            ptr = &(pool->world_matrices[index]);
            size = sizeof(mat4);
            break;

        case ENTITY_CHILDREN:
            ptr = &(pool->children_lists[index]);
            size = sizeof(ArrayList *);
            break;

        case ENTITY_NAME:
            ptr = &(pool->names[index]);
            size = sizeof(char *);
            break;
            
        case ENTITY_DIRTY:
            ptr = &(pool->dirties[index]);
            size = sizeof(uint32);
            break;

        default:
            printf("????????\n");
        return;
    }

    memcpy(data, ptr, size);
}

void set_entity_data(uint32 id, EntityDataType type, void *data){
    uint32 *sparse = pool->sparse;
    uint32 index = sparse[id];
    if(index == INVALID_ENTITY_INDEX) return;
    
    void* ptr;
    uint32 size;

    switch(type){
        case ENTITY_TRANSFORM:
            ptr = &(pool->transforms[index]);
            size = sizeof(Transform);
            break;

        case ENTITY_LOCAL:
            ptr = &(pool->local_matrices[index]);
            size = sizeof(mat4);
            break;

        case ENTITY_WORLD:
            ptr = &(pool->world_matrices[index]);
            size = sizeof(mat4);
            break;

        case ENTITY_CHILDREN:
            ptr = &(pool->children_lists[index]);
            size = sizeof(ArrayList *);
            break;

        case ENTITY_NAME:
            ptr = &(pool->names[index]);
            size = sizeof(char *);
            break;
        
        case ENTITY_DIRTY:
            ptr = &(pool->dirties[index]);
            size = sizeof(uint32);
        break;

        default:
            printf("????????\n");
        return;
    }

    set_dirty(id);
    memcpy(ptr, data, size);
}

void entity_translate(uint32 id, vec3 translation){
    uint32 index = pool->sparse[id];
    if(index == INVALID_INDEX) return;

    Transform *transform = &pool->transforms[index];
    transform->position = vec3_add(transform->position, translation);

    set_dirty(id);
}

void entity_rotate(uint32 id, vec3 angles){
    uint32 index = pool->sparse[id];
    if(index == INVALID_INDEX) return;

    Transform *transform = &pool->transforms[index];
    transform->rotation = vec3_add(transform->rotation, angles);

    set_dirty(id);
}

void entity_scale(uint32 id, vec3 factor){
    uint32 index = pool->sparse[id];
    if(index == INVALID_INDEX) return;

    Transform *transform = &pool->transforms[index];
    transform->scale = vec3_add(transform->scale, factor);

    set_dirty(id);
}

INLINE void set_dirty_recursive(uint32 id, uint32 *sparse, uint32 *dense){
    uint32 index = sparse[id];
    if(index == INVALID_INDEX) return;

    pool->dirties[index] = 1;

    ArrayList *children;
    get_entity_data(id, ENTITY_CHILDREN, &children);
    int32 count = children->count;
    if(count){
        int32 i;
        for(i = 0; i<count; i++){
            uint32 child_id;

            array_list_get(children, &child_id, i);
            set_dirty_recursive(child_id, sparse, dense);
        }
    }
}

void set_dirty(uint32 id){
    uint32 *sparse = pool->sparse;
    uint32 *dense = pool->dense;

    set_dirty_recursive(id, sparse, dense);
}

void sort_entities(){

}