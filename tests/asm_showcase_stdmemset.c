#include <stdio.h>
#include "../include/smemset.h"
#include <string.h>

// Те же тесты но с обычным memset
void test_memset_clear()
{
    char password[64] = "MySecretPassword123";
    printf("Before: %s\n", password);

    // Обычный memset
    memset(password, 0, sizeof(password));

    printf("After: [CLEARED]\n");
}

void test_memset_with_calc()
{
    char buffer[100];
    int size = sizeof(buffer);

    for (int i = 0; i < size; i++)
    {
        buffer[i] = 'A' + (i % 26);
    }

    printf("Buffer filled: %.10s...\n", buffer);

    // Обычный memset
    memset(buffer, 'X', size);

    printf("Buffer after: %.10s...\n", buffer);
}

int main()
{
    printf("🔍 ASM Showcase for STANDARD memset\n\n");

    test_memset_clear();
    test_memset_with_calc();

    return 0;
}