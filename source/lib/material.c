#include "../dependencies/glad.h"

#include "rendering.h"
#include "file_stream.h"
#include "material.h"
#include "engine.h"
#include "types.h"

#include "components/id_generator.h"

#include <stdio.h>
#include <stdlib.h>

static ID_Generator *mat_id_gen = NULL;


void init_material(){
    mat_id_gen = id_generator_new(1u);
}

Material create_material(ShaderProgram program, int32 uniform_count){
    Material material;
    material.program = program;
    material.uniforms = (Uniform *)malloc(uniform_count * sizeof(Uniform));
    material.uniform_count = 0;
    material.uniform_capacity = uniform_count;

    uint32 id = next_id(mat_id_gen);
    material.id = id;

    return material;
}

ShaderProgram create_shader(const char *vert_name, const char *frag_name){
    ShaderProgram program = {};

    uint32 vert = load_shader(vert_name, GL_VERTEX_SHADER);
    uint32 frag = load_shader(frag_name, GL_FRAGMENT_SHADER);

    uint32 program_id = glCreateProgram();
    program.gl_id = program_id;

    glAttachShader(program_id, vert);
    glAttachShader(program_id, frag);
    glLinkProgram(program_id);

    int32 success;
    char infoLog[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        printf("Erro creating shader program %u!\n", program_id);
    }
    return program;
}

void material_add_texture_uniform(Material *material, Texture2D texture){
    int32 capacity = material->uniform_capacity;
    int32 count = material->uniform_count;

    if(capacity == count){
        printf("Can't add more uniforms!\n");
        return;
    }

    UniformValue value;
    value.tex_value = texture;
    Uniform uniform;
    uniform.type = UNIFORM_TEXTURE;
    uniform.value = value;
    uniform.id = -1;

    material->uniforms[count] = uniform;
    material->uniform_count++;
}

void material_add_uniform(Material *material, const char *name, UniformType type, UniformValue value){
    if(type == UNIFORM_TEXTURE){
        material_add_texture_uniform(material, value.tex_value);
        return ;
    }
    int32 capacity = material->uniform_capacity;
    int32 count = material->uniform_count;

    if(capacity == count){
        printf("Can't add more uniforms! UniformType: %d\n", type);
        return;
    }
    Uniform uniform;
    uniform.type = type;
    uniform.value = value;
    
    uint64 program_id = material->program.gl_id;
    int64 id = glGetUniformLocation(program_id, name);
    if(id == -1){
        printf("No uniform is named %s on program %d!\n", name, program_id);
        return;
    }
    uniform.id = id;

    material->uniforms[count] = uniform;
    material->uniform_count++;
}

uint32 load_shader(const char *name, int64 type){
    char path[512];
    sprintf(path, "shaders\\%s.shader", name);

    const char *shaderSource = read_all_bytes(path);

    if(!shaderSource){
        printf(
            "Failed to load shader: %s - %s",
            type == GL_VERTEX_SHADER ? "VERTEX" : "FRAG",
            path
        );
    }

    unsigned int shader;
    shader = glCreateShader(type);

    glShaderSource(shader, 1, &shaderSource, (void *)0);
    glCompileShader(shader);
    free((void *)shaderSource);

    int32  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("%s - %s", path, infoLog);
    }

    return shader;
}

Texture2D create_texture(const char *name)
{ // sem mipmaps, tudo sem filtro, perfeito
    char path[512];
    sprintf(path, "textures/%s.texture", name);

    Texture2D texture = {};
    FILE *file = fopen(path, "rb");

    if (file)
    {
        struct texture_metadata{
            int32 width, height, channels;
        };
        struct texture_metadata metadata;
        fread(&metadata, sizeof(struct texture_metadata), 1, file);

        int32 bytes_count = metadata.width * metadata.height * metadata.channels;
        int8 *bytes = malloc(bytes_count);
        fread(bytes, 1, bytes_count, file);

        GLenum format;
        switch (metadata.channels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            printf("Unsupported texture format\n");
            return texture;
        }

        uint32 gl_tex;
        glGenTextures(1, &gl_tex);
        glBindTexture(GL_TEXTURE_2D, gl_tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            metadata.width,
            metadata.height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            bytes
        );

        texture.gl_texture = gl_tex;
        texture.width = metadata.width;
        texture.height = metadata.height;
        texture.channels = metadata.channels;

        free(bytes);
    }else{
        printf("Failed to load texture: %s", name);
        return texture;
    }
    return texture;
}

int32 material_comparator(void *a, void *b){
    ShaderProgram prog_1 = (*(Material *)a).program;
    ShaderProgram prog_2 = (*(Material *)b).program;

    return (prog_1.gl_id > prog_2.gl_id) - (prog_1.gl_id < prog_2.gl_id);
}