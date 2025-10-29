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

// /* unsafe approach, don't use this shit
//   void* is the pointer type that malloc should say it returns (defense)
// */

// #ifndef void
// #define void void
// #endif /*void*/

// ⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣⭣

// if (get_malloc_debug_flag())
// {
//     /* secure debug code */
//     __asm__ volatile("mfence" : : : "memory"); /* Compiler barrier */
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
        // read time stamp counter (roughly speaking, just a stopwatch)
        /*
        The Time Stamp Counter (TSC) is a 64-bit register present on all x86 processors since the Pentium.【It counts the number of CPU cycles since its reset】
        The instruction RDTSC returns the TSC in EDX:EAX. In x86-64 mode, RDTSC also clears the upper 32 bits of RAX and RDX.
        Its opcode is 0F 31.[1] Pentium competitors such as the Cyrix 6x86 did not always have a TSC and may consider RDTSC an illegal instruction.
        Cyrix included a Time Stamp Counter in their MII.
        */
        "rdtsc\n\t" // EAX = low 32 bits, EDX = high 32
        //               ⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄
        /*
        EAX and EDX are
        32-bit registers in the x86 processor architecture, which are used to temporarily store data when executing instructions.
        EAX is an accumulator register, mainly used for arithmetic operations, while EDX is a data register used to store data in I/O operations or the remainder during division.

        but ⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄⌄

        RDTSC IGNORES their "specialization"!
        It just uses EAX/EDX as CONVENIENT CONTAINERS

        Why exactly these registers? Historical reasons:
        __asm__("rdtsc"); // Returns 64-bits in EDX:EAX - FIXED!
        */
        //                             -m64 = Compilation for 64-bit architectures
        //                                              ⌄⌄⌄⌄⌄
        "shlq $32, %%rdx\n\t" // shlq valid only under -m64 (the table in the docs/x86 for smalloc)
        /* //       ⭡⭡⭡⭡
        //          RDX is a 64-bit general-purpose register, part of the register d extended set, used for holding data or memory addresses
        ↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎
        note:
        __asm__ volatile(
            "movq %%rax, %%rdx\n\t" // TWO %% for registers inside asm
            : "=r"(result) // ONE% for operands (result)
            :
            : "%rax", "%rdx"
        );

        the rule:
        %%rdx - inside the assembler string

        %0, %1 - for operands (variables from C)

        %rdx - in the clobber list
        ↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎↔︎
        */
        // RAX is the 64-bit accumulator and a general-purpose register in x86-64 assembly that is used for storing data, holding return values from functions, and for arithmetic operations.
        //           ⌄⌄⌄⌄⌄
        "orq %%rdx, %%rax\n\t" // orq valid only under -m64 (the table in the docs/x86 for smalloc)
        // ⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡⭡
        /*
        "shlq$32, %%rdx\n\t" it connects and it glues "orq %%rdx, %%rax\n\t"

        ## **SEPARATION OF ROLES:**

        ### **1.  SHLQ - PREPARATION (connects with zeros):**
        ```asm
        "shlq $32, %%rdx"
        // RDX was: 0x0000000012345678
        // RDX became: 0x1234567800000000
        // ↑ high-order bits + 32 zeros
        ```

        ### **2.  ORQ - GLUING (combines halves):**
        ```asm
        "orq %%rdx, %%rax"
        // RAX = 0x0000000090ABCDEF
        // RDX = 0x1234567800000000
        // ORQ = 0x1234567890ABCDEF ← GLUED!
        ```

        ## **VISUALIZATION OF THE PROCESS:**

        ```
        STEP 1 (rdtsc):
        RDX = [00000000][12345678] ← upper 32 bits
        RAX = [00000000][90ABCDEF] ← lower 32 bits

        STEP 2 (shlq):
        RDX = [12345678][00000000] ← shifted the high-order bits into place

        STEP 3 (orq):
        RAX = [12345678][00000000] OR [00000000][90ABCDEF]
        = [12345678][90ABCDEF] ← GLUED!
        ```
        */
        /*
        MOVQ = MOVE Quadword (64-bit)
        %%rax = source (from where)
        %1 = destination (to where)

        : "=r"(result), "=r"(start), "r"(input_var) (these variables in the code will be)
        // ↑ output       ↑ output    ↑ input
        //    %0             %1          %2
        */
        "movq %%rax, %1\n\t"

        "rdtsc\n\t"             // repeat for end
        "shlq $32, %%rdx\n\t"   // shift high bits
        "orq %%rdx, %%rax\n\t"  // gluing in rax
        "subq %1, %%rax\n\t"    // subq valid only under -m64 || Difference = end - start (subq = subtract)
        "cmpq $5000, %%rax\n\t" // cmpq valid only under -m64 (cmpq = compare)
        // "seta %0\n\t"           // %0 result = 1 if delta > 5000 or %0 = result = 0 if delta <= 5000 (set byte if above\without notes)
        /*but this shit that seta works with 8-bit registers, but we're trying to write to a 32-bit variable!*/
        "seta %%al\n\t"       // Writing to 8-bit AL
        "movzbl %%al, %0\n\t" // Expand to 32 bits in result
        /*
        these are lists of operands and clobbers!
        __asm__ volatile(
            "instructions\n\t"
            : outputs // What we return to C code
            : inputs // What we take from C code
            : clobbers // What we "break" in the process
        );

        outputs:
        "=r"(result), "=r"(start)
           │ │ │ │
           │ │ │ └─ C variable start
           │ │ └─ %1 in assembler
           │ └─ C variable result
           └─ "=r" = output operand in register
        */
        : "=r"(result), "=r"(start)
        :
        /*
        RAX \ RDX
        // We explicitly change these registers:
        rdtsc // Changes RAX, RDX
        shlq // Changes RDX
        orq // Changes RAX
        // → Must tell the compiler!

        CC
        // Instructions that change flags:
        subq // Changes ZF, CF, SF, OF...
        cmpq // Changes ZF, CF
        seta // Uses flags
        // → Must specify!

        MEMORY
        // Prevents the compiler from reordering memory operations
        // Without this, the compiler might optimize like this:
        // 1. rdtsc (start)
        // 3. rdtsc (end) ← moved up!
        // 2. some code ← moved down!
        // → DELTA will be negative!
        */
        : "%rax", "%rdx", "cc", "memory");

    return result;
}

#endif