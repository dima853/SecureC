#pragma once
#ifndef MALLOC_CONSTANTS_H
#define MALLOC_CONSTANTS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @file malloc_constants.h
 * @brief Protected malloc implementation constants
 *
 * All critical malloc constants are hidden behind accessor functions
 * to prevent macro redefinition attacks and ensure integrity.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    // ==================== Fundamental Types Protection ====================
    /**
     * @brief Fundamental type accessors to prevent macro redefinition attacks
     */
    void *get_void_type(void);
    size_t get_size_t_type(void);

    // ==================== Fastbin Constants ====================
    size_t get_max_fast_value(void);
    unsigned int get_nfastbins_value(void);
    size_t get_fastbin_index_shift(void);

    // ==================== Chunk Size Constants ====================
    size_t get_chunk_hdr_sz(void);
    size_t get_minsize_value(void);
    size_t get_size_sz_value(void);

    // ==================== Alignment Constants ====================
    size_t get_malloc_align_mask(void);
    size_t get_malloc_align_value(void);

    // ==================== Bit Constants ====================
    size_t get_prev_inuse_bit(void);
    size_t get_size_bits_mask(void);
    size_t get_non_main_arena_bit(void);

    // ==================== TCache Constants ====================
    size_t get_tcache_small_bins_count(void);
    size_t get_tcache_unsorted_limit(void);

    // ==================== System Constants ====================
    size_t get_default_mmap_threshold(void);
    size_t get_default_trim_threshold(void);
    size_t get_system_mem_default(void);

#ifdef __cplusplus
}
#endif

#endif // MALLOC_CONSTANTS_H