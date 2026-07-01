#include <stdio.h>
#include <math.h>

#include "../math/mathf.h"
#include "../math/mathf.h"
#include "../math/matrices.h"
#include "../screen.h"
#include "../rendering.h"
#include "../entities.h"
#include "camera.h"
#include "components.h"

void set_camera_position(vec3 position){
    main_camera.position = position;
    main_camera.view_dirty = 1;
}

void set_camera_rotation(vec3 rotation){
    main_camera.rotation = rotation;
    main_camera.view_dirty = 1;
}

void translate_camera(vec3 translation){
    main_camera.position = vec3_add(main_camera.position, translation);
    main_camera.view_dirty = 1;
}

void rotate_camera(vec3 angles){
    main_camera.rotation = vec3_add(main_camera.rotation, angles);
    main_camera.view_dirty = 1;
}

void set_camera_fov(float fov){
    main_camera.FOV = clampf(fov, 1.0f, 179.0f);
    main_camera.projection_dirty = 1;
}

void set_camera_near(float near_plane){
    main_camera.near_plane = (near_plane < 0.01f) ? 0.01f : near_plane;
    main_camera.projection_dirty = 1;
}

void set_camera_far(float far_plane){
    if (far_plane < main_camera.near_plane + 0.1f)
        far_plane = main_camera.near_plane + 0.1f;

    main_camera.far_plane = far_plane;
    main_camera.projection_dirty = 1;
}

void set_camera_target(uint32 target_id){
    main_camera.target_id = target_id;
    main_camera.view_dirty = 1;
}

void camera_clean_target(){
    main_camera.target_id = 0;
    main_camera.view_dirty = 1;
}

mat4 get_camera_projection(){
    float f = 1/tanf(deg2rad(main_camera.FOV)/2.0f);
    float fa = f/(screen_aspect_ratio);

    float far = main_camera.far_plane;
    float near = main_camera.near_plane;

    float y = (far + near)/(near - far);
    float z = (2 * far * near)/(near - far);

    mat4 projection = { .m = {
        fa, 0, 0, 0,
         0, f, 0, 0,
         0, 0, y,-1,
         0, 0, z, 0
    }};

    return projection;
}

mat4 get_camera_view(){
    uint32 target_id = main_camera.target_id;
    if(target_id){
        vec3 cam_pos = main_camera.position;

        Transform target;
        get_entity_data(target_id, ENTITY_TRANSFORM, &target);
        vec3 target_position = target.position;
        
        mat4 t = mat4_translate(MAT4_IDENTITY, vec3_mul(cam_pos, -1.0f));
        mat4 rot = mat4_look_at(cam_pos, target_position, VEC3_UP);
        mat4 rot_transposed = mat4_transpose(rot);

        return mat4_mul(rot_transposed, t);
    }

    vec3 position = vec3_mul(main_camera.position, -1.0f);
    mat4 translation = mat4_translate(MAT4_IDENTITY, position);

    mat4 rotation = mat4_rotate(MAT4_IDENTITY, main_camera.rotation);
    rotation = mat4_transpose(rotation);

    return mat4_mul(rotation, translation);
}

vec3 get_camera_eye(){
    return vec3_rot_dir(main_camera.rotation);
}