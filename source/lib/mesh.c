#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../dependencies/glad.h"

#include "file_stream.h"
#include "rendering.h"
#include "mesh.h"

typedef struct{
    char magic[20];
    int32 version;
}MeshHeader;

// static Vertex vertices[] = {
//     { { -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
//     { {  1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
//     { {  1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
//     { { -1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }
// };
// static uint32 indices[] = {
//     0, 1, 2,
//     2, 3, 0
// };

Mesh * load_mesh(const char *name){
    char real_path[512];
    sprintf(real_path, "meshes/%s.mesh", name);

    Mesh *mesh = malloc(sizeof(Mesh));

    FILE *file = fopen(real_path, "rb");

    MeshHeader header;
    fread(&header, sizeof(MeshHeader), 1, file);

    if(strcmp(header.magic, "MESH")){
        printf("%s is not a mesh!\n", name);

        return NULL;
    }

    if(header.version == 1){
        int32 vertex_count;
        Vertex *vertices = read_array(file, sizeof(Vertex), &vertex_count);

        int32 index_count;
        uint32 *indices = read_array(file, sizeof(uint32), &index_count);

        int32 i;

        mesh->vertex_count = vertex_count;
        mesh->vertices = vertices;
        mesh->index_count = index_count;
        mesh->indices = indices;
    }else{
        printf("Invalid mesh version at %s!\n", name);
        free(mesh);

        return NULL;
    }
    fclose(file);

    return mesh;
}

Mesh *create_mesh(const char *path){
    Mesh *mesh = load_mesh(path);
    if(!mesh) return NULL;

    uint32 VAO;
    uint32 VBO;
    uint32 EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertex_count, mesh->vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * mesh->index_count, mesh->indices, GL_STATIC_DRAW);    

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, normal)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, uv)
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    mesh->VAO = VAO;
    mesh->VBO = VBO;
    mesh->EBO = EBO;

    return mesh;
}

void destroy_mesh(Mesh *mesh){
    glDeleteBuffers(1, &mesh->EBO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->VAO);

    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
};