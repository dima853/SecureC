#pragma once
#ifndef S_MALLOC
#define S_MALLOC

/*
use
#if get_malloc_debug_flag()

#endif
*/
__attribute__((always_inline)) static inline int get_malloc_debug_flag()
{
    int result;

    __asm__ volatile(
        "movl $1, %%eax\n\t"
        "movl %%eax, %0\n\t"
        : "=r"(result)
        :
        : "%eax");

    return result;
}

#endif