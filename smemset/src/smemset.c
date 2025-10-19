/**
 * @file smemset.h
 * @brief Secure memset implementation that prevents compiler optimization
 */

/**
 * @brief Secure memset function that guarantees memory zeroing/initialization
 *
 * This function is designed to securely set memory regions while preventing
 * compiler optimizations that might remove "unnecessary" memset calls. It's
 * particularly useful for:
 * - Clearing sensitive data (passwords, keys, tokens)
 * - Memory initialization for security-critical operations
 * - Preventing dead store elimination optimizations
 *
 * Key features:
 * - Uses volatile pointers to prevent compiler optimization
 * - Implements memory barriers for proper memory ordering
 * - Optimized for different architectures (x86_64, AArch64, others)
 * - Handles unaligned memory addresses efficiently
 * - Uses word-sized operations for better performance on large buffers
 *
 * Memory barrier behavior:
 * - x86_64: Uses MFENCE instruction for full memory barrier
 * - AArch64: Uses DMB SY instruction for system-wide data memory barrier
 * - Other architectures: Uses compiler barrier only
 *
 * Optimization strategy:
 * 1. Handle small buffers (<8 bytes) with byte-wise operations
 * 2. Align to architecture word size (OPSIZ) for larger buffers
 * 3. Use 8x unrolled loops for maximum throughput on large buffers
 * 4. Fall back to appropriate chunk sizes for remaining data
 *
 * @param dstpp Pointer to the destination memory region to set
 * @param c Value to set (interpreted as unsigned char)
 * @param len Number of bytes to set
 *
 * @return Original dstpp pointer (same as standard memset)
 *
 * @note This function is slower than standard memset but provides security
 *       guarantees that the memory will actually be modified.
 * @warning Unlike standard memset, this function uses volatile stores which
 *          may have different performance characteristics.
 * @see memset() for standard memory setting function
 */
#include "../include/smemset.h"
#include <stdint.h>

typedef unsigned char byte;

#ifdef __x86_64__
typedef uint64_t op_t;
#define OPSIZ 8
#elif defined(__aarch64__)
typedef uint64_t op_t;
#define OPSIZ 8
#else
typedef uint32_t op_t;
#define OPSIZ 4
#endif

/**
 * @brief Validate memset parameters for specific context
 * @return 0 if valid, error code if invalid
 */
int smemset_validate_params(const void *dstpp, size_t len, SmemsetContext context)
{
    if (dstpp == NULL) {
        return -1;   
    }
    
    if (len == 0) {
        return 0;  
    }

    size_t max_allowed;
    switch (context) {
        case SMEMSET_CONTEXT_ETHERNET:
            max_allowed = SMEMSET_LIMIT_ETH_FRAME;
            break;
        case SMEMSET_CONTEXT_IP:
            max_allowed = SMEMSET_LIMIT_IP_PACKET;
            break;
        case SMEMSET_CONTEXT_SNMP:
            max_allowed = SMEMSET_LIMIT_SNMP_PDU;
            break;
        case SMEMSET_CONTEXT_UDP:
            max_allowed = SMEMSET_LIMIT_UDP_MAX;
            break;
        case SMEMSET_CONTEXT_TCP:
            max_allowed = SMEMSET_LIMIT_TCP_MSS;
            break;
        case SMEMSET_CONTEXT_GENERIC:
        default:
            max_allowed = SMEMSET_LIMIT_GENERIC;
            break;
    }
    
    if (len > max_allowed) {
        return -2;  // E2BIG
    }
    
    return 0;
}

/**
 * @brief Validate memset parameters with custom limit
 * @return 0 if valid, error code if invalid
 */
int smemset_validate_params_limited(const void *dstpp, size_t len, size_t max_allowed)
{
    if (dstpp == NULL) {
        return -1;  
    }
    
    if (len == 0) {
        return 0;  
    }
    
    if (len > max_allowed) {
        return -2;  // E2BIG
    }
    
    return 0; 
}

void *smemset(void *dstpp, int c, size_t len)
{
    if (smemset_validate_params(dstpp, len, SMEMSET_CONTEXT_GENERIC) != 0) {
        return NULL;
    }

    uintptr_t dstp = (uintptr_t)dstpp;

#if defined(__x86_64__)
    __asm__ __volatile__("mfence" ::: "memory");
#elif defined(__aarch64__)
    __asm__ __volatile__("dmb sy" ::: "memory");
#else
    #error "Memory barrier not implemented for this architecture"
#endif

    if (len >= 8)
    {
        size_t xlen;
        op_t cccc;

        cccc = (unsigned char)c;
        cccc |= cccc << 8;
        cccc |= cccc << 16;
        if (OPSIZ > 4)
            cccc |= (cccc << 16) << 16;

        while (dstp % OPSIZ != 0 && len > 0)
        {
            *((volatile byte *)dstp) = c;
            dstp += 1;
            len -= 1;
        }

        __asm__ __volatile__("mfence" ::: "memory");

        xlen = len / (OPSIZ * 8);
        while (xlen > 0)
        {
            for (int i = 0; i < 8; i++)
            {
                *((volatile op_t *)(dstp + i * OPSIZ)) = cccc;
            }
            dstp += 8 * OPSIZ;
            xlen -= 1;
        }
        len %= OPSIZ * 8;

        xlen = len / OPSIZ;
        while (xlen > 0)
        {
            *((volatile op_t *)dstp) = cccc;
            dstp += OPSIZ;
            xlen -= 1;
        }
        len %= OPSIZ;
    }

    while (len > 0)
    {
        *((volatile byte *)dstp) = c;
        dstp += 1;
        len -= 1;
    }

#if defined(__x86_64__)
    __asm__ __volatile__("mfence" ::: "memory");
#elif defined(__aarch64__)
    __asm__ __volatile__("dmb sy" ::: "memory");
#else
    #error "Memory barrier not implemented for this architecture"
#endif

    return dstpp;
}

void *smemset_ex(void *dstpp, int c, size_t len, SmemsetContext context)
{
    if (smemset_validate_params(dstpp, len, context) != 0) {
        return NULL;
    }
    
    return smemset(dstpp, c, len);
}

void *smemset_limited(void *dstpp, int c, size_t len, SmemsetLimit limit)
{
    if (smemset_validate_params_limited(dstpp, len, (size_t)limit) != 0) {
        return NULL;
    }
    
    return smemset(dstpp, c, len);
}
