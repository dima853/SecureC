#include "C:\Users\dmako\safe_repo\sgoto\src\include\sgoto.h"

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

// Function to get current addresses of all verification labels
void **sgoto_get_label_addresses(void)
{
    // Declare labels in the same function where we use their addresses
    __asm__ volatile(
        "jmp after_labels_sgoto\n"

        "cleanup_label:\n"
        "nop\n"

        "goto_label_1:\n"
        "nop\n"

        "goto_label_2:\n"
        "nop\n"

        "goto_label_3:\n"
        "nop\n"

        "after_labels_sgoto:\n");

    static void *labels[16];
    labels[0] = &&cleanup_label;
    labels[1] = &&goto_label_1;
    labels[2] = &&goto_label_2;
    labels[3] = &&goto_label_3;
    return labels;
}