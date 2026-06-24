#ifndef MESH_PROCESSOR_H
#define MESH_PROCESSOR_H

#include "types.h"

typedef struct{
    float x, y, z;
}vec3;

typedef struct{
    float x, y;
}vec2;

typedef struct{
    vec3 position;
    vec3 normal;
    vec2 uv;
}Vertex;

void process_meshes();

#endif