#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
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

// Function to get current addresses of all verification labels
void **sgoto_get_label_addresses(void)
{
    static void *labels[16];

// Объявляем все 16 обычных меток C
cleanup_label:
goto_label_1:
goto_label_2:
goto_label_3:
goto_label_4:
goto_label_5:
goto_label_6:
goto_label_7:
goto_label_8:
goto_label_9:
goto_label_10:
goto_label_11:
goto_label_12:
goto_label_13:
goto_label_14:
goto_label_15:
goto_label_16:

    // Получаем адреса всех меток
    labels[0] = &&cleanup_label;
    labels[1] = &&goto_label_1;
    labels[2] = &&goto_label_2;
    labels[3] = &&goto_label_3;
    labels[4] = &&goto_label_4;
    labels[5] = &&goto_label_5;
    labels[6] = &&goto_label_6;
    labels[7] = &&goto_label_7;
    labels[8] = &&goto_label_8;
    labels[9] = &&goto_label_9;
    labels[10] = &&goto_label_10;
    labels[11] = &&goto_label_11;
    labels[12] = &&goto_label_12;
    labels[13] = &&goto_label_13;
    labels[14] = &&goto_label_14;
    labels[15] = &&goto_label_15;
    // Последний элемент массива (16-й) не используется, так как cleanup_label занимает 0-ю позицию

    return labels;
}