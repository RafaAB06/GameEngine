#include <math.h>
#include <stdio.h>

#include "vectors.h"

void vec3_print(vec3 v){
    printf("[%.2f, %.2f, %.2f]\n", v.x, v.y, v.z);
}

vec3 vec3_rot_dir(vec3 euler){
    vec3 dir;

    dir.x = cosf(euler.x) * sinf(euler.y);
    dir.y = sinf(euler.x);
    dir.z = -cosf(euler.x) * cosf(euler.y);

    return dir;
}

vec2 vec2_negate(vec2 input){
    vec2 output;
    output.x = -input.x;
    output.y = -input.y;
    return output;
}

vec3 vec3_negate(vec3 input){
    vec3 output;
    output.x = -input.x;
    output.y = -input.y;
    output.z = -input.z;
    return output;
}

vec4 vec4_negate(vec4 input){
    vec4 output;
    output.x = -input.x;
    output.y = -input.y;
    output.z = -input.z;
    output.w = -input.w;
    return output;
}

vec2 vec2_add(vec2 a, vec2 b){
    vec2 result = {
        a.x + b.x,
        a.y + b.y
    };
    return result;
}

vec3 vec3_add(vec3 a, vec3 b){
    vec3 result = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
    return result;
}

vec4 vec4_add(vec4 a, vec4 b){
    vec4 result = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
    return result;
}

vec2 vec2_sub(vec2 a, vec2 b){
    vec2 result = {
        a.x - b.x,
        a.y - b.y
    };

    return result;
}

vec3 vec3_sub(vec3 a, vec3 b){
    vec3 result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };

    return result;
}

vec4 vec4_sub(vec4 a, vec4 b){
    vec4 result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    };

    return result;
}

vec2 vec2_mul(vec2 a, float f){
    vec2 result = {
        a.x * f,
        a.y * f
    };

    return result;
}

vec3 vec3_mul(vec3 a, float f){
    vec3 result = {
        a.x * f,
        a.y * f,
        a.z * f
    };

    return result;
}

vec4 vec4_mul(vec4 a, float f){
    vec4 result = {
        a.x * f,
        a.y * f,
        a.z * f,
        a.w * f
    };

    return result;
}

vec2 vec2_div(vec2 a, float f){
    vec2 result = {
        a.x / f,
        a.y / f
    };

    return result;
}

vec3 vec3_div(vec3 a, float f){
    vec3 result = {
        a.x / f,
        a.y / f,
        a.z / f
    };

    return result;
}

vec4 vec4_div(vec4 a, float f){
    vec4 result = {
        a.x / f,
        a.y / f,
        a.z / f,
        a.w / f
    };

    return result;
}

float vec2_distance(vec2 a, vec2 b){
    return sqrt(
        pow(a.x - b.x, 2.0) +
        pow(a.y - b.y, 2.0)
    );
}

float vec3_distance(vec3 a, vec3 b){
    return sqrt(
        pow(a.x - b.x, 2.0) +
        pow(a.y - b.y, 2.0) +
        pow(a.z - b.z, 2.0)
    );
}

float vec4_distance(vec4 a, vec4 b){
    return sqrt(
        pow(a.x - b.x, 2.0) +
        pow(a.y - b.y, 2.0) +
        pow(a.z - b.z, 2.0) +
        pow(a.w - b.w, 2.0)
    );
}

float vec2_sqrm(vec2 a){
    return pow(a.x, 2.0) + pow(a.y, 2.0);
}

float vec3_sqrm(vec3 a){
    return pow(a.x, 2.0) + pow(a.y, 2.0) + pow(a.z, 2.0);
}

float vec4_sqrm(vec4 a){
    return pow(a.x, 2.0) + pow(a.y, 2.0) + pow(a.z, 2.0) + pow(a.w, 2.0);
}

float vec2_dot(vec2 a, vec2 b){
    return a.x * b.x + a.y * b.y;
}

float vec3_dot(vec3 a, vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec4_dot(vec4 a, vec4 b){
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vec2 vec2_lerp(vec2 a, vec2 b, float t){
    vec2 result = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y)
    };
    return result;
}

vec3 vec3_lerp(vec3 a, vec3 b, float t){
    vec3 result = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z)
    };
    return result;
}

vec4 vec4_lerp(vec4 a, vec4 b, float t){
    vec4 result = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w)
    };
    return result;
}

float vec2_length(vec2 a){
    return sqrt(a.x * a.x + a.y * a.y);
}

float vec3_length(vec3 a){
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float vec4_length(vec4 a){
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

vec2 vec2_normalize(vec2 a){
    float length = vec2_length(a);
    vec2 result = vec2_div(a, length);

    return result;
}

vec3 vec3_normalize(vec3 a){
    float length = vec3_length(a);
    vec3 result = vec3_div(a, length);

    return result;
}

vec4 vec4_normalize(vec4 a){
    float length = vec4_length(a);
    vec4 result = vec4_div(a, length);

    return result;
}

vec3 vec3_cross(vec3 a, vec3 b){
    vec3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

INLINE int32 float_equals(float a, float b){
    return fabsf(a - b) < 0.0001f;
}

int32 vec2_equals(vec2 a, vec2 b){
    return float_equals(a.x, b.x) &&
           float_equals(a.y, b.y);
}

int32 vec3_equals(vec3 a, vec3 b){
    return float_equals(a.x, b.x) &&
           float_equals(a.y, b.y) &&
           float_equals(a.z, b.z);
}

int32 vec4_equals(vec4 a, vec4 b){
    return float_equals(a.x, b.x) &&
           float_equals(a.y, b.y) &&
           float_equals(a.z, b.z) &&
           float_equals(a.w, b.w);
}