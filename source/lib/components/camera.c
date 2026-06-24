#include <stdio.h>
#include <math.h>

#include "../math/mathf.h"
#include "../math/matrices.h"
#include "../screen.h"
#include "camera.h"
#include "components.h"

mat4 get_camera_projection(Camera *camera){
    float f = 1/tanf(deg2rad(camera->FOV)/2.0f); 
    float fa = f/(screen_aspect_ratio);

    float far = camera->far_plane;
    float near = camera->near_plane;
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

void set_camera_target(Camera *camera, uint32 target_id){
    camera->target_id = target_id;
}

mat4 get_camera_view(Camera *camera){
    if(camera->target_id){
        vec3 cam_pos = camera->position;

        GameObject obj;
        get_game_object(camera->target_id, &obj);
        vec3 target_position = obj.position;

        mat4 t = mat4_translate(MAT4_IDENTITY, vec3_mul(cam_pos, -1.0f));
        mat4 rot = mat4_look_at(cam_pos, target_position, VEC3_UP);
        mat4 rot_transposed = mat4_transpose(rot);
        mat4 view = mat4_mul(rot_transposed, t);

        return view;
    }

    vec3 position = camera->position;
    position = vec3_mul(position, -1.0f);
    mat4 translation = mat4_translate(MAT4_IDENTITY, position);

    mat4 rotation = mat4_rotate(MAT4_IDENTITY, camera->rotation);
    rotation = mat4_transpose(rotation);

    return mat4_mul(rotation, translation);
}
