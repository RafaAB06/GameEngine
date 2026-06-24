#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "../dependencies/glfw3.h"
#include "../dependencies/glad.h"
#include "components/components.h"

#include "material.h"
#include "configs.h"
#include "rendering.h"
#include "engine.h"
#include "types.h"
#include "time.h"
#include "screen.h"
#include "input.h"

#include "../game/game.h"

GLFWwindow *main_window = NULL;

void set_icon(){
    char path[512];
    sprintf(path, "textures\\%s.texture", configs.icon);
    FILE *file = fopen(path, "rb");

    struct texture_metadata{
        int32 width, height, channels;
    };
    struct texture_metadata metadata;
    fread(&metadata, sizeof(struct texture_metadata), 1, file);

    int32 bytes_count = metadata.width * metadata.height * metadata.channels;
    int8 *rgb = malloc(bytes_count);
    fread(rgb, 1, bytes_count, file);

    uint8 *rgba = malloc(metadata.width * metadata.height * 4);
    for(int i = 0; i < metadata.width * metadata.height; i++){
        rgba[i * 4 + 0] = rgb[i * 3 + 0];
        rgba[i * 4 + 1] = rgb[i * 3 + 1];
        rgba[i * 4 + 2] = rgb[i * 3 + 2];
        rgba[i * 4 + 3] = 255;
    }//magica maligna para converter rgb para rgba, thanks chat gpt

    GLFWimage icon;
    icon.width = metadata.width;
    icon.height = metadata.height;
    icon.pixels = rgba;

    glfwSetWindowIcon(main_window, 1, &icon);

    free(rgb);
    free(rgba);
}

void create_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    vec2 resolution = configs.resolution;
    GLFWmonitor* monitor = NULL;
    if(vec2_equals(resolution, VEC2_ZERO)){
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        resolution.x = mode->width;
        resolution.y = mode->height;
    }
    main_window = glfwCreateWindow(resolution.x, resolution.y, configs.name, monitor, NULL); 
    glfwMakeContextCurrent(main_window);   

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);

    init_screen(configs.resolution.x, configs.resolution.y);

    set_icon();
}

int main(){
    read_configs();
    create_window();

    init_components();

    init_time();
    init_input();
    init_material();
    init_rendering();

    game_awake();

    while (!glfwWindowShouldClose(main_window)){
        glfwPollEvents();
        update_input();
        update_components();

        game_update();

        update_rendering();

        glfwSwapBuffers(main_window);

        update_time();
    }
    game_shutdown();

    glfwTerminate();

    return 0;
}
