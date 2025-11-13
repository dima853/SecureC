#include "/mnt/c/Users/dmako/safe_repo/sgoto/src/include/sgoto.h"
#include <stdio.h>

// Объявляем внешнюю функцию из sgoto.c
void **sgoto_get_label_addresses(void);

int main()
{
    printf("=== DEBUG SECURE GOTO ===\n");

    sgoto_init(0x1234);

    // Получаем адреса из ЕДИНОЙ функции
    void **label_addrs = sgoto_get_label_addresses();
    void *real_cleanup = label_addrs[0];
    printf("Real cleanup address: %p\n", real_cleanup);

    // Тест 1: Одинаковые адреса (должен вернуть 1)
    printf("\n🔍 TEST 1: Same addresses\n");
    sgoto_t *sg1 = sg_init(real_cleanup); // Передаем РЕАЛЬНЫЙ адрес
    int result1 = sgoto_check_place(sg1);
    printf("Result: %d (Expected: 1)\n", result1);

    // Тест 2: Разные адреса (должен вернуть 0)
    printf("\n🔍 TEST 2: Different addresses\n");
    sgoto_t *sg2 = sg_init(real_cleanup);
    sg2->core_addr = (void *)0x2000; // Меняем на СЛУЧАЙНЫЙ адрес
    int result2 = sgoto_check_place(sg2);
    printf("Result: %d (Expected: 0)\n", result2);

    free(sg1);
    free(sg2);

    return 0;
}