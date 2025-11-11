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
        "mov %[cleanup_addr], %%rax\n"
        "lea cleanup_label(%%), %%rdx\n"
        "cmp %%rax, %%rdx\n"
        "jne integrity_failed\n"

        "mov %[goto_count], %%rcx\n"     
        "mov %[goto_addrs], %%rsi\n"  
        "mov %[goto_labels], %%rdi\n"

        "check_goto_loop:\n"
        "mov (%%rax), %%r8\n"
        "mov (%%rdx), %%r9\n"
        "cmp %%r8, %%r9\n"
        "jne integrity_failed\n"
        "add $8, %%rax\n"
        "add $8, %%rdx\n" 
        "dec %%rcx\n"
        "jnz check_goto_loop\n"

        "mov $1, %[result]\n"
        "jmp end\n"

        "integrity_failed\n"

        "mov $0, %[result]\n"
        "end\n"
        
        )

    return result;
}

secure_goto_t *sg_init(void *cleanup_addr);
