#pragma once

#include "/mnt/c/Users/dmako/safe_repo/sgoto/third-party/ephemeral-hash-cas/src/core/include/core.h"
#include <stdint.h>
#include <stdlib.h>

// Secure GOTO structure definition
typedef struct
{
    uint16_t curr_hash;
    void *core_addr;
    void *goto_addrs[16];
    uint16_t goto_hashes[16];
    uint8_t child_count;
} sgoto_t;

// External global hash generator
extern HashGenerator global_hash_gen;

// Function declarations
void sgoto_init(uint64_t seed);
sgoto_t *sg_init(void *cleanup_addr);
int sgoto_check_place(sgoto_t *sg);