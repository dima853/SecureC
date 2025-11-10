// benchmark_antidebug.c
#include <stdio.h>
#include <stdint.h>
#include <time.h>

// 🔴 ТРАДИЦИОННЫЕ ЕБАНЫЕ МАКРОСЫ (LEGACY SHIT)
#ifndef MALLOC_DEBUG
#define MALLOC_DEBUG 0
#endif

#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

// 🚀 ТВОЙ АНТИ-ДЕБАГ (MODERN PROTECTION)
__attribute__((always_inline)) static inline int get_malloc_debug_flag(void)
{
    int result = 0;
    uint64_t start;
    __asm__ volatile(
        "rdtsc\n\t"
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "movq %%rax, %1\n\t"
        "rdtsc\n\t"
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "subq %1, %%rax\n\t"
        "cmpq $100000, %%rax\n\t"
        "seta %%al\n\t"
        "movzbl %%al, %0\n\t"
        : "=r"(result), "=r"(start) : : "%rax", "%rdx", "cc", "memory");
    return result;
}

// 🔥 ТЕСТОВЫЙ АЛЛОКАТОР
void *safe_malloc(size_t size)
{
    // МАКРОСЫ (ЛЕГКО ОБОЙТИ)
    if (MALLOC_DEBUG)
    {
        printf("🔴 MACRO: Debug mode detected (but easy to bypass!)\n");
        return NULL;
    }

    if (DEBUG_MODE)
    {
        printf("🔴 MACRO: DEBUG defined (but easy to bypass!)\n");
        return NULL;
    }

    // ТВОЙ АНТИ-ДЕБАГ (РЕАЛЬНАЯ ЗАЩИТА)
    if (get_malloc_debug_flag())
    {
        printf("🚨 ANTIDEBUG: Debugger detected! Crashing...\n");
        *((volatile int *)0) = 1; // Segmentation fault
        return NULL;
    }

    printf("✅ SAFE: Normal allocation\n");
    return malloc(size);
}

// 🎪 ФУНКЦИЯ-ПРОВОКАЦИЯ ДЛЯ ОТЛАДЧИКА
void provoke_debugger()
{
    printf("=== PROVOKING DEBUGGER ===\n");
    for (volatile int i = 0; i < 1000; i++)
    {
    } // Место для брейкпоинта
}

int main()
{
    printf("=== BENCHMARK: MACROS vs ANTIDEBUG ===\n\n");

    // ТЕСТ 1: Нормальное выполнение
    printf("📊 TEST 1: Normal execution\n");
    safe_malloc(100);

    // ТЕСТ 2: С макросами
    printf("\n📊 TEST 2: With debug macros\n");
    printf("Try: gcc -DMALLOC_DEBUG=1 -DDEBUG benchmark_antidebug.c\n");

    // ТЕСТ 3: Под отладчиком
    printf("\n📊 TEST 3: Under debugger\n");
    provoke_debugger();
    safe_malloc(100);

    return 0;
}