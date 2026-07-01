#ifndef COMPONENTS_H
#define COMPONENTS_H

#define NO_PARENT 0

#include "../types.h"

#include "../collections/sparse_set.h"
#include "../collections/array_list.h"
#include "../math/vectors.h"
#include "../math/matrices.h"
#include "../mesh.h"

typedef void(* OnUpdate)(void *);
typedef void(* OnDestroy)(void *);

typedef enum{
    COMPONENT_NONE,
    COMPONENT_RENDERER,
    COMPONENT_RIGIBODY,

    COMPONENT_COUNT
}ComponentType;

typedef struct{
    uint32 entity_id;
    ComponentType type;
}ComponentHandler;

typedef struct{
    ComponentType type;
    int32 component_size;
    SparseSet *sparse_set;

    OnUpdate on_update;
    OnDestroy on_destroy;
}ComponentPool;

void init_components();
void update_components();

void register_component(ComponentType type, int32 size, OnUpdate on_update, OnDestroy on_destroy);
ComponentHandler add_component(ComponentType type, uint32 entity_id, void* component);
void get_component(ComponentHandler handler, void* component);
void destroy_component(ComponentHandler handler);


#endif
