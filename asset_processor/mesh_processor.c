#include "mesh_processor.h"
#include "array_list.h"
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct{
    char magic[20];
    int32 version;
}MeshHeader;

typedef struct{
    int32 x, y, z;
}vec3int;

INLINE void process_position(ArrayList *positions, char *line){
    vec3 position;
    sscanf(line, "v %f %f %f", &position.x, &position.y, &position.z);

    array_list_add(positions, &position);
}

INLINE void process_normal(ArrayList *normals, char *line){
    vec3 normal;
    sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);

    array_list_add(normals, &normal);
}

INLINE void process_uv(ArrayList *uvs, char *line){
    vec2 uv;
    sscanf(line, "vt %f %f", &uv.x, &uv.y);

    uv.y = 1.0f - uv.y;

    array_list_add(uvs, &uv);
}

INLINE void process_face(
    ArrayList *vertices_list,
    ArrayList *indices_list,
    ArrayList *positions,
    ArrayList *uvs,
    ArrayList *normals,
    char *line
){
    Vertex temp[16];

    char *token = strtok(line, " ");

    int32 vert_count;
    for(vert_count = 0; (token = strtok(NULL, " ")) != NULL; vert_count++){
        vec3int obj_indices;

        sscanf(token, "%d/%d/%d",
            &obj_indices.x,
            &obj_indices.y,
            &obj_indices.z
        );

        obj_indices.x--;
        obj_indices.y--;
        obj_indices.z--;

        Vertex vert;

        array_list_get(positions, &vert.position, obj_indices.x);
        array_list_get(uvs, &vert.uv, obj_indices.y);
        array_list_get(normals, &vert.normal, obj_indices.z);

        temp[vert_count] = vert;
    }
    int32 tri_count = vert_count - 2, i;
    for(i = 0; i < tri_count; i++){
        int32 tri[3] = {
            0,
            i + 1,
            i + 2
        };

        int32 j;
        for(j = 0; j < 3; j++){
            Vertex v = temp[tri[j]];

            uint32 index = array_list_index_of(vertices_list, &v, NULL);

            if(index == INVALID_INDEX){
                index = vertices_list->count;
                array_list_add(vertices_list, &v);
            }
            array_list_add(indices_list, &index);
        }
    }
}

void parse_obj(char *path){
    FILE *obj_file = fopen(path, "r");

    ArrayList *positions = array_list_new(sizeof(vec3));
    ArrayList *uvs = array_list_new(sizeof(vec2));
    ArrayList *normals = array_list_new(sizeof(vec3));

    ArrayList *vertices_list = array_list_new(sizeof(Vertex));
    ArrayList *indices_list = array_list_new(sizeof(uint32));

    char buffer[512];
    while(fgets(buffer, sizeof(buffer), obj_file)){
        if(!strncmp(buffer, "o", 1)){
                //nada
        }else if(!strncmp(buffer, "v ", 2)){
            process_position(positions, buffer);
        }else if(!strncmp(buffer, "vn", 2)){
            process_normal(normals, buffer);
        }else if(!strncmp(buffer, "vt", 2)){
            process_uv(uvs, buffer);
        }else if(!strncmp(buffer, "f", 1)){
            process_face(vertices_list, indices_list, positions, uvs, normals, buffer);
        }else if(!strncmp(buffer, "s", 1)){
            
        }
    }
    fclose(obj_file);

    char build_path[512];
    sprintf(build_path, "..\\build\\%s", path);

    char *dot = strrchr(build_path, '.');
    strcpy(dot, ".mesh");

    FILE *mesh_file = fopen(build_path, "wb");
    MeshHeader header = {
        "MESH",
        1
    };
    fwrite(&header, sizeof(MeshHeader), 1, mesh_file);

    int32 vertex_count = vertices_list->count;
    fwrite(&vertex_count, sizeof(int32), 1, mesh_file);
    fwrite(vertices_list->array, sizeof(Vertex), vertex_count, mesh_file);

    int32 index_count = indices_list->count;
    fwrite(&index_count, sizeof(int32), 1, mesh_file);
    fwrite(indices_list->array, sizeof(uint32), index_count, mesh_file);

    fclose(mesh_file);

    printf("processed: %s -> %s\n", path, build_path);
}

void process_meshes(){
    char path[512] = "meshes\\*.obj";

    WIN32_FIND_DATA findData;
    HANDLE hFind = NULL;

    hFind = FindFirstFile(path, &findData);
    if (hFind != INVALID_HANDLE_VALUE){
        do
        {
            char file_path[512];
            sprintf(file_path, "meshes\\%s", findData.cFileName);
            parse_obj(file_path);
        }
        while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }
}