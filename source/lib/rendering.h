#ifndef RENDERING_H
#define RENDERING_H

#include "types.h"
#include "material.h"
#include "components/renderer.h"

void init_rendering(void);
void update_rendering(void);
void set_uniforms(Material material);
void register_material(Material material);
void add_renderer(Renderer *renderer);
void remove_renderer(Renderer *renderer);

#endif