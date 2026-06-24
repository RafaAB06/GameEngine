#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "../math/vectors.h"
#include "../types.h"

typedef struct{
   vec3 position;
   vec3 rotation;
   vec4 color; 
   float intensity;
}DirectionalLight;

#endif