#include "sgoto.h"

void sgoto_init(uint64_t seed)
{
    hash_generator_init(&global_hash_gen, seed);
}

sgoto_t *sg_init(void *cleanup_addr)
{
    sgoto_t *sg = malloc(sizeof(sgoto_t));
    sg->current_hash = hash_generator_next(&global_hash_gen);
    sg->cleanup_addr = cleanup_addr;
    sg->child_count = 0;
    return sg;
}