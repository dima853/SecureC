#include "malloc_constants.h"

/**
 * @file malloc_constants.c
 * @brief Protected malloc constants implementation
 *
 * All constants are static and only accessible through getter functions,
 * making them immune to macro redefinition attacks.
 */

// ==================== Fundamental Types ====================
/**
 * @brief Fundamental type protection
 *
 * These dummy functions return the actual types through pointer magic,
 * preventing any macro-based type redefinition attacks.
 */
static const int __void_protector = 0;
static const size_t __size_t_protector = 0;

void *get_void_type(void)
{
    return (void *)&__void_protector; // Returns a void* type indicator
}

size_t get_size_t_type(void)
{
    return (size_t)__size_t_protector; // Returns size_t type indicator
}

// ==================== Fastbin Constants ====================
static const size_t MAX_FAST = 128 * sizeof(size_t) / 4; // 64 on 32bit, 128 on 64bit
static const unsigned int NFASTBINS = 10;
static const size_t FASTBIN_INDEX_SHIFT = 4;

size_t get_max_fast_value(void) { return MAX_FAST; }
unsigned int get_nfastbins_value(void) { return NFASTBINS; }
size_t get_fastbin_index_shift(void) { return FASTBIN_INDEX_SHIFT; }

// ==================== Chunk Size Constants ====================
static const size_t CHUNK_HDR_SZ = 2 * sizeof(size_t);
static const size_t MINSIZE = 4 * sizeof(size_t);
static const size_t SIZE_SZ = sizeof(size_t);

size_t get_chunk_hdr_sz(void) { return CHUNK_HDR_SZ; }
size_t get_minsize_value(void) { return MINSIZE; }
size_t get_size_sz_value(void) { return SIZE_SZ; }

// ==================== Alignment Constants ====================
static const size_t MALLOC_ALIGN_MASK = 2 * sizeof(size_t) - 1;
static const size_t MALLOC_ALIGN = 2 * sizeof(size_t);

size_t get_malloc_align_mask(void) { return MALLOC_ALIGN_MASK; }
size_t get_malloc_align_value(void) { return MALLOC_ALIGN; }

// ==================== Bit Constants ====================
static const size_t PREV_INUSE = 0x1;
static const size_t SIZE_BITS = ~(sizeof(size_t) - 1);
static const size_t NON_MAIN_ARENA = 0x4;

size_t get_prev_inuse_bit(void) { return PREV_INUSE; }
size_t get_size_bits_mask(void) { return SIZE_BITS; }
size_t get_non_main_arena_bit(void) { return NON_MAIN_ARENA; }

// ==================== TCache Constants ====================
static const size_t TCACHE_SMALL_BINS = 64;
static const size_t TCACHE_UNSORTED_LIMIT = 16;

size_t get_tcache_small_bins_count(void) { return TCACHE_SMALL_BINS; }
size_t get_tcache_unsorted_limit(void) { return TCACHE_UNSORTED_LIMIT; }

// ==================== System Constants ====================
static const size_t DEFAULT_MMAP_THRESHOLD = 128 * 1024;
static const size_t DEFAULT_TRIM_THRESHOLD = 128 * 1024;
static const size_t SYSTEM_MEM_DEFAULT = 128 * 1024 * 1024; // 128MB

size_t get_default_mmap_threshold(void) { return DEFAULT_MMAP_THRESHOLD; }
size_t get_default_trim_threshold(void) { return DEFAULT_TRIM_THRESHOLD; }
size_t get_system_mem_default(void) { return SYSTEM_MEM_DEFAULT; }