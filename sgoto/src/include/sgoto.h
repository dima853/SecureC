#pragma once

#include "core.h"

typedef struct
{
    uint16_t curr_hash;
    void *core_addr;
    void *goto_addrs[16];
    uint16_t goto_hashes[16];
    uint8_t child_count;
} sgoto_t;

extern HashGenerator global_hash_gen;

secure_goto_t *sg_init(void *cleanup_addr);