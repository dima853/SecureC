// test_dead_store.c
#include "../include/smemset.h"
#include <stdio.h>
#include <string.h>

// Функция, которая "не использует" очищенные данные
void __attribute__((noinline)) process_sensitive_data(char *data, int len)
{
    // Компилятор может убрать memset если увидит что data не используется после
    // memset(data, 0, len);  // ← этот вызов может быть удален!
    memset(data, 0, len); // ← этот вызов ДОЛЖЕН остаться

    // Не используем data дальше - идеальные условия для оптимизации
}

int main()
{
    char password[64] = "super_secret_password_12345";

    printf("Before: %s\n", password);
    process_sensitive_data(password, sizeof(password));

    // Проверяем что smemset действительно выполнился
    int is_cleared = 1;
    for (int i = 0; i < sizeof(password); i++)
    {
        if (password[i] != 0)
        {
            is_cleared = 0;
            break;
        }
    }

    if (is_cleared)
    {
        printf("PASS: smemset survived dead store elimination\n");
    }
    else
    {
        printf("FAIL: Data was not cleared - optimization detected!\n");
    }

    return 0;
}