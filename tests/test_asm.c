#include <stdio.h>
#include "../include/smemset.h"

void test_standard_memset()
{
    char data1[64] = "STANDARD MEMSET DATA";
    printf("Before standard memset: %s\n", data1);
    asm volatile("# === STANDARD MEMSET START ===");
    memset(data1, 0, sizeof(data1));
    asm volatile("# === STANDARD MEMSET END ===");
}

void test_smemset()
{
    char data2[64] = "SMEMSET DATA";
    printf("Before smemset: %s\n", data2);
    asm volatile("# === SMEMSET START ===");
    smemset(data2, 0, sizeof(data2));
    asm volatile("# === SMEMSET END ===");
}

int main()
{
    printf("🔍 Testing if compiler optimizes away memset calls\n\n");
    test_standard_memset();
    test_smemset();
    printf("\n✅ Check assembly output\n");
    return 0;
}