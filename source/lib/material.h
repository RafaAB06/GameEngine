#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vectors.h"
#include "math/matrices.h"
#include "types.h"

#define FRAGMENT_SHADER 0x8B30
#define VERTEX_SHADER 0x8B31

typedef struct{
    uint32 gl_texture;
    
    int32 width;
    int32 height;
    int32 channels;
}Texture2D;

typedef enum{
    UNIFORM_FLOAT,
    UNIFORM_INT,
    UNIFORM_VEC3,
    UNIFORM_VEC4,
    UNIFORM_mat4,
    UNIFORM_TEXTURE
}UniformType;

typedef union{
    float float_value;
    int32 int_value;
    vec3 vec3_value;
    vec4 vec4_value;
    mat4 mat4_value;
    Texture2D tex_value;
}UniformValue;

typedef struct{
    int64 id;
    UniformType type;
    UniformValue value;
}Uniform;

typedef struct{
    uint32 gl_id;
}ShaderProgram;

typedef struct{
    Uniform *uniforms;
    int32 uniform_count;
    int32 uniform_capacity;

    ShaderProgram program;
    uint32 id;
}Material;

void init_material();
Material create_material(ShaderProgram program, int32 uniform_count);
ShaderProgram create_shader(const char *vert_name, const char *frag_name);
void material_add_texture_uniform(Material *material, Texture2D texture);
void material_add_uniform(Material *material, const char *name, UniformType type, UniformValue value);
uint32 load_shader(const char *path, int64 type);
Texture2D create_texture(const char *name);
int32 material_comparator(void *a, void *b);

#endif