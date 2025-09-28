#include <string.h>

void extreme_optimization_test() {
    char data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    memset(data, 0, sizeof(data));  // СТАНДАРТНЫЙ memset
}

int main() {
    extreme_optimization_test();
    return 0;
}
