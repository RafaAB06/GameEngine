#ifndef CONFIGS_H
#define CONFIGS_H

#include "types.h"
#include "math/vectors.h"

typedef struct{
    vec2 resolution;
    char name[256];
    char icon[256]; 
}Configs;

extern Configs configs;

void read_configs();

#endif