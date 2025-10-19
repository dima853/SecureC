#pragma once
#ifndef SMEMSET_H
#define SMEMSET_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *smemset(void *dstpp, int c, size_t len);

#ifdef REPLACE_STANDARD_MEMSET
#undef memset
#define memset smemset
#endif

// ==================== Typed Limits ====================
typedef enum {
    SMEMSET_LIMIT_ETH_FRAME = 1518U,
    SMEMSET_LIMIT_ETH_PAYLOAD = 1500U,  
    SMEMSET_LIMIT_IP_PACKET = 65535U,
    SMEMSET_LIMIT_SNMP_PDU = 1472U,
    SMEMSET_LIMIT_UDP_MAX = 65507U,
    SMEMSET_LIMIT_TCP_MSS = 1460U
} SmemsetLimit;

// ==================== Context Types ====================
typedef enum {
    SMEMSET_CONTEXT_GENERIC,
    SMEMSET_CONTEXT_ETHERNET,
    SMEMSET_CONTEXT_IP,
    SMEMSET_CONTEXT_UDP,
    SMEMSET_CONTEXT_TCP, 
    SMEMSET_CONTEXT_SNMP
} SmemsetContext;

// ==================== Context Aliases ====================
static const SmemsetContext SMEMSET_CTX_ETHERNET = SMEMSET_CONTEXT_ETHERNET;
static const SmemsetContext SMEMSET_CTX_IP = SMEMSET_CONTEXT_IP;
static const SmemsetContext SMEMSET_CTX_UDP = SMEMSET_CONTEXT_UDP;
static const SmemsetContext SMEMSET_CTX_TCP = SMEMSET_CONTEXT_TCP;
static const SmemsetContext SMEMSET_CTX_SNMP = SMEMSET_CONTEXT_SNMP;

// ==================== Limit Aliases ====================
static const SmemsetLimit SMEMSET_MAX_ETH_FRAME = SMEMSET_LIMIT_ETH_FRAME;
static const SmemsetLimit SMEMSET_MAX_ETH_PAYLOAD = SMEMSET_LIMIT_ETH_PAYLOAD;
static const SmemsetLimit SMEMSET_MAX_IP_PACKET = SMEMSET_LIMIT_IP_PACKET;
static const SmemsetLimit SMEMSET_MAX_SNMP_PDU = SMEMSET_LIMIT_SNMP_PDU;
static const SmemsetLimit SMEMSET_MAX_UDP = SMEMSET_LIMIT_UDP_MAX;
static const SmemsetLimit SMEMSET_MAX_TCP_MSS = SMEMSET_LIMIT_TCP_MSS;

// ==================== Inline Validation Functions ====================

/**
 * @brief Validate memset parameters for specific context
 * @return 0 if valid, error code if invalid
 */
static inline int smemset_validate_params(const void *dstpp, size_t len, SmemsetContext context)
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
            max_allowed = 1048576U; // 1MB generic limit
            break;
    }
    
    if (len > max_allowed) {
        return -2;
    }
    
    return 0;
}

/**
 * @brief Validate memset parameters with custom limit
 * @return 0 if valid, error code if invalid
 */
static inline int smemset_validate_params_limited(const void *dstpp, size_t len, size_t max_allowed)
{
    if (dstpp == NULL) {
        return -1;  
    }
    
    if (len == 0) {
        return 0;  
    }
    
    if (len > max_allowed) {
        return -2;
    }
    
    return 0; 
}

/**
 * @brief Get maximum size for context
 */
static inline size_t smemset_get_max_size(SmemsetContext context)
{
    switch (context) {
        case SMEMSET_CONTEXT_ETHERNET: return SMEMSET_LIMIT_ETH_FRAME;
        case SMEMSET_CONTEXT_IP: return SMEMSET_LIMIT_IP_PACKET;
        case SMEMSET_CONTEXT_SNMP: return SMEMSET_LIMIT_SNMP_PDU;
        case SMEMSET_CONTEXT_UDP: return SMEMSET_LIMIT_UDP_MAX;
        case SMEMSET_CONTEXT_TCP: return SMEMSET_LIMIT_TCP_MSS;
        case SMEMSET_CONTEXT_GENERIC:
        default: return 1048576U;
    }
}

// ==================== Inline Safe Functions ====================

/**
 * @brief Secure memset with context validation
 */
static inline void *smemset_ex(void *dstpp, int c, size_t len, SmemsetContext context)
{
    if (smemset_validate_params(dstpp, len, context) != 0) {
        return NULL;
    }
    
    return smemset(dstpp, c, len);
}

/**
 * @brief Secure memset with custom limit validation
 */
static inline void *smemset_limited(void *dstpp, int c, size_t len, SmemsetLimit limit)
{
    if (smemset_validate_params_limited(dstpp, len, (size_t)limit) != 0) {
        return NULL;
    }
    
    return smemset(dstpp, c, len);
}

#ifdef __cplusplus
}
#endif

#endif // SMEMSET_H
