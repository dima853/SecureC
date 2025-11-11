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

__attribute__((always_inline)) static inline int sgoto_check_place() {
    int result = 0;

    __asm__ volatile(
        "push %%rbx\n"
        "push %%rcx\n"
        "push %%rdx\n"
        "push %%rsi\n"
        "push %%rdi\n"

        "push %[start], %%rdi\n"
        "push %[end], %%rsi\n"
        "xor %%rbx, %%rbx\n" 

        "calc_curr_hash:\n"
        "movb (%%rdx), %%al\n"
        "xorb %%al, %%bl\n"
    )

    return result;
}

secure_goto_t *sg_init(void *cleanup_addr);
