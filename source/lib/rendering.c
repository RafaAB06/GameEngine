#include <stdio.h>

#include "../dependencies/glad.h"

#include "material.h"
#include "rendering.h"
#include "engine.h"
#include "input.h"
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

static Camera cam = {};
static DirectionalLight main_light = {
    (vec3){0, 10, 0}, (vec3){-45, 45, 0}, (vec4){1, 0, 0, 1}, 1
};
static uint32 target = 0;

void init_rendering(){
    glEnable(GL_CULL_FACE);

    cam.near_plane = 0.01f;
    cam.far_plane = 100.0f;
    cam.FOV = 90.0f;
    vec3 pos = {0, 0, 2};
    cam.position = pos;
    vec3 rot = {0, 0, 0};
    cam.rotation = rot;
    
    materials = array_list_new(sizeof(Material));
    renderers = array_list_new(sizeof(Renderer));

    vec3 position = {0, 0, 0};
    vec3 rotation = {0, 0, 0};
    vec3 scale = VEC3_ONE;
    GameObject obj;
    game_object_new(&obj, "MyGameObject", position, rotation, scale, NO_PARENT);
    
    vec4 color;
    ShaderProgram program = program = create_shader("unlit_vert", "unlit_frag");
    Material mat = create_material(program, 2);

    Texture2D tex = create_texture("peixe");
    material_add_texture_uniform(&mat, tex);
    UniformValue value = { .vec4_value = {0.5f, 1.0f, 0.0f, 1.0f} };
    material_add_uniform(&mat, "base_color", UNIFORM_VEC4, value);

    register_material(mat);

    Mesh *mesh = create_mesh("cube");
    
    register_component(COMPONENT_RENDERER, sizeof(Renderer), renderer_on_update, renderer_on_destroy);
    
    Renderer renderer;
    renderer_new(&renderer, obj.id, mesh, mat);
    add_component(COMPONENT_RENDERER, obj.id, &renderer);

    set_camera_target(&cam, obj.id);
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
            
            //main light
            int32 u_ml_pos = glGetUniformLocation(current_program, "u_main_light.position");
            int32 u_ml_dir = glGetUniformLocation(current_program, "u_main_light.direction");
            int32 u_ml_col = glGetUniformLocation(current_program, "u_main_light.color");
            int32 u_ml_int = glGetUniformLocation(current_program, "u_main_light.intensity");

            vec3 position = main_light.position;
            glUniform3fv(u_ml_pos, 1, &position.x);
            vec3 direction = vec3_rot_dir(main_light.rotation);
            glUniform3fv(u_ml_dir, 1, &direction.x);
            vec4 color = main_light.color;
            glUniform4fv(u_ml_col, 1, &color.x);
            float intensity = main_light.intensity;
            glUniform1f(u_ml_int, intensity);

            mat4 projection = get_camera_projection(&cam);
            int32 u_projection = glGetUniformLocation(current_program, "u_projection");
            glUniformMatrix4fv(u_projection, 1, GL_FALSE, projection.m);
            
            mat4 view = get_camera_view(&cam);
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

            uint32 go_id = renderer.go_id;
            GameObject obj;
            get_game_object(go_id, &obj);

            vec3 position = obj.position;
            vec3 rotation = obj.rotation;
            vec3 scale = obj.scale;

            position.y = sin(get_time())*5;
            obj.position = position;
            rotation.y += 25.0f * get_delta_time();
            rotation.z += 25.0f * get_delta_time();
            obj.rotation = rotation;
            update_game_object(go_id, &obj);
            
            mat4 model = mat4_trs(position, rotation, scale);
            int32 model_u_loc = glGetUniformLocation(current_program, "u_model");
            glUniformMatrix4fv(model_u_loc, 1, GL_FALSE, model.m);

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
                vec3 vec = value.vec3_value;
                glUniform3fv(id, 1, (float *)&vec);
            break;
            case UNIFORM_VEC4:
                vec4 vec = value.vec4_value;
                glUniform4fv(id, 1, (float *)&vec);
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
    array_list_add(renderers, renderer);
    renderers_is_dirty = 1;
}

void remove_renderer(Renderer *renderer){//trocar por busca binária, visto que estão ordenados na lista
    array_list_remove(renderers, renderer, NULL);
    renderers_is_dirty = 1;
}