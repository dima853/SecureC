#include "C:\Users\dmako\safe_repo\sgoto\src\include\sgoto.h"
#include <stdio.h>

// Simple test function
void test_basic_assembly()
{
    printf("Testing basic assembly functionality...\n");

    // Initialize
    sgoto_init(0x1234);
    sgoto_t *sg = sg_init(&&cleanup_label);

    // Test assembly call
    int result = sgoto_check_place(sg);
    printf("Assembly result: %d\n", result);

    // Cleanup
    free(sg);
}

int main()
{
    printf("=== SIMPLE ASSEMBLER TEST ===\n");
    test_basic_assembly();
    printf("If no errors and result=1, assembler works!\n");
    return 0;
}