#include "../include/smemset.h"

void extreme_optimization_test()
{
    char data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    smemset(data, 0, sizeof(data));
}

int main()
{
    extreme_optimization_test();
    return 0;
}