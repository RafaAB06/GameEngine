#ifndef COMPONENTS_H
#define COMPONENTS_H

#define NO_PARENT 0

#include "../types.h"

#include "../collections/sparse_set.h"
#include "../collections/array_list.h"
#include "../math/vectors.h"
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
    char *name;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    uint32 id;
    uint32 parent;
    uint32 depth;

    ArrayList *children_ids;
}GameObject;

typedef struct{
    ComponentType type;
    int32 component_size;
    SparseSet *sparse_set;

    OnUpdate on_update;
    OnDestroy on_destroy;
}ComponentPool;

void init_components();
void update_components();

void game_object_new(GameObject *obj, const char *name, vec3 position, vec3 rotation, vec3 scale, uint32 parent_id);
void destroy_game_object(uint32 go_id);
int32 update_game_object(uint32 id, GameObject *obj);
int32 get_game_object(uint32 id, GameObject *obj);

void register_component(ComponentType type, int32 size, OnUpdate on_update, OnDestroy on_destroy);
int32 add_component(ComponentType type, uint32 go_id, void* component);
int32 destroy_component(ComponentType type, uint32 go_id);


#endif
