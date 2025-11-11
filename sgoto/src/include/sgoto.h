#pragma once

#include "C:\Users\dmako\safe_repo\sgoto\third-party\ephemeral-hash-cas\src\core\include\core.h"

// Secure GOTO structure definition
// This structure stores all the necessary information for secure goto operations
typedef struct
{
    uint16_t curr_hash;       // Current ephemeral hash for verification
    void *core_addr;          // Address of the core cleanup handler
    void *goto_addrs[16];     // Array of expected goto target addresses (max 16)
    uint16_t goto_hashes[16]; // Array of ephemeral hashes for each goto target
    uint8_t child_count;      // Number of active goto targets (0-16)
} sgoto_t;

// External global hash generator for ephemeral hash creation
extern HashGenerator global_hash_gen;

// Secure GOTO integrity verification function
// This function checks if all goto labels and cleanup addresses are unchanged in memory
// Parameters: sg - pointer to secure goto structure
// Returns: 1 if integrity verified, 0 if integrity check failed
__attribute__((always_inline)) static inline int sgoto_check_place(sgoto_t *sg)
{
    int result = 0; // Initialize result to 0 (failure)

    __asm__ volatile(
        "jmp after_labels\n"

        "cleanup_label:\n"
        "nop\n"

        "goto_label_1:\n"
        "nop\n"

        "goto_label_2:\n"
        "nop\n"

        "goto_label_3:\n"
        "nop\n"

        "after_labels:\n");

    // Inline assembly block for low-level memory integrity verification
    __asm__ volatile(
        // ==================== CLEANUP ADDRESS VERIFICATION ====================
        // Verify that the cleanup handler address hasn't changed in memory
        "mov %[cleanup_addr], %%rax\n"   // Load expected cleanup address from C variable into RAX
        "lea cleanup_label(%%), %%rdx\n" // Load actual current address of cleanup_label into RDX using LEA
        "cmp %%rax, %%rdx\n"             // Compare expected address (RAX) with actual address (RDX)
        "jne integrity_failed\n"         // If addresses don't match, jump to integrity failure handler

        // ==================== GOTO ADDRESSES VERIFICATION ====================
        // Verify all goto target addresses in a loop
        "mov %[goto_count], %%rcx\n"  // Load goto target count from C variable into RCX (loop counter)
        "mov %[goto_addrs], %%rsi\n"  // Load pointer to expected goto addresses array into RSI
        "mov %[goto_labels], %%rdi\n" // Load pointer to actual goto label addresses array into RDI

        // Loop through all goto targets and verify their addresses
        "check_goto_loop:\n"
        "mov (%%rsi), %%r8\n"    // Load expected goto address from array pointed by RSI into R8
        "mov (%%rdi), %%r9\n"    // Load actual goto address from array pointed by RDI into R9
        "cmp %%r8, %%r9\n"       // Compare expected address (R8) with actual address (R9)
        "jne integrity_failed\n" // If addresses don't match, jump to integrity failure handler
        "add $8, %%rsi\n"        // Increment RSI by 8 bytes to point to next expected address (64-bit pointer size)
        "add $8, %%rdi\n"        // Increment RDI by 8 bytes to point to next actual address (64-bit pointer size)
        "dec %%rcx\n"            // Decrement loop counter (RCX)
        "jnz check_goto_loop\n"  // If counter is not zero, jump back to check_goto_loop

        // ==================== SUCCESS PATH ====================
        // All integrity checks passed successfully
        "mov $1, %[result]\n" // Set result to 1 (success) in the output variable
        "jmp end\n"           // Jump to end label to skip failure code

        // ==================== FAILURE PATH ====================
        // Integrity verification failed (address mismatch detected)
        "integrity_failed:\n"
        "mov $0, %[result]\n" // Set result to 0 (failure) in the output variable

        // ==================== EXIT POINT ====================
        "end:\n"

        // ==================== LABELS FOR ADDRESS VERIFICATION ====================
        // These labels are used to get their actual memory addresses for verification
        // The jmp instruction skips over these labels during normal execution
        "jmp after_labels\n" // Jump over the verification labels to avoid executing them

        "cleanup_label:\n" // Label for cleanup handler address verification
        "nop\n"            // No-operation instruction as placeholder

        "goto_label_1:\n" // First goto target label for address verification
        "nop\n"           // No-operation instruction as placeholder

        "goto_label_2:\n" // Second goto target label for address verification
        "nop\n"           // No-operation instruction as placeholder

        "goto_label_3:\n" // Third goto target label for address verification
        "nop\n"           // No-operation instruction as placeholder

        "after_labels:\n" // Label marking the end of verification labels

        // ==================== ASSEMBLY OPERANDS ====================
        // Output operands: variables that the assembly code writes to
        : [result] "=r"(result) // Output: result variable in any register

        // Input operands: variables that the assembly code reads from
        : [cleanup_addr] "r"(sg->core_addr),             // Input: expected cleanup address from structure
          [goto_count] "r"(sg->child_count),             // Input: number of goto targets to verify
          [goto_addrs] "r"(sg->goto_addrs),              // Input: array of expected goto addresses
          [goto_labels] "r"(sgoto_get_label_addresses()) // Input: array of actual goto label addresses

        // Clobber list: registers and memory that the assembly code modifies
        : "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", // Registers modified by assembly code
          "cc",                                          // Condition codes (flags) are modified
          "memory"                                       // Memory is accessed and potentially modified
    );

    return result;
}

// Function to get current addresses of all verification labels
// This function creates an array containing the actual memory addresses
// of all labels used for integrity verification
// Returns: pointer to array of label addresses
void **sgoto_get_label_addresses()
{
    static void *labels[16];     // Static array to store label addresses
    labels[0] = &&cleanup_label; // Store address of cleanup_label
    labels[1] = &&goto_label_1;  // Store address of first goto label
    labels[2] = &&goto_label_2;  // Store address of second goto label
    labels[3] = &&goto_label_3;  // Store address of third goto label
    return labels;               // Return pointer to the address array
}

// Secure GOTO initialization function
// Parameters: cleanup_addr - address of the cleanup handler function
// Returns: pointer to initialized secure goto structure
sgoto_t *sg_init(void *cleanup_addr);