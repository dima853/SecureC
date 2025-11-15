// test_sgoto_fixed.c
#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>
#include <stdlib.h>

// Define real labels that we can jump to
void cleanup_label()
{
    printf("Cleanup label reached\n");
    exit(0);
}

void goto_label_1()
{
    printf("Goto label 1 reached - SUCCESS!\n");
    exit(0);
}

void goto_label_2()
{
    printf("Goto label 2 reached\n");
    exit(0);
}

void goto_label_3()
{
    printf("Goto label 3 reached\n");
    exit(0);
}

void test_function()
{
    printf("=== START SGOTO TEST ===\n");

    sgoto_init(12345);

    // Get the real label addresses
    void **label_addrs = sgoto_get_label_addresses();

    sgoto_t *sg = malloc(sizeof(sgoto_t));
    sg->core_addr = label_addrs[0]; // cleanup_label
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

    // If we return here, it means the jump failed
    printf("ERROR: Should not reach here after jump!\n");
    free(sg);
}

int main()
{
    test_function();
    return 0;
}