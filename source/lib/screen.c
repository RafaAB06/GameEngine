#include "../dependencies/glfw3.h"
#include "../dependencies/glad.h"

#include "engine.h"
#include "screen.h"

vec2 window_size;
vec2 screen_size;
float screen_aspect_ratio;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void init_screen(int width, int height){
    glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback); 
    
    set_window_size(width, height);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    screen_size.x = mode->width;
    screen_size.y = mode->height;
}

void set_window_size(int width, int height){
    window_size = (vec2){width, height};
    screen_aspect_ratio = window_size.x/window_size.y;

    glViewport(0, 0, width, height);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if(window == main_window){
        set_window_size(width, height);
    }
}  