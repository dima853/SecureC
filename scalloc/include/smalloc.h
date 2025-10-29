#pragma once
#ifndef S_MALLOC
#define S_MALLOC

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for getenv(), abort() */
#include <unistd.h> /* for __libc_enable_secure */

#include <atomic.h>
#include <_itoa.h>
#include <bits/wordsize.h>
#include <sys/sysinfo.h>

#include <ldsodefs.h>
#include <setvmaname.h>

#include <unistd.h>
#include <stdio.h> /* needed for malloc_stats */
#include <errno.h>
#include <assert.h>
#include <intprops.h>

#include <shlib-compat.h>

/* For uintptr_t.  */
#include <stdint.h>

/* For stdc_count_ones.  */
#include <stdbit.h>

/* For va_arg, va_start, va_end.  */
#include <stdarg.h>

/* For MIN, MAX, powerof2.  */
#include <sys/param.h>

/* For ALIGN_UP et. al.  */
#include <libc-pointer-arith.h>

/* For DIAG_PUSH/POP_NEEDS_COMMENT et al.  */
#include <libc-diag.h>

/* For memory tagging.  */
#include <libc-mtag.h>

#include <malloc/malloc-internal.h>

/* For SINGLE_THREAD_P.  */
#include <sysdep-cancel.h>

#include <libc-internal.h>

/* For tcache double-free check.  */
#include <random-bits.h>
#include <sys/random.h>
#include <not-cancel.h>

// /*
//   void* is the pointer type that malloc should say it returns (defense)
// */

// #ifndef void
// #define void void
// #endif /*void*/

// ⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣

// if (get_malloc_debug_flag())
// {
//     /* secure debug code */
//     __asm__ volatile("" : : : "memory"); /* Compiler barrier */
// }

/*
use
#if get_malloc_debug_flag()

#endif
*/
__attribute__((always_inline)) static inline int get_malloc_debug_flag(void)
{
    int result = 0;
    uint64_t start, end;

    __asm__ volatile(
        // read time stamp counter
        /*
        The Time Stamp Counter (TSC) is a 64-bit register present on all x86 processors since the Pentium.【It counts the number of CPU cycles since its reset】
        The instruction RDTSC returns the TSC in EDX:EAX. In x86-64 mode, RDTSC also clears the upper 32 bits of RAX and RDX.
        Its opcode is 0F 31.[1] Pentium competitors such as the Cyrix 6x86 did not always have a TSC and may consider RDTSC an illegal instruction.
        Cyrix included a Time Stamp Counter in their MII.
        */
        "rdtsc\n\t"            // EAX = младшие 32 бита, EDX = старшие 32
        "shlq $32, %%rdx\n\t"  // Сдвигаем старшую часть на место
        "orq %%rdx, %%rax\n\t" // Объединяем в RAX
        "movq %%rax, %1\n\t"   // Сохраняем start
        "rdtsc\n\t"            // Повторяем для end
        "shlq $32, %%rdx\n\t"
        "orq %%rdx, %%rax\n\t"
        "subq %1, %%rax\n\t"    // Разница = end - start
        "cmpq $5000, %%rax\n\t" // Порог для отладчика (увеличил)
        "seta %0\n\t"           // result = 1 если delta > 5000
        : "=r"(result), "=r"(start)
        :
        : "%rax", "%rdx", "cc", "memory");

    return result;
}

#endif