#include "../include/smemset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void test_basic_functionality()
{
    printf("🧪 Testing Basic Functionality...\n");
    char buffer[100];
    strcpy(buffer, "Hello World");
    printf("   Before: %s\n", buffer);
    smemset(buffer, 'X', 50);
    printf("   After:  %.50s\n", buffer);

    for (int i = 0; i < 50; i++)
    {
        assert(buffer[i] == 'X');
    }
    printf("   ✅ PASS\n");
}

void test_secure_zero_clear()
{
    printf("🧪 Testing Secure Zero Clear...\n");
    char sensitive[64];
    const char *secret = "MY_SECRET_PASSWORD_1234567890";
    strcpy(sensitive, secret);
    printf("   Before: %s\n", sensitive);
    smemset(sensitive, 0, sizeof(sensitive));

    for (size_t i = 0; i < sizeof(sensitive); i++)
    {
        if (sensitive[i] != 0)
        {
            printf("   ❌ SECURITY BREACH at byte %zu: 0x%02X\n", i, (unsigned char)sensitive[i]);
            assert(0);
        }
    }
    printf("   ✅ SECURE CLEAR PASS\n");
}

void test_performance_large_buffer()
{
    printf("🧪 Testing Performance with Large Buffer...\n");
    const size_t size = 1000000;
    char *large_buffer = malloc(size);
    if (!large_buffer)
    {
        printf("   ⚠️  Skip (memory allocation failed)\n");
        return;
    }

    clock_t start = clock();
    for (int i = 0; i < 10; i++)
    {
        smemset(large_buffer, i % 256, size);
    }
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("   Time for 10x 1MB smemset: %.3f seconds\n", elapsed);
    printf("   ✅ PERFORMANCE TEST PASS\n");
    free(large_buffer);
}

void test_edge_cases()
{
    printf("🧪 Testing Edge Cases...\n");
    void *result = smemset(NULL, 0, 100);
    assert(result == NULL);
    printf("   ✅ NULL pointer test passed\n");

    char buffer[10] = "test";
    result = smemset(buffer, 'X', 0);
    assert(result == buffer);
    assert(buffer[0] == 't');
    printf("   ✅ Zero length test passed\n");

    buffer[0] = 'A';
    smemset(buffer, 'B', 1);
    assert(buffer[0] == 'B');
    printf("   ✅ Single byte test passed\n");

    char block[8];
    smemset(block, 0xAA, sizeof(block));
    for (int i = 0; i < 8; i++)
    {
        assert(block[i] == (char)0xAA);
    }
    printf("   ✅ Block size test passed\n");
}

int main()
{
    printf("🚀 Starting smemset Test Suite\n\n");
    test_basic_functionality();
    test_secure_zero_clear();
    test_performance_large_buffer();
    test_edge_cases();
    printf("\n🎉 ALL TESTS PASSED! smemset is working correctly.\n");
    return 0;
}