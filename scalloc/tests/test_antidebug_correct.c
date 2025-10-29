#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

__attribute__((always_inline)) static inline uint64_t read_tsc(void)
{
    uint64_t result;
    __asm__ volatile(
        "rdtsc\n\t"
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "movq %%rax, %0\n\t"
        : "=r"(result)
        :
        : "%rax", "%rdx", "cc", "memory");
    return result;
}

int main()
{
    printf("=== ANTIDEBUG TEST ===\n");

    uint64_t start = read_tsc();
    printf("First TSC reading taken: %lu\n", start);

    printf("=== PUT BREAKPOINT HERE AND WAIT 10 SECONDS! ===\n");
    printf("Then press 'continue' in GDB\n");

    // Второе измерение
    uint64_t end = read_tsc();
    printf("Second TSC reading taken: %lu\n", end);

    // Вычисляем разницу
    uint64_t delta = end - start;
    printf("Delta: %lu ticks\n", delta);

    // Проверяем порог
    int debugger_detected = (delta > 1000000); // 1 миллион тактов
    printf("Debugger detected: %d (1 = YES, 0 = NO)\n", debugger_detected);

    if (debugger_detected)
    {
        printf("🚨 DEBUGGER DETECTED! 🚨\n");
    }
    else
    {
        printf("✅ No debugger detected\n");
    }

    return 0;
}