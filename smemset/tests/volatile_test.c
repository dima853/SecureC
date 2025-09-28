#include <stdio.h>
#include "../include/smemset.h"

void test_volatile_smemset()
{
    volatile char secret[64] = "VolatileSecretData";
    printf("Volatile before: %s\n", (char *)secret);

    smemset((void *)secret, 0, sizeof(secret));

    printf("Volatile after: [CLEARED]\n");
}

int main()
{
    test_volatile_smemset();
    return 0;
}