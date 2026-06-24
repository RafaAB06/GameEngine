#ifndef MESH_H
#define MESH_H

#include "math/vectors.h"
#include "types.h"

typedef struct{
    vec3 position;
    vec3 normal;
    vec2 uv;
}Vertex;

typedef struct{
    Vertex *vertices;
    int32 vertex_count;

    uint32 *indices;
    int32 index_count;

    uint32 VAO;
    uint32 VBO;
    uint32 EBO;
}Mesh;

Mesh * create_mesh(const char *path);
void destroy_mesh(Mesh *mesh);

#endif