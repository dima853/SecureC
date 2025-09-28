#include <stdio.h>
#include "../include/smemset.h"

// Простая функция которая использует smemset
void test_smemset_clear()
{
    char password[64] = "MySecretPassword123";
    printf("Before: %s\n", password);

    // ВАЖНО: этот вызов должен остаться в ассемблере
    smemset(password, 0, sizeof(password));

    printf("After: [CLEARED]\n");
}

// Функция с небольшими вычислениями
void test_smemset_with_calc()
{
    char buffer[100];
    int size = sizeof(buffer);

    // Немного вычислений чтобы компилятор не всё упростил
    for (int i = 0; i < size; i++)
    {
        buffer[i] = 'A' + (i % 26);
    }

    printf("Buffer filled: %.10s...\n", buffer);

    // Вызов smemset
    smemset(buffer, 'X', size);

    printf("Buffer after: %.10s...\n", buffer);
}

int main()
{
    printf("🔍 ASM Showcase for smemset\n\n");

    test_smemset_clear();
    test_smemset_with_calc();

    return 0;
}