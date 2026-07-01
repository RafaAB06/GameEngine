#include <math.h>
#include <stdio.h>

#include "matrices.h"
#include "mathf.h"

INLINE void mat_print(float *a, int32 size);
INLINE void mat_mul_vec(float *a, float*v, float *n, int32 size);
INLINE void mat_mul_s(float *a, float s, float *n, int32 size);
INLINE void mat_add(float *a, float *b, float *n, int32 size);
INLINE void mat_sub(float *a, float *b, float *n, int32 size);
INLINE void mat_mul(float *a, float *b, float *n, int32 size);
INLINE void mat_transpose(float *a, float *b, int32 size);

void mat4_print(mat4 mat){
    mat_print(&mat.m[0], 4);
}

vec3 mat4_angles(mat4 m){
    // float *fs = m.m;

    // float r31 = fs[2];
    // float y = -asinf(fs[2]);
    // float cos_y = cosf(x);
    // float y = atan2(
    //     fs[6]/cos_y,
    //     fs[10]/cos_y
    // );
    // float z = atan2(
    //     fs[1]/cos_y,
    //     fs[0]/cos_y
    // );

    // R11 = m[0]
    // R12 = m[4]
    // R13 = m[8]

    // R21 = m[1]
    // R22 = m[5]
    // R23 = m[9]

    // R31 = m[2]
    // R32 = m[6]
    // R33 = m[10]

    // if (R31 != ±1)
    //     θ1 = -asin(R31)
    //     θ2 = π - θ1

    //     ψ1 = atan2(R32 / cos(θ1), R33 / cos(θ1))
    //     ψ2 = atan2(R32 / cos(θ2), R33 / cos(θ2))

    //     φ1 = atan2(R21 / cos(θ1), R11 / cos(θ1))
    //     φ2 = atan2(R21 / cos(θ2), R11 / cos(θ2))

    // else
    //     φ = anything; can set to 0

    //     if (R31 == -1)
    //         θ = π/2
    //         ψ = φ + atan2(R12, R13)
    //     else
    //         θ = -π/2
    //         ψ = -φ + atan2(-R12, -R13)
    //     end if
    // end if
}

vec3 mat4_back(mat4 m){
    float *fs = m.m;
    vec3 v = (vec3){
        fs[8], fs[9], fs[10]
    };

    return vec3_normalize(v);
}

vec3 mat4_right(mat4 m){
    float *fs = m.m;
    vec3 v = (vec3){
        fs[0], fs[1], fs[2]
    };

    return vec3_normalize(v);
}

vec3 mat4_up(mat4 m){
    float *fs = m.m;
    vec3 v = (vec3){
        fs[4], fs[5], fs[6]
    };

    return vec3_normalize(v);
}

mat4 mat4_look_at(vec3 eye, vec3 target, vec3 up){
    vec3 f = vec3_normalize(vec3_sub(eye, target));
    float dot = vec3_dot(f, VEC3_UP);
    if(fabs(dot) > 0.999f){
        up = VEC3_FORWARD;
    }
    vec3 r = vec3_normalize(vec3_cross(up, f));
    vec3 u = vec3_cross(f, r);

    mat4 t = mat4_translate(MAT4_IDENTITY, vec3_mul(eye, -1.0f));
    mat4 rot = { .m = {
        r.x, r.y, r.z, 0,
        u.x, u.y, u.z, 0,
        f.x, f.y, f.z, 0,//open gl usa -Z, entao informo o back
        0  , 0  , 0  , 1
    }};

    return rot;
}

mat4 mat4_transpose(mat4 m){
    mat4 n = {};
    mat_transpose(&m.m[0], &n.m[0], 4);

    return n;
}

mat4 mat4_translate(mat4 m, vec3 v3){
    mat4 t = { .m = {
        1,    0,    0,    0,
        0,    1,    0,    0,
        0,    0,    1,    0,
        v3.x, v3.y, v3.z, 1
    }};
    
    return mat4_mul(m, t);
}

