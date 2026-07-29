#include <stdio.h>

#include "../dependencies/glad.h"

#include "material.h"
#include "entities.h"
#include "rendering.h"
#include "engine.h"
#include "types.h"
#include "mesh.h"
#include "time.h"

#include "components/directional_light.h"
#include "components/components.h"
#include "components/id_generator.h"
#include "components/renderer.h"
#include "components/camera.h"

#include "math/mathf.h"
#include "math/vectors.h"
#include "math/matrices.h"
#include <math.h>

#include "collections/array_list.h"

static ID_Generator *mat_id_gen;
static ArrayList *materials;
static int32 materials_is_dirty = 0;
static ArrayList *renderers;
static int32 renderers_is_dirty = 0;

Camera main_camera = {};
DirectionalLight main_light = {};

INLINE void init_main_camera(){
    main_camera.position = VEC3_ZERO;
    main_camera.rotation = VEC3_ZERO;

    main_camera.near_plane = 0.01f;
    main_camera.far_plane = 100.0f;
    main_camera.FOV = 90.0f;

    main_camera.projection_dirty = 1;
    main_camera.view_dirty = 1;
}

INLINE void init_main_light(){
    main_light.color = VEC4_ONE;
    main_light.intensity = 1.0f;
    main_light.rotation = (vec3){-45, 0, 0};
}

void init_rendering(){
    init_main_camera();
    init_main_light();

    vec3 pos = {0, 0, 2};
    main_camera.position = pos;
    vec3 rot = {0, 0, 0};
    main_camera.rotation = rot;
    
    materials = array_list_new(sizeof(Material));
    renderers = array_list_new(sizeof(Renderer));

    glEnable(GL_CULL_FACE);
}

void update_rendering(){    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(materials_is_dirty){
        materials_is_dirty = 0;

        array_list_sort(materials, material_comparator);
    }

    if(renderers_is_dirty){
        renderers_is_dirty = 0;

        array_list_sort(renderers, renderer_comparator);
    }  

    uint32 target_id = main_camera.target_id;
    if(target_id){
        uint32 dirty;
        get_entity_data(target_id, ENTITY_DIRTY, &dirty);
        main_camera.view_dirty = dirty;
    }
    if(main_camera.projection_dirty){
        main_camera.projection = get_camera_projection();
        main_camera.projection_dirty = 0;
    }
    if(main_camera.view_dirty){
        main_camera.view = get_camera_view();
        main_camera.view_dirty = 0;
    }

    int32 mat_count = materials->count;
    int32 renderer_count = renderers->count;
    int32 i, j;
    uint32 last_program = 0,last_material = 0;
    for(i = 0; i<mat_count; i++){
        Material mat;
        array_list_get(materials, &mat, i);

        uint32 current_program = mat.program.gl_id;
        if(current_program != last_program){
            last_program = current_program;
            
            glUseProgram(current_program);

            int32 u_time = glGetUniformLocation(current_program, "u_time");
            glUniform1f(u_time, get_time());
            
            int32 u_ml_dir = glGetUniformLocation(current_program, "u_main_light.direction");
            int32 u_ml_col = glGetUniformLocation(current_program, "u_main_light.color");
            int32 u_ml_int = glGetUniformLocation(current_program, "u_main_light.intensity");

            vec3 direction = vec3_rot_dir(main_light.rotation);
            glUniform3fv(u_ml_dir, 1, &direction.x);
            vec4 color = main_light.color;
            glUniform4fv(u_ml_col, 1, &color.x);
            float intensity = main_light.intensity;
            glUniform1f(u_ml_int, intensity);

            mat4 projection = main_camera.projection;
            int32 u_projection = glGetUniformLocation(current_program, "u_projection");
            glUniformMatrix4fv(u_projection, 1, GL_FALSE, projection.m);
            
            mat4 view = main_camera.view;
            int32 u_view = glGetUniformLocation(current_program, "u_view");
            glUniformMatrix4fv(u_view, 1, GL_FALSE, view.m);
        }

        uint32 current_material = mat.id;
        if(current_material != last_material){
            last_material = current_material;

            set_uniforms(mat);
        } 

        for(j = 0; j<renderer_count; j++){
            Renderer renderer;
            array_list_get(renderers, &renderer, j);
            
            Mesh mesh = *(renderer.mesh);

            uint32 entity_id = renderer.entity_id;
            mat4 world_matrix;
            get_entity_data(entity_id, ENTITY_WORLD, &world_matrix);
            
            glUniformMatrix4fv(renderer.model_location, 1, GL_FALSE, world_matrix.m);

            glBindVertexArray(renderer.mesh->VAO);
            glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
        }
    }

    glDisable(GL_DEPTH_TEST);
}

void set_uniforms(Material material){
    int32 i;
    int32 count = material.uniform_count;
    for(i = 0; i<count; i++){
        Uniform uniform = material.uniforms[i];
        int64 id = uniform.id;
        UniformType type = uniform.type;
        UniformValue value = uniform.value;

        switch(type){
            case UNIFORM_FLOAT:
                float float_v = value.float_value;
                glUniform1f(id, float_v);
            break;
            case UNIFORM_INT:
                int32 int_v = value.int_value;
                glUniform1i(id, int_v);
            break;
            case UNIFORM_VEC3:
                vec3 vec_3 = value.vec3_value;
                glUniform3fv(id, 1, (float *)&vec_3);
            break;
            case UNIFORM_VEC4:
                vec4 vec_4 = value.vec4_value;
                glUniform4fv(id, 1, (float *)&vec_4);
            break;
            case UNIFORM_TEXTURE:
                uint32 tex = value.tex_value.gl_texture;
                glBindTexture(GL_TEXTURE_2D, tex);
            break;
        }
    }
}

void register_material(Material material){
    array_list_add(materials, &material);
    materials_is_dirty = 1;
}

void add_renderer(Renderer *renderer){
    renderer->model_location = glGetUniformLocation(renderer->program_id, "u_model");

    array_list_add(renderers, renderer);
    renderers_is_dirty = 1;
}

void remove_renderer(Renderer *renderer){//trocar por busca binária, visto que estão ordenados na lista
    array_list_remove(renderers, renderer, NULL);
    renderers_is_dirty = 1;
}