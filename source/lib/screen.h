#ifndef SCREEN_H
#define SCREEN_H

#include "math/vectors.h"

extern vec2 window_size;
extern vec2 screen_size;
extern float screen_aspect_ratio;

void init_screen(int width, int height);
void set_window_size(int width, int height);

#endif