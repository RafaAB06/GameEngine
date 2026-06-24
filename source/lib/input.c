#include "../dependencies/glfw3.h"

#include "math/vectors.h"

#include "engine.h"
#include "input.h"

static vec2 mouse_position;
static vec2 mouse_delta;

void init_input(){
    mouse_position = (vec2){};
    mouse_delta = (vec2){};

    glfwSetCursorPosCallback(main_window, glfw_mouse_callback);
}

inline vec2 get_mouse_position(){
    return mouse_position;
} 

inline vec2 get_mouse_delta(){
    return mouse_delta;
}

inline int get_key_down(int key){
    return glfwGetKey(main_window, key);
}

void glfw_mouse_callback(GLFWwindow* window, double xpos, double ypos){
    vec2 position = { xpos, ypos };
    vec2 previous_position = mouse_position;

    vec2 delta = vec2_sub(position, previous_position);

    mouse_position = position;
    mouse_delta = delta;
}

void update_input(){
    //do smth
}