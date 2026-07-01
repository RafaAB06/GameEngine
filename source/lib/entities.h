#ifndef ENTITIES_H
#define ENTITIES_H
#define MAX_ENTITIES 256

#define INVALID_ENTITY_INDEX 0xFFFFFFFF
#define INVALID_ENTITY_ID 0xFFFFFFFF

#include "math/vectors.h"
#include "math/matrices.h"
#include "types.h"

#include "components/components.h"

typedef enum{
    ENTITY_TRANSFORM,
    ENTITY_LOCAL,
    ENTITY_WORLD,
    ENTITY_CHILDREN,
    ENTITY_NAME,
    ENTITY_DIRTY
}EntityDataType;

typedef struct{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    uint32 parent;
    uint32 depth;
}Transform;

typedef struct{
    uint32 sparse[MAX_ENTITIES];
    uint32 dense[MAX_ENTITIES];

    Transform transforms[MAX_ENTITIES];

    mat4 local_matrices[MAX_ENTITIES];
    mat4 world_matrices[MAX_ENTITIES];

    ArrayList *children_lists[MAX_ENTITIES];
    char *names[MAX_ENTITIES];

    int32 dirties[MAX_ENTITIES];

    int32 count;
}EntityPool;

void init_entities();
void update_entities();

uint32 entity_new(const char *name, vec3 position, vec3 rotation, vec3 scale, uint32 parent_id);
void entity_destroy(uint32 id);

void get_entity_data(uint32 id, EntityDataType type, void *data);
void set_entity_data(uint32 id, EntityDataType type, void *data);

void entity_translate(uint32 id, vec3 translation);
void entity_rotate(uint32 id, vec3 angles);
void entity_scale(uint32 id, vec3 factor);

void set_dirty(uint32 id);
void sort_entities();

#endif