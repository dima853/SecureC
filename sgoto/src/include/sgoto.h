#pragma once

#include "/mnt/c/Users/dmako/safe_repo/sgoto/third-party/ephemeral-hash-cas/src/core/include/core.h"

typedef struct
{
    uint16_t curr_hash;
    void *core_addr;
    void *goto_addrs[16];
    uint16_t goto_hashes[16];
    uint8_t child_count;
} sgoto_t;

extern HashGenerator global_hash_gen;

void sgoto_init(uint64_t seed);
sgoto_t *sg_init(void *cleanup_addr);
void **sgoto_get_label_addresses(void);
int sgoto_check_place(sgoto_t *sg, void **current_labels);
void sgoto_raw(sgoto_t *sg, uint8_t idx);