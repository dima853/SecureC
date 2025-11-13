#pragma once

#include "/mnt/c/Users/dmako/safe_repo/sgoto/third-party/ephemeral-hash-cas/src/core/include/core.h"

// Secure GOTO structure definition
typedef struct
{
    uint16_t curr_hash;
    void *core_addr;
    void *goto_addrs[16];
    uint16_t goto_hashes[16];
    uint8_t child_count;
} sgoto_t;

// External global hash generator
extern HashGenerator global_hash_gen;

// Function declarations
void sgoto_init(uint64_t seed);
sgoto_t *sg_init(void *cleanup_addr);
void **sgoto_get_label_addresses(void);

// Secure GOTO integrity verification function
__attribute__((always_inline)) static inline int sgoto_check_place(sgoto_t *sg)
{
    int result = 0;

    // Get label addresses FIRST (they contain the actual addresses)
    void **label_addrs = sgoto_get_label_addresses();

    // Inline assembly block for low-level memory integrity verification
    __asm__ volatile(
        // ==================== CLEANUP ADDRESS VERIFICATION ====================
        // Verify that the cleanup handler address hasn't changed in memory
        "mov %[cleanup_addr], %%rax\n"   // Load expected cleanup address into RAX
        "mov %[actual_cleanup], %%rdx\n" // Load actual cleanup address into RDX (ИСПРАВЛЕНО: edx -> rdx)
        "cmp %%rax, %%rdx\n"             // Compare expected vs actual address
        "jne integrity_failed\n"         // If addresses don't match, jump to failure

        // ==================== GOTO ADDRESSES VERIFICATION ====================
        // Verify all goto target addresses in a loop
        "mov %[goto_count], %%ecx\n" // Load goto target count into ECX (32-bit for counter)
        "test %%ecx, %%ecx\n"        // Test if count is zero
        "jz check_success\n"         // If zero, skip goto checks

        "mov %[goto_addrs], %%rsi\n"  // Load expected goto addresses array into RSI
        "mov %[label_addrs], %%rdi\n" // Load actual label addresses array into RDI
        "add $8, %%rdi\n"             // Skip cleanup label, start from goto_label_1

        // Loop through all goto targets and verify their addresses
        "check_goto_loop:\n"
        "movq (%%rsi), %%r8\n"   // Load expected goto address into R8 (64-bit)
        "movq (%%rdi), %%r9\n"   // Load actual goto address into R9 (64-bit)
        "cmp %%r8, %%r9\n"       // Compare expected vs actual address
        "jne integrity_failed\n" // If addresses don't match, jump to failure
        "add $8, %%rsi\n"        // Move to next expected address
        "add $8, %%rdi\n"        // Move to next actual address
        "dec %%ecx\n"            // Decrement loop counter
        "jnz check_goto_loop\n"  // If counter not zero, continue loop

        // ==================== SUCCESS PATH ====================
        "check_success:\n"
        "mov $1, %[result]\n" // Set result to 1 (success)
        "jmp end\n"           // Jump to end

        // ==================== FAILURE PATH ====================
        "integrity_failed:\n"
        "mov $0, %[result]\n" // Set result to 0 (failure)

        // ==================== EXIT POINT ====================
        "end:\n"

        // ==================== ASSEMBLY OPERANDS ====================
        : [result] "=r"(result)                        // Output
        : [cleanup_addr] "r"(sg->core_addr),           // Input: expected cleanup
          [actual_cleanup] "r"(label_addrs[0]),        // Input: actual cleanup
          [goto_count] "r"((uint32_t)sg->child_count), // Input: goto count (ИСПРАВЛЕНО: приведение типа)
          [goto_addrs] "r"(sg->goto_addrs),            // Input: expected addresses
          [label_addrs] "r"(label_addrs)               // Input: actual addresses
        : "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "cc", "memory");

    return result;
}