CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./include
TARGETS = test_smemset example_basic example_secure test_asm

all: $(TARGETS)

smemset.o: src/smemset.c include/smemset.h
	$(CC) $(CFLAGS) -c src/smemset.c -o smemset.o

test_smemset: tests/test_smemset.c smemset.o
	$(CC) $(CFLAGS) -o $@ tests/test_smemset.c smemset.o

example_basic: examples/basic.c smemset.o
	$(CC) $(CFLAGS) -o $@ examples/basic.c smemset.o

example_secure: examples/secure.c smemset.o
	$(CC) $(CFLAGS) -o $@ examples/secure.c smemset.o

test_asm: tests/test_asm.c smemset.o
	$(CC) $(CFLAGS) -o $@ tests/test_asm.c smemset.o

test: test_smemset
	./test_smemset

clean:
	rm -f $(TARGETS) *.o

.PHONY: all test clean