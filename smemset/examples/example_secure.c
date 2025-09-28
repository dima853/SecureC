#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "smemset.h"

void secure_password_handling()
{
    printf("=== Secure Password Handling ===\n\n");

    char password[128];
    printf("Please enter your password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    printf("Password received: %s\n", password);
    printf("Processing...\n");
    printf("Authenticating...\n");

    printf("Clearing password from memory...\n");
    smemset(password, 0, sizeof(password));

    printf("Password securely cleared.\n");

    int is_secure = 1;
    for (size_t i = 0; i < sizeof(password); i++)
    {
        if (password[i] != 0)
        {
            is_secure = 0;
            printf("❌ SECURITY WARNING at byte %zu!\n", i);
            break;
        }
    }

    if (is_secure)
    {
        printf("✅ Password memory securely wiped.\n");
    }
}

void cryptographic_key_management()
{
    printf("\n=== Cryptographic Key Management ===\n\n");

    unsigned char encryption_key[32];
    printf("Generating encryption key...\n");

    for (int i = 0; i < 32; i++)
    {
        encryption_key[i] = rand() % 256;
    }

    printf("Key generated: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%02X", encryption_key[i]);
    }
    printf("...\n");

    printf("Encrypting data with key...\n");

    printf("Destroying encryption key...\n");
    smemset(encryption_key, 0, sizeof(encryption_key));
    smemset(encryption_key, 0xFF, sizeof(encryption_key));
    smemset(encryption_key, 0xAA, sizeof(encryption_key));
    smemset(encryption_key, 0, sizeof(encryption_key));

    printf("Encryption key securely destroyed.\n");
}

void secure_session_cleanup()
{
    printf("\n=== Secure Session Cleanup ===\n\n");

    typedef struct
    {
        char session_id[64];
        char user_token[256];
        char temporary_keys[512];
        char sensitive_cache[1024];
    } session_data_t;

    session_data_t *session = malloc(sizeof(session_data_t));
    if (!session)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(session->session_id, "SESS_1234567890abcdef");
    strcpy(session->user_token, "TOKEN_xyz9876543210");
    strcpy(session->temporary_keys, "TEMP_KEYS_HERE");
    strcpy(session->sensitive_cache, "CACHED_SENSITIVE_DATA");

    printf("Session data created:\n");
    printf("  Session ID: %s\n", session->session_id);
    printf("  User Token: %.20s...\n", session->user_token);

    printf("Session in progress...\n");

    printf("Ending session with secure cleanup...\n");
    smemset(session, 0, sizeof(session_data_t));
    free(session);

    printf("✅ Session securely terminated.\n");
}

int main()
{
    printf("🔒 smemset Security Examples\n\n");
    secure_password_handling();
    cryptographic_key_management();
    secure_session_cleanup();
    printf("\n🎯 Security examples completed!\n");
    return 0;
}