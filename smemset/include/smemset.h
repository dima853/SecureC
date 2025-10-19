#pragma once
#ifndef SMEMSET_H
#define SMEMSET_H

#include <stddef.h>

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

// ==================== Safe Functions ====================
void *smemset_ex(void *dstpp, int c, size_t len, SmemsetContext context);
void *smemset_limited(void *dstpp, int c, size_t len, SmemsetLimit limit);
size_t smemset_get_max_size(SmemsetContext context);

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

#ifdef __cplusplus
}
#endif

#endif // SMEMSET_H
