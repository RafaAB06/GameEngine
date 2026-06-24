#include "../dependencies/glfw3.h"

#include "time.h"
#include "types.h"

static double last_time = 0;
static double time = 0;
static double delta_time = 0;

void init_time(){
    last_time = glfwGetTime();
}

inline double get_delta_time(){
    return delta_time;
}

inline double get_time(){
    return time;
}

void update_time(){
    double current_time = glfwGetTime();
    double dif = current_time - last_time;

    delta_time = dif;
    time = current_time;

    last_time = current_time;
}