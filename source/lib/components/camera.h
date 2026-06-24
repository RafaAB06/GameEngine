#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_NO_TARGET 0

#include "../types.h"

#include "../math/matrices.h"
#include "../math/mathf.h"

typedef struct{

    float near_plane;
    float far_plane;
    float FOV;
    
    mat4 projection;
    int32 projection_dirty;

    vec3 position;
    vec3 rotation;

    mat4 view;
    int32 view_dirty;

    uint32 target_id;
}Camera;

void set_camera_target(Camera *camera, uint32 target_id);
mat4 get_camera_projection(Camera *camera);
mat4 get_camera_view(Camera *camera);

#endif