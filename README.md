# 🚀 Secure Memset Library (smemset)

⚠️### **The project is under implementation, this is a test version!**

**Guaranteed memory overwrite that the compiler won't dare optimize away!**

## 📖 About the Project

`smemset` is a secure replacement for the standard `memset` function, designed for cases where you need to be **absolutely sure** that sensitive data will be overwritten in memory. Unlike the standard implementation, which compilers often optimize away (especially for dead saves), `smemset` uses advanced techniques to guarantee the operation.

## ⚡ Features

- **🔒 Optimization protection** — volatile pointers and memory barriers
- **🏎️ High performance** — only 2-3x slower than memset
- **🎯 Cross-platform** — support for x86_64, AArch64, and other architectures
- **📊 Smart alignment** — efficient work with unaligned memory
- **🧪 Comprehensive testing** — performance, security, and edge case tests

## 🏗️ Project structure

```
smemset/
├── 📁 github/workflows/ # CI/CD configurations
├── 📁 examples/ # Usage examples
│ ├── example_basic.c # Basic example
│ └── example_secure.c # Secure usage example
├── 📁 include/
│ └── smemset.h # Header file
├── 📁 src/
│ └── smemset.c # Implementation source code
├── 📁 tests/ # Various tests
│ ├── test_smemset.c # Basic unit tests
│ ├── extreme_test.c # Big data stress tests
│ ├── volatile_test.c # Optimization protection tests
│ ├── test_speed.c # Performance tests
│ └── asm_showcase.c # Assembly code demonstration
└── 📁 asm/ # Generated assembly code
```

## 🚀 Quick Start

### Compilation

```bash
# Standard compilation
make

# With CMake
mkdir build && cd build
cmake .. && make
```

### Basic usage

```c
#include "include/smemset.h"
#include <stdio.h>

int main() {
char sensitive_data[256] = "my_secret_password";

// Safe erasure - the compiler does not optimize!
smemset(sensitive_data, 0, sizeof(sensitive_data));

printf("Data securely wiped!\n");
return 0;
}
```

### Advanced Example

```c
#include "include/smemset.h"
#include <stdlib.h>

void handle_sensitive_operation() {
char *crypto_key = malloc(32);

// Using the key for some operations...

// Guaranteed erasure of the key in memory
smemset(crypto_key, 0xAA, 32); // Fill with a pattern for debugging
smemset(crypto_key, 0x55, 32); // Double erasure
smemset(crypto_key, 0, 32); // Final zeroing

free(crypto_key);
}
```

## 🧪 Testing

The project includes a comprehensive testing system:

```bash
# Run all tests
make test

# Specific test scenarios
./tests/test_smemset # Basic functionality
./tests/extreme_test # Stress tests on big data
./tests/volatile_test # Check for optimization protection
./tests/test_speed # Performance tests
./tests/asm_showcase # Assembly code demonstration
```

## 📊 Performance tests

### Test results

```
📊 Test results (100 iterations of 1,000,000 Bytes):
──────────────────────────────────────────────
│ Function │ Time (sec) │
───────────────────────────────────────
│ memset │ 0.002 │
│ smemset │ 0.005 │
──────────────────────────────────────────
⚡ Slowdown: 120.4%
✅ Data check: memset=OK, smemset=OK
```

### Key performance metrics:

- **Only 2.2x slowdown** relative to optimized memset
- **Almost unnoticeable** in real-world scenarios (3ms difference per 100MB)
- **Excellent tradeoff** between speed and security guarantees

### Running the test Performance:

```bash
gcc -O2 -I./include tests/test_speed.c src/smemset.c -o tests/test_speed
./tests/test_speed
```

## 🛠️ Technical Details

### Supported Architectures

- **x86_64**: Uses `mfence` for memory barriers
- **AArch64**: Uses `dmb sy` for system barriers
- **Other**: Compiler-based memory barriers

### Operation Algorithm

1. **Boundary condition checking** (NULL pointers, zero length)
2. **Memory barrier on input** for ordering operations
3. **Byte-based processing** for small buffers (<8 bytes)
4. **Alignment and vector processing** for large buffers
5. **8x Loop unrolling** for maximum performance
6. **Memory barrier on exit** to ensure operation completion

## 🎯 When to use

```c
// ✅ USE smemset for:
smemset(password, 0, length); // Sensitive data
smemset(crypto_key, 0, key_size); // Cryptographic keys
smemset(auth_token, 0, token_len); // Authentication tokens

// ✅ USE memset for:
memset(work_buffer, 0, size); // Regular buffers
memset(temp_array, 0, count); // Temporary data
memset(struct_ptr, 0, sizeof(struct)); // Initializing structures
```

## 🔧 Integration

### Into Existing Projects

```cmake
# CMakeLists.txt
add_subdirectory(smemset)
target_link_libraries(your_target smemset)
```

### Direct Inclusion

```c
// Just copy smemset.h and smemset.c into your project
#include "smemset.h"
```

## 🐛 Debugging and Analysis

For performance and generated code analysis:

```bash
# Comparing assembly code
make asm_showcase

# Tests with different optimization levels
make test O_LEVEL=-O0 # No optimization
make test O_LEVEL=-O2 # Standard optimization
make test O_LEVEL=-O3 # Aggressive Optimization
```

## 📄 License

The project is distributed under the MIT license. Details are in the LICENSE file.

## 🤝 Contributing to the Project

Pull requests and bug reports are welcome! Before making changes:

1. Check the code with `make test`
2. Ensure that all tests pass, including performance
3. Update the documentation if necessary

---

## ⚡ Performance Summary

**smemset demonstrates an excellent balance:** only 2.2x slower than a highly optimized memset, while providing a 100% guarantee of erasing sensitive data.
