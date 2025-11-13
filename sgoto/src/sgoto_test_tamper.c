// sgoto_test.c
#include "sgoto.h"
#include <stdio.h>
#include <stdint.h>

int main()
{
    printf("=== SECURE GOTO INTEGRITY TESTS ===\n\n");

    sgoto_init(0x1234);

    // ТЕСТ 1: Нормальная работа
    printf("Test 1: Normal operation...\n");
    sgoto_t *sg = sg_init(NULL);
    int result1 = sgoto_check_place(sg);
    printf("Result: %d (Expected: 1 - SUCCESS)\n\n", result1);

    // ТЕСТ 2: Нарушение целостности
    printf("Test 2: Memory tampering simulation...\n");
    void *original_addr = sg->core_addr;
    printf("Original cleanup address: %p\n", original_addr);

    // Имитируем атаку
    sg->core_addr = (void *)((uintptr_t)original_addr + 0x1000);
    printf("Tampered cleanup address: %p\n", sg->core_addr);

    int result2 = sgoto_check_place(sg);
    printf("Result: %d (Expected: 0 - INTEGRITY FAILURE)\n\n", result2);

    // ТЕСТ 3: Восстановление
    printf("Test 3: Restoring original address...\n");
    sg->core_addr = original_addr;
    int result3 = sgoto_check_place(sg);
    printf("Result: %d (Expected: 1 - SUCCESS)\n\n", result3);

    printf("=== TEST SUMMARY ===\n");
    printf("Normal: %s | Tampering: %s | Recovery: %s\n",
           result1 == 1 ? "PASS" : "FAIL",
           result2 == 0 ? "PASS" : "FAIL",
           result3 == 1 ? "PASS" : "FAIL");

    free(sg);
    return 0;
}