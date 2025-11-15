#pragma once

typedef struct
{
    void *core_addr;
    void *goto_addrs[16];
    uint8_t child_count;
} sgoto_session_t;

#define SGOTO(session, index) sgoto_perform(session, index)

sgoto_session_t *sgoto_init_session(void);