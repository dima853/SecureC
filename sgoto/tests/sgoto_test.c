#include "sgoto.h"
#include <stdio.h>

int main()
{
    printf("=== SIMPLE SECURE GOTO TEST ===\n");

    sgoto_init(0x1234);

    // Тест 1: NULL адрес (как у тебя)
    printf("Test 1: NULL address...\n");
    sgoto_t *sg1 = sg_init(NULL);
    int result1 = sgoto_check_place(sg1);
    printf("Result with NULL: %d\n", result1);

    // Тест 2: Случайный адрес
    printf("Test 2: Random address...\n");
    sgoto_t *sg2 = sg_init((void *)0x12345678);
    int result2 = sgoto_check_place(sg2);
    printf("Result with random: %d\n", result2);

    // Тест 3: Другой случайный адрес
    printf("Test 3: Another random address...\n");
    sgoto_t *sg3 = sg_init((void *)0x87654321);
    int result3 = sgoto_check_place(sg3);
    printf("Result with another random: %d\n", result3);

    free(sg1);
    free(sg2);
    free(sg3);

    printf("=== ANALYSIS ===\n");
    if (result1 == 1 && result2 == 1 && result3 == 1)
    {
        printf("❌ ASSEMBLER IS BROKEN - always returns 1!\n");
    }
    else
    {
        printf("✅ Assembler works correctly\n");
    }

    return 0;
}