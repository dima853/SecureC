#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>

int main()
{
    printf("Testing secure goto...\n");

    // Initialize
    sgoto_init(0x1234);

    // Get cleanup address from labels
    void **labels = sgoto_get_label_addresses();
    sgoto_t *sg = sg_init(labels[0]); // Use actual cleanup label address

    // Test integrity check
    int result = sgoto_check_place(sg);
    printf("Integrity check result: %d\n", result);

    free(sg);
    return 0;
}