mat4 mat4_rotate(mat4 m, vec3 angles){
    float c, s, deg, rad;

    deg = angles.x;
    rad = deg2rad(deg);
    c = cosf(rad);
    s = sinf(rad);
    mat4 x = { .m = {
        1, 0, 0, 0,
        0, c, s, 0,
        0,-s, c, 0,
        0, 0, 0, 1
    }};

    deg = angles.y;
    rad = deg2rad(deg);
    c = cosf(rad);
    s = sinf(rad);
    mat4 y = { .m = {
        c, 0,-s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    }};

    deg = angles.z;
    rad = deg2rad(deg);
    c = cosf(rad);
    s = sinf(rad);
    mat4 z = { .m = {
        c, s, 0, 0,
       -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }};
    mat4 zy = mat4_mul(z, y);
    mat4 zyx = mat4_mul(zy, x);
    return mat4_mul(m, zyx);
}

mat4 mat4_scale(mat4 m, vec3 v3){
    mat4 s = { .m = {
        v3.x, 0,    0,    0,
        0,    v3.y, 0,    0,
        0,    0,    v3.z, 0,
        0,    0,    0,    1
    }};

    return mat4_mul(m, s);
}

mat4 mat4_trs(vec3 position, vec3 rotation, vec3 scale){
    mat4 t = mat4_translate(MAT4_IDENTITY, position);
    mat4 r = mat4_rotate(MAT4_IDENTITY, rotation);
    mat4 s = mat4_scale(MAT4_IDENTITY, scale);

    return mat4_mul(t, mat4_mul(r, s));
}

vec4 mat4_vec4(mat4 a, vec4 v){
    vec4 n;

    mat_mul_vec(&a.m[0], &v.x, &n.x, 4);

    return n;
}

mat4 mat4_mul_s(mat4 a, float s){
    mat4 n;

    mat_mul_s(&a.m[0], s, &n.m[0], 4);

    return n;
}

mat4 mat4_add(mat4 a, mat4 b){
    mat4 n;

    mat_add(&a.m[0], &b.m[0], &n.m[0], 4);

    return n;
}

mat4 mat4_sub(mat4 a, mat4 b){
    mat4 n;

    mat_sub(&a.m[0], &b.m[0], &n.m[0], 4);

    return n;
}

mat4 mat4_mul(mat4 a, mat4 b){
    mat4 n;

    mat_mul(&a.m[0], &b.m[0], &n.m[0], 4);
    
    return n;
}

INLINE void mat_print(float *a, int32 size){
    int32 c, r;
    for(c = 0; c<size; c++){
        for(r = 0; r<size; r++){
            printf("%.2f ", a[c + r * size]);
        }
        putchar('\n');
    }
}

INLINE void mat_mul_vec(float *a, float*v, float *n, int32 size){
    int32 r, k;
    for(r = 0; r < size; r++){
        float sum = 0;
        for(k = 0; k < size; k++){
            sum += a[r + size * k] * v[k];
        }
        n[r] = sum;
    }
}

INLINE void mat_mul_s(float *a, float s, float *n, int32 size){
    int32 i;
    for(i = 0; i<size * size; i++){
        n[i] = a[i] * s;
    }
}

INLINE void mat_add(float *a, float *b, float *n, int32 size){
    int32 i;
    for(i = 0; i<size * size; i++){
        n[i] = a[i] + b[i];
    }
}

INLINE void mat_sub(float *a, float *b, float *n, int32 size){
    int32 i;
    for(i = 0; i<size * size; i++){
        n[i] = a[i] - b[i];
    }
}

INLINE void mat_mul(float *a, float *b, float *n, int32 size){
    int32 r, c, i;
    for(r = 0; r<size; r++){
        for(c = 0; c<size; c++){
            float sum = 0.0;
            for(i = 0; i < size; i++){
                sum += a[i * size + r] * b[c * size + i];
            }
            n[c * size + r] = sum;
        }
    }
}

INLINE void mat_transpose(float *a, float *b, int32 size){
    int32 c, r;
    for(c = 0; c<size; c++){
        for(r = 0; r<size; r++){
            b[r * size + c] = a[c * size + r];
        }
    }
}