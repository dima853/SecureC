// sgoto_test.c
#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"

#include <stdio.h>
#include <stdint.h>

// Простая функция для получения адреса cleanup метки
void *get_cleanup_address(void)
{
// Объявляем метку прямо здесь
cleanup_test_label:
    __asm__("nop");
    return &&cleanup_test_label;
}

int main()
{
    printf("=== SECURE GOTO INTEGRITY TESTS ===\n\n");

    // Инициализируем
    sgoto_init(0x1234);

    // ТЕСТ 1: Получаем реальный адрес и инициализируем
    printf("Test 1: Getting real cleanup address...\n");
    void *real_cleanup = get_cleanup_address();
    printf("Real cleanup address: %p\n", real_cleanup);

    // Инициализируем с ПРАВИЛЬНЫМ адресом
    sgoto_t *sg = sg_init(real_cleanup);
    printf("SG core_addr: %p\n", sg->core_addr);

    // Проверяем целостность
    int result1 = sgoto_check_place(sg);
    printf("Result: %d (Expected: 1 - SUCCESS)\n\n", result1);

    // Если первый тест не прошел, остальные бессмысленны
    if (result1 != 1)
    {
        printf("❌ BASIC INTEGRITY CHECK FAILED! Fix this first.\n");
        printf("Possible issues:\n");
        printf("1. sgoto_check_place not working\n");
        printf("2. Label addresses mismatch\n");
        printf("3. Assembly code broken\n");
        free(sg);
        return 1;
    }

    // ТЕСТ 2: Нарушение целостности (только если первый тест прошел)
    printf("Test 2: Memory tampering simulation...\n");
    void *original_addr = sg->core_addr;
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