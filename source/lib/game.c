#include <stdio.h>
#include <math.h>

#include "game.h"
#include "time.h"
#include "mesh.h"
#include "material.h"
#include "rendering.h"
#include "input.h"
#include "components/renderer.h"
#include "components/camera.h"
#include "components/components.h"

#include "entities.h"

void game_awake(){
    // GameObject obj2;
    uint32 obj1 = entity_new("MyGameObject", VEC3_ZERO, VEC3_ZERO, VEC3_ONE, NO_PARENT);
    //entity(&obj2, "Another", (vec3){2, 0, 0}, (vec3){0, 45, 45}, VEC3_ONE, obj1.id);
    
    vec4 color;
    ShaderProgram program = program = create_shader("unlit_vert", "unlit_frag");
    Material mat = create_material(program, 2);

    Texture2D tex = create_texture("peixe");
    material_add_texture_uniform(&mat, tex);
    UniformValue value = { .vec4_value = VEC4_ONE };
    material_add_uniform(&mat, "base_color", UNIFORM_VEC4, value);

    register_material(mat);

    Mesh *mesh = create_mesh("cube");
    
    register_component(COMPONENT_RENDERER, sizeof(Renderer), renderer_on_update, renderer_on_destroy);
    
    Renderer renderer1;
    renderer_new(&renderer1, obj1, mesh, mat);
    add_component(COMPONENT_RENDERER, obj1, &renderer1);

    // Renderer renderer2;
    // renderer_new(&renderer2, obj2, mesh, mat);
    // add_component(COMPONENT_RENDERER, obj2.id, &renderer2);

    set_camera_position((vec3){0, 0, 4});
}
int a = 1;
void game_update(){
    vec3 rotation = {};
    if(get_key_down(KEY_W)){
        rotation.x = -1;
    }
    if(get_key_down(KEY_S)){
        rotation.x = 1;
    }
    if(get_key_down(KEY_D)){
        rotation.y = 1;
    }
    if(get_key_down(KEY_A)){
        rotation.y = -1;
    }
    if(get_key_down(KEY_E)){
        rotation.z = 1;
    }
    if(get_key_down(KEY_Q)){
        rotation.z = -1;
    }
    rotation = vec3_mul(rotation, get_delta_time() * 45);

    entity_rotate(0, rotation);
}

void game_shutdown(){
    
}