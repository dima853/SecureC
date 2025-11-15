#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

HashGenerator global_hash_gen;

void sgoto_init(uint64_t seed)
{
    hash_generator_init(&global_hash_gen, seed);
}

sgoto_t *sg_init(void *cleanup_addr)
{
    sgoto_t *sg = malloc(sizeof(sgoto_t));
    sg->curr_hash = hash_generator_next(&global_hash_gen);
    sg->core_addr = cleanup_addr;
    sg->child_count = 0;
    return sg;
}

void **sgoto_get_label_addresses(void)
{
    static void *labels[16];

    labels[0] = (void *)0x1;
    labels[1] = (void *)0x2;
    labels[2] = (void *)0x3;
    labels[3] = (void *)0x4;
    labels[4] = (void *)0x5;
    labels[5] = (void *)0x6;
    labels[6] = (void *)0x7;
    labels[7] = (void *)0x8;
    labels[8] = (void *)0x9;
    labels[9] = (void *)0xA;
    labels[10] = (void *)0xB;
    labels[11] = (void *)0xC;
    labels[12] = (void *)0xD;
    labels[13] = (void *)0xE;
    labels[14] = (void *)0xF;
    labels[15] = (void *)0x10;

    return labels;
}

int sgoto_check_place(sgoto_t *sg, void **current_labels)
{
    int result = 0;

    __asm__ volatile(
        "mov %[cleanup_addr], %%rax\n"
        "mov %[actual_cleanup], %%rdx\n"
        "cmp %%rax, %%rdx\n"
        "jne integrity_failed\n"

        "mov %[goto_count], %%ecx\n"
        "test %%ecx, %%ecx\n"
        "jz check_success\n"

        "mov %[goto_addrs], %%rsi\n"
        "mov %[current_labels], %%rdi\n"
        "add $8, %%rdi\n"

        "check_goto_loop:\n"
        "movq (%%rsi), %%r8\n"
        "movq (%%rdi), %%r9\n"
        "cmp %%r8, %%r9\n"
        "jne integrity_failed\n"
        "add $8, %%rsi\n"
        "add $8, %%rdi\n"
        "dec %%ecx\n"
        "jnz check_goto_loop\n"

        "check_success:\n"
        "mov $1, %[result]\n"
        "jmp end\n"

        "integrity_failed:\n"
        "mov $0, %[result]\n"

        "end:\n"
        : [result] "=r"(result)
        : [cleanup_addr] "r"(sg->core_addr),
          [actual_cleanup] "r"(current_labels[0]),
          [goto_count] "r"((uint32_t)sg->child_count),
          [goto_addrs] "r"(sg->goto_addrs),
          [current_labels] "r"(current_labels)
        : "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "cc", "memory");

    return result;
}

void sgoto_raw(sgoto_t *sg, uint8_t idx)
{
    // Получаем текущие метки для проверки
    void **current_labels = sgoto_get_label_addresses();

    if (!sgoto_check_place(sg, current_labels))
    {
        __asm__ volatile("jmp *%0" : : "r"(sg->core_addr));
    }

    __asm__ volatile(
        "mov %[sg_ptr], %%rbx\n"
        "mov %[index], %%al\n"
        "cmp 0xB0(%%rbx), %%al\n"
        "jae sgoto_fail_%=\n"
        "movzx %%al, %%rax\n"
        "mov 0x10(%%rbx, %%rax, 8), %%rax\n"
        "jmp *%%rax\n"
        "sgoto_fail_%=:\n"
        "mov 0x8(%%rbx), %%rax\n"
        "jmp *%%rax\n"
        :
        : [sg_ptr] "r"(sg), [index] "r"(idx)
        : "rax", "rbx", "cc", "memory");
}