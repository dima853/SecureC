#include <stdio.h>
#include <string.h>

void test_volatile_memset() {
    volatile char secret[64] = "VolatileSecretData";
    printf("Volatile before: %s\n", (char*)secret);
    memset((void*)secret, 0, sizeof(secret));  // СТАНДАРТНЫЙ memset
    printf("Volatile after: [CLEARED]\n");
}

int main() {
    test_volatile_memset();
    return 0;
}
