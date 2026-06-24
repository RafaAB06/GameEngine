#include <stdlib.h>
#include <stdio.h>

#include "mesh_processor.h"
#include "texture_processor.h"
#include "shader_processor.h"

int main(){
    process_meshes();
    process_textures();
    process_shaders();

    exit(1);//??????
}