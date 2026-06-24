#include <stdio.h>
#include <string.h>

#include "configs.h"

Configs configs = {};

void read_configs(){
    FILE *file = fopen("configs.config", "r");

    char buffer[512];
    while(fgets(buffer, sizeof(buffer), file)){
        if(!strncmp(buffer, "r", 1)){
            vec2 resolution;
            sscanf(buffer, "r %f %f", &resolution.x, &resolution.y);

            configs.resolution = resolution;
        }else if(!strncmp(buffer, "n", 1)){
            sscanf(buffer, "n %s", configs.name);
        }else if(!strncmp(buffer, "i", 1)){
            sscanf(buffer, "i %s", configs.icon);
        }
    }
}