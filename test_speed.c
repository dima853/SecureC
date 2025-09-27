#include "../include/smemset.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_SIZE 1000000 // 1 MB
#define ITERATIONS 100

int main()
{
    char *buffer1 = malloc(TEST_SIZE);
    char *buffer2 = malloc(TEST_SIZE);

    if (!buffer1 || !buffer2)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    clock_t start = clock();
    for (int i = 0; i < ITERATIONS; i++)
    {
        memset(buffer1, 0x42, TEST_SIZE);
    }
    clock_t end = clock();
    double std_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < ITERATIONS; i++)
    {
        smemset(buffer2, 0x42, TEST_SIZE);
    }
    end = clock();
    double secure_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("📊 Результаты теста (%d итераций по %d байт):\n", ITERATIONS, TEST_SIZE);
    printf("───────────────────────────────────────\n");
    printf("│ %-15s │ %-12s │\n", "Функция", "Время (сек)");
    printf("───────────────────────────────────────\n");
    printf("│ %-15s │ %-12.3f │\n", "memset", std_time);
    printf("│ %-15s │ %-12.3f │\n", "smemset", secure_time);
    printf("───────────────────────────────────────\n");

    double slowdown = (secure_time / std_time - 1) * 100;
    printf("⚡ Замедление: %.1f%%\n", slowdown);

    int ok1 = 1, ok2 = 1;
    for (int i = 0; i < 100; i++)
    {
        if (buffer1[i] != 0x42)
            ok1 = 0;
        if (buffer2[i] != 0x42)
            ok2 = 0;
    }

    printf("✅ Проверка данных: memset=%s, smemset=%s\n",
           ok1 ? "OK" : "FAIL", ok2 ? "OK" : "FAIL");

    free(buffer1);
    free(buffer2);
    return 0;
}