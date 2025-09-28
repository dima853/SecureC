#include <stdio.h>
#include <string.h>
#include "smemset.h"

void demonstrate_basic_usage()
{
    printf("=== Basic smemset Usage ===\n\n");

    printf("1. Basic Filling:\n");
    char buffer[50];
    smemset(buffer, 'A', sizeof(buffer));
    printf("   Filled with 'A': %.10s...\n", buffer);

    printf("\n2. Zero Clear:\n");
    int numbers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("   Before: ");
    for (int i = 0; i < 10; i++)
        printf("%d ", numbers[i]);

    smemset(numbers, 0, sizeof(numbers));

    printf("\n   After:  ");
    for (int i = 0; i < 10; i++)
        printf("%d ", numbers[i]);
    printf("\n");

    printf("\n3. Partial Fill:\n");
    char text[] = "Hello, World!";
    printf("   Before: %s\n", text);
    smemset(text + 7, '*', 5);
    printf("   After:  %s\n", text);
}

void demonstrate_data_structures()
{
    printf("\n=== Working with Data Structures ===\n\n");

    struct user_credentials
    {
        char username[32];
        char password[64];
        char token[128];
    };

    struct user_credentials user;
    strcpy(user.username, "john_doe");
    strcpy(user.password, "SuperSecretPassword123");
    strcpy(user.token, "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...");

    printf("Before clearing:\n");
    printf("  Username: %s\n", user.username);
    printf("  Password: %s\n", user.password);
    printf("  Token: %.20s...\n", user.token);

    smemset(&user, 0, sizeof(user));

    printf("\nAfter smemset:\n");
    printf("  Username: %s\n", user.username[0] ? user.username : "[CLEARED]");
    printf("  Password: %s\n", user.password[0] ? user.password : "[CLEARED]");
    printf("  Token: %s\n", user.token[0] ? user.token : "[CLEARED]");

    int is_cleared = 1;
    for (size_t i = 0; i < sizeof(user); i++)
    {
        if (((char *)&user)[i] != 0)
        {
            is_cleared = 0;
            break;
        }
    }

    printf("\nSecurity verification: %s\n",
           is_cleared ? "✅ PASSED" : "❌ FAILED");
}

int main()
{
    printf("🔧 smemset Basic Examples\n\n");
    demonstrate_basic_usage();
    demonstrate_data_structures();
    printf("\n✨ Example completed successfully!\n");
    return 0;
}