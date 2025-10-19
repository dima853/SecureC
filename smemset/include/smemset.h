#pragma once
#ifndef SMEMSET_H
#define SMEMSET_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== Main function ====================
void *smemset(void *dstpp, int c, size_t len);

#ifdef REPLACE_STANDARD_MEMSET
#undef memset
#define memset smemset
#endif

// ==================== Contexts for secure memset ====================
typedef enum {
    SMEMSET_CONTEXT_GENERIC, // General case
    SMEMSET_CONTEXT_ETHERNET, // Ethernet frames
    SMEMSET_CONTEXT_IP, // IP packets  
    SMEMSET_CONTEXT_UDP, // UDP datagrams
    SMEMSET_CONTEXT_TCP, // TCP segments
    SMEMSET_CONTEXT_SNMP,        // SNMP PDU
    SMEMSET_CONTEXT_USER // User limit
} SmemsetContext;

// ==================== Typed limits ====================
typedef struct {
    size_t max_frame_size;
    size_t max_payload_size;
} EthernetLimits;

typedef struct {
    size_t max_packet_size; 
    size_t max_header_size;
    size_t default_mtu;
} IpLimits;

// ==================== External constants (declaration) ====================
extern const EthernetLimits ETH_LIMITS;
extern const IpLimits IP_LIMITS;

// ==================== Advanced secure features ====================

/**
* @brief Secure memset with context check
 * @param dstpp Buffer pointer
 * @param c Value to fill in
 * @param len Length to fill in
 * @param context Usage context
 * @return Pointer to dstpp or NULL in case of error
*/
void *smemset_ex(void *dstpp, int c, size_t len, SmemsetContext context);

/**
* @brief Secure memset with user limit
 * @param dstpp Buffer pointer
 * @param c Value to fill in  
 * @param len Length to fill in
 * @param max_allowed is the maximum allowed length for this context
 * @return Pointer to dstpp or NULL in case of error
*/
void *smemset_limited(void *dstpp, int c, size_t len, size_t max_allowed);

/**
* @brief Get the maximum size for the context
 * @param context Usage context
 * @return Maximum allowed size
*/
size_t smemset_get_max_size(SmemsetContext context);

// ==================== Macros for convenience ====================

// Fast secure memset for known contexts
#define SMEMSET_ETH(dst, c, len) smemset_ex(dst, c, len, SMEMSET_CONTEXT_ETHERNET)
#define SMEMSET_IP(dst, c, len) smemset_ex(dst, c, len, SMEMSET_CONTEXT_IP) 
#define SMEMSET_SNMP(dst, c, len) smemset_ex(dst, c, len, SMEMSET_CONTEXT_SNMP)

#ifdef __cplusplus
}
#endif

#endif // SMEMSET_H
