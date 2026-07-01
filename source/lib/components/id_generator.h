#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include "../collections/array_list.h"

typedef struct{
    uint32 next_id;

    ArrayList *unused_ids;
}ID_Generator;

ID_Generator *id_generator_new(uint32 start);
uint32 next_id(ID_Generator *gen);
void add_unused_id(ID_Generator *gen, uint32 id);

#endif