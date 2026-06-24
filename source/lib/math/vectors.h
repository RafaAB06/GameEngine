#ifndef vecS_H
#define vecS_H

#include "../types.h"

#define VEC4_ZERO (vec4){}

#define VEC3_UP (vec3){ 0, 1, 0}
#define VEC3_DOWN (vec3){ 0, -1, 0}
#define VEC3_RIGHT (vec3){ 1, 0, 0}
#define VEC3_LEFT (vec3){ -1, 0, 0}
#define VEC3_FORWARD (vec3){ 0, 0, -1}
#define VEC3_BACK (vec3){ 0, 0, 1}
#define VEC3_ONE (vec3){ 1, 1, 1 }
#define VEC3_ZERO (vec3){}

#define VEC2_UP (vec2){ 0, 1}
#define VEC2_DOWN (vec2){ 0, -1}
#define VEC2_RIGHT (vec2){ 1, 0 }
#define VEC2_LEFT (vec2){ -1, 0 }
#define VEC2_ONE (vec2){ 1, 1 }
#define VEC2_ZERO (vec2){}

typedef struct{
    float x; float y;
}vec2;

typedef struct{
    float x; float y; float z;
}vec3;

typedef struct{
    float x; float y; float z; float w;
}vec4;

void vec3_print(vec3 v);

vec3 vec3_rot_dir(vec3 euler);

vec2 vec2_negate(vec2 input);
vec3 vec3_negate(vec3 input);
vec4 vec4_negate(vec4 input);

vec2 vec2_add(vec2 a, vec2 b);
vec3 vec3_add(vec3 a, vec3 b);
vec4 vec4_add(vec4 a, vec4 b);

vec2 vec2_sub(vec2 a, vec2 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec4 vec4_sub(vec4 a, vec4 b);

vec2 vec2_mul(vec2 a, float f);
vec3 vec3_mul(vec3 a, float f);
vec4 vec4_mul(vec4 a, float f);

vec2 vec2_div(vec2 a, float f);
vec3 vec3_div(vec3 a, float f);
vec4 vec4_div(vec4 a, float f);

float vec2_distance(vec2 a, vec2 b);
float vec3_distance(vec3 a, vec3 b);
float vec4_distance(vec4 a, vec4 b);

float vec2_sqrm(vec2 a);
float vec3_sqrm(vec3 a);
float vec4_sqrm(vec4 a);

float vec2_dot(vec2 a, vec2 b);
float vec3_dot(vec3 a, vec3 b);
float vec4_dot(vec4 a, vec4 b);

vec2 vec2_lerp(vec2 a, vec2 b, float t);
vec3 vec3_lerp(vec3 a, vec3 b, float t);
vec4 vec4_lerp(vec4 a, vec4 b, float t);

float vec2_length(vec2 a);
float vec3_length(vec3 a);
float vec4_length(vec4 a);

vec2 vec2_normalize(vec2 a);
vec3 vec3_normalize(vec3 a);
vec4 vec4_normalize(vec4 a);

vec3 vec3_cross(vec3 a, vec3 b);

int32 vec2_equals(vec2 a, vec2 b);
int32 vec3_equals(vec3 a, vec3 b);
int32 vec4_equals(vec4 a, vec4 b);

#endif