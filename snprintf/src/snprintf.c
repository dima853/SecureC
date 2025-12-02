#include <stddef.h>

static inline int has_percent_asm_x64(const char *str) {
    int result = 0;

    __asm__ volatile(
        // prepare (\t = tabulation)
        "xor %%eax, %%eax\n\t" // zeroing out
        "mov $0x25, %%dl\n\t" //  0x25 = ASCII '%'

        // section 1
        "1:\n\t"
        /*
        () like *ptr (looking at the content) 
        
        cl = the lowest byte of rcx
        */
        "movb (%%rdi), %%cl\n\t" // like char current_char = *str;
        // test byte
        "testb %%cl, %%cl\n\t" // like if (current_char == '\0')
        // jump if zero (if 1)
        /* 3f (forward)
        3f = search for label 3: after the current position
        3b = search for label 3: in front of the current position
        */
        "jz 3f\n\t" // like if (current_char == '\0') goto end;
        // 'zf' = 1 if ==, 'cf' if cl < dl
        "cmpb %%dl, %%cl\n\t" // compare byte \\ like if (current_char == '%')
        "je 2f\n\t" // jump if equal \\ like if (current_char == '%') goto found
        "inc %%rdi\n\t" // str++
        "jmp 1b\n\t" // like continue

        "2:\n\t"
        "mov $1, %%eax\n\t" // like result = 1

        "3:\n\t"
        "mov %%eax, %0\n\t"
        : "=r" (result)
        : "D" (str)
        : "%rax", "%rcx", "%rdx", "cc", "memory"
    );

    return result;
}

static inline int has_percent_asm_arm64(const char *str) {
    int result = 0;

    __asm__ volatile(
        // prepare (\t = tabulation)
        "mov w0, #0\n\t"              // zeroing out \\ w0 = 0 (result = false)
        "mov w1, #0x25\n\t"           // 0x25 = ASCII '%' \\ w1 = '%'

        // section 1
        "1:\n\t"
        /*
        ldrb = Load Register Byte 
        [] like *ptr (looking at the content) 
        
        w2 = 32-bit register (lower half of x2)
        */
        "ldrb w2, [%1]\n\t"           // like char current_char = *str;
        /*
        cbz = Compare and Branch on Zero
        cbz w2, label = if (w2 == 0) goto label
        */
        "cbz w2, 3f\n\t"              // like if (current_char == '\0') goto end;
        /*
        cmp = Compare
        cmp w2, w1 = compare w2 with w1, sets condition flags
        */
        "cmp w2, w1\n\t"              // compare \\ like if (current_char == '%')
        /*
        b.eq = Branch if Equal 
        Condition codes from cmp instruction:
        - eq (equal) = Z flag set (1)
        - ne (not equal) = Z flag clear (0)
        */
        "b.eq 2f\n\t"                 // jump if equal \\ like if (current_char == '%') goto found
        /*
        add = Add
        add %1, %1, #1 = %1 = %1 + 1
        #1 = immediate value 1
        */
        "add %1, %1, #1\n\t"          // str++ \\ pointer increment
        /*
        b = Branch 
        1b = backward to label 1: (искать метку 1: перед текущей позицией)
        */
        "b 1b\n\t"                    // like continue \\ goto loop_start

        "2:\n\t"
        "mov w0, #1\n\t"              // like result = 1 \\ w0 = 1

        "3:\n\t"
        /*
        mov %0, w0 = copy w0 to output operand %0
        %0 refers to the first output operand: "=r" (result)
        */
        "mov %0, w0\n\t"              // store result
        /*
        OUTPUT operands 
        "=r" = write-only, any register
        (result) = C variable to store the result
        */
        : "=r" (result)
        /*
        INPUT operands 
        "r" = any register
        (str) = C variable containing the string pointer
        ARM calling convention: first argument in x0
        But we use generic "r" constraint
        */
        : "r" (str)
        /*
        CLOBBER list 
        "w0", "w1", "w2" = 32-bit registers we modify
        "cc" = condition codes (flags) are modified
        "memory" = memory may be modified
        */
        : "w0", "w1", "w2", "cc", "memory"
    );

    return result;
}