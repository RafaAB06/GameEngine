#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_NO_TARGET 0

#include "../types.h"

#include "../math/matrices.h"
#include "../math/mathf.h"

typedef struct{
    vec3 position;
    vec3 rotation;
    
    float near_plane;
    float far_plane;
    float FOV;
    
    mat4 projection;
    int32 projection_dirty;

    mat4 view;
    int32 view_dirty;

    uint32 target_id;
}Camera;

void set_camera_position(vec3 position);
void set_camera_rotation(vec3 rotation);
void translate_camera(vec3 translation);
void rotate_camera(vec3 angles);

void set_camera_fov(float fov);
void set_camera_near(float near_plane);
void set_camera_far(float far_plane);

void set_camera_target(uint32 target_id);
void camera_clean_target();

mat4 get_camera_projection();
mat4 get_camera_view();

#endif