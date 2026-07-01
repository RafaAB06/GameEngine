#include "../rendering.h"
#include "renderer.h"

int32 renderer_comparator(void *a, void *b){
    Renderer r_1 = *(Renderer *)a;
    Renderer r_2 = *(Renderer *)b;

    if(r_1.program_id == r_2.program_id){
        return (r_1.material_id > r_2.material_id) - (r_1.material_id < r_2.material_id);
    }

    return (r_1.program_id > r_2.program_id) - (r_1.program_id < r_2.program_id);
}

void renderer_new(Renderer *self, uint32 entity_id,  Mesh *mesh, Material mat){
    self->entity_id = entity_id;
    self->mesh = mesh;
    self->material_id = mat.id;
    self->program_id = mat.program.gl_id;

    add_renderer(self);//adiciona ao pipeline grafico
}

void renderer_on_update(void *self){
    //as matrizes faço lá mesmo
    //calculos de matrizes que eu nao fiz ainda
}

void renderer_on_destroy(void *self){
    remove_renderer(self);//remove do pipeline gráfico
}