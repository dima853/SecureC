#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>
#include <stdlib.h>

void test_function()
{
    printf("=== START SGOTO TEST ===\n");

    sgoto_init(12345);

    // Получаем адреса меток из sgoto_get_label_addresses
    void **label_addrs = sgoto_get_label_addresses();

    sgoto_t *sg = malloc(sizeof(sgoto_t));
    sg->core_addr = label_addrs[0]; // cleanup_label из sgoto_get_label_addresses
    sg->child_count = 3;
    sg->goto_addrs[0] = label_addrs[1]; // goto_label_1
    sg->goto_addrs[1] = label_addrs[2]; // goto_label_2
    sg->goto_addrs[2] = label_addrs[3]; // goto_label_3

    printf("Structure initialized:\n");
    printf("  cleanup_addr: %p\n", sg->core_addr);
    printf("  goto_addrs[0]: %p\n", sg->goto_addrs[0]);
    printf("  goto_addrs[1]: %p\n", sg->goto_addrs[1]);
    printf("  goto_addrs[2]: %p\n", sg->goto_addrs[2]);
    printf("  child_count: %d\n", sg->child_count);

    printf("\nTesting sgoto_raw with index 0...\n");
    sgoto_raw(sg, 0);

    // Если сюда вернулись - ошибка
    printf("ERROR: Should not reach here after jump 0!\n");
    free(sg);
    return;

// Эти метки теперь не используются - переходы идут в sgoto_get_label_addresses
unused_label_1:
    printf("This should not be reached\n");
    return;

unused_label_2:
    printf("This should not be reached\n");
    return;

unused_label_3:
    printf("This should not be reached\n");
    return;

unused_cleanup:
    printf("This should not be reached\n");
    return;
}

int main()
{
    test_function();
    return 0;
}