#include <stdlib.h>

#include "id_generator.h"
#include "../collections/array_list.h"

ID_Generator *id_generator_new(uint32 start){
    ID_Generator *gen = (ID_Generator *)malloc(sizeof(ID_Generator));

    ArrayList *list = array_list_new(sizeof(uint32));

    gen->next_id = start;
    gen->unused_ids = list;

    return gen;
}

uint32 next_id(ID_Generator *gen){
    ArrayList *list = gen->unused_ids;
    uint32 count = list->count;
    uint32 id;

    if(count){
        int32 index =  count - 1;
        array_list_get(gen->unused_ids, &id, index);
        array_list_remove_at(gen->unused_ids, index);
    }else{
        id = gen->next_id;
        gen->next_id++;
    }
    return id;
}


