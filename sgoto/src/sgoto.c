#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>
#include <stdlib.h>

// Define global hash generator
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

// ЕДИНАЯ функция для получения адресов меток
void **sgoto_get_label_addresses(void)
{
// Объявляем метки как C метки в этой функции
cleanup_label:
    __asm__ volatile("nop");

goto_label_1:
    __asm__ volatile("nop");

goto_label_2:
    __asm__ volatile("nop");

goto_label_3:
    __asm__ volatile("nop");

    static void *labels[16];
    labels[0] = &&cleanup_label;
    labels[1] = &&goto_label_1;
    labels[2] = &&goto_label_2;
    labels[3] = &&goto_label_3;

    return labels;
}

// Secure GOTO integrity verification function
int sgoto_check_place(sgoto_t *sg)
{
    int result = 0;
    void **label_addrs = sgoto_get_label_addresses();

    printf("[DEBUG] === START ASSEMBLER EXECUTION ===\n");
    printf("[DEBUG] Input: cleanup_addr=%p, actual_cleanup=%p\n", sg->core_addr, label_addrs[0]);
    printf("[DEBUG] Input: goto_count=%d\n", sg->child_count);
    printf("[DEBUG] Initial result=%d\n", result);

    __asm__ volatile(
        // Cleanup address check
        "mov %[cleanup_addr], %%rax\n"
        "mov %[actual_cleanup], %%rdx\n"
        "cmp %%rax, %%rdx\n"
        "jne integrity_failed\n"

        "mov $1, %[result]\n"
        "jmp end\n"

        "integrity_failed:\n"
        "mov $0, %[result]\n"

        "end:\n"
        : [result] "=r"(result)
        : [cleanup_addr] "r"(sg->core_addr),
          [actual_cleanup] "r"(label_addrs[0])
        : "rax", "rdx", "cc");

    printf("[DEBUG] === END ASSEMBLER EXECUTION ===\n");
    printf("[DEBUG] Final result=%d\n", result);

    return result;
}