#include <stdio.h>
#include <stdint.h>

// 🔴 ТВОЯ ФУНКЦИЯ С ИСКУССТВЕННОЙ ЗАДЕРЖКОЙ
__attribute__((always_inline)) static inline int get_malloc_debug_flag(void)
{
    int result = 0;
    uint64_t start;

    __asm__ volatile(
        "rdtsc\n\t"
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "movq %%rax, %1\n\t"

        // 🔴 ИСКУССТВЕННАЯ ЗАДЕРЖКА ДЛЯ ТЕСТА
        "movq $100000000, %%rcx\n\t"
        "1:\n\t"
        "decq %%rcx\n\t"
        "jnz 1b\n\t"
        // 🔴 КОНЕЦ ЗАДЕРЖКИ

        "rdtsc\n\t"
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "subq %1, %%rax\n\t"
        "cmpq $1000000, %%rax\n\t"
        "seta %%al\n\t"
        "movzbl %%al, %0\n\t"
        : "=r"(result), "=r"(start)
        :
        : "%rax", "%rdx", "%rcx", "cc", "memory");

    return result;
}

int main()
{
    printf("=== TEST YOUR ANTIDEBUG FUNCTION ===\n");

    // 🔴 ПЕРВЫЙ ВЫЗОВ - без GDB
    int normal = get_malloc_debug_flag();
    printf("Normal execution: %d (should be 0)\n", normal);

    // 🔴 ВТОРОЙ ВЫЗОВ - под GDB с брейкпоинтом ВНУТРИ функции
    printf("Now run under GDB and put breakpoint INSIDE the function!\n");
    int under_debugger = get_malloc_debug_flag();
    printf("Under debugger: %d (should be 1)\n", under_debugger);

    if (under_debugger)
    {
        printf("🚨 YOUR FUNCTION WORKS! IT DETECTED DEBUGGER! 🚨\n");
    }

    return 0;
}