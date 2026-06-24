#ifndef MATRICES_H
#define MATRICES_H

#include "../types.h"
#include "vectors.h"

#define MAT4_IDENTITY (mat4){ .m = {\
    1, 0, 0, 0,\
    0, 1, 0, 0,\
    0, 0, 1, 0,\
    0, 0, 0, 1\
}}

typedef struct{
    float m[16];
}mat4;

void mat4_print(mat4 mat);

mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up);
mat4 mat4_transpose(mat4 m);

mat4 mat4_translate(mat4 m, vec3 v3);
mat4 mat4_rotate(mat4 m, vec3 v3);
mat4 mat4_scale(mat4 m, vec3 v3);
mat4 mat4_trs(vec3 position, vec3 rotation, vec3 scale);

vec4 mat4_vec4(mat4 a, vec4 v);
mat4 mat4_mul_s(mat4 a, float s);
mat4 mat4_add(mat4 a, mat4 b);
mat4 mat4_sub(mat4 a, mat4 b);
mat4 mat4_mul(mat4 a, mat4 b);

#endif
