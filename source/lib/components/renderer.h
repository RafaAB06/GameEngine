#ifndef RENDERER_H
#define RENDERER_H

#include "../types.h"
#include "../material.h"
#include "../mesh.h"

typedef struct{
    Mesh *mesh;
    uint32 material_id;
    uint32 program_id;
    uint32 entity_id;
    uint32 model_location;
}Renderer;

void renderer_new(Renderer *self, uint32 entity_id,  Mesh *mesh, Material mat);
void renderer_on_update(void *self);
void renderer_on_destroy(void *self);

int32 renderer_comparator(void *a, void *b);

#endif