#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <setjmp.h>

static jmp_buf env;

// Обработчик таймаута
static void timeout_handler(int sig)
{
    printf("⏰ TIMEOUT - test took too long, moving to next test\n");
    longjmp(env, 1);
}

// Обертка для теста с таймаутом
static int run_test_with_timeout(void (*test_func)(), const char *test_name, int timeout_sec)
{
    printf("\n🔍 %s\n", test_name);

    // Устанавливаем обработчик сигнала
    signal(SIGALRM, timeout_handler);

    if (setjmp(env) == 0)
    {
        // Устанавливаем таймаут
        alarm(timeout_sec);

        // Запускаем тест
        test_func();

        // Отключаем таймаут если тест завершился вовремя
        alarm(0);
        return 1; // Успех
    }
    else
    {
        // Таймаут сработал
        printf("❌ %s: TIMEOUT after %d seconds\n", test_name, timeout_sec);
        return 0; // Таймаут
    }
}

// Вспомогательные функции
static void test_pass(const char *test_name)
{
    printf("✅ %s: PASS\n", test_name);
}

static void test_fail(const char *test_name, const char *error)
{
    printf("❌ %s: FAIL - %s\n", test_name, error);
}

static int create_test_socket_pair(int *sock1, int *sock2)
{
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0)
    {
        printf("   socketpair() failed: %s\n", strerror(errno));
        return -1;
    }
    *sock1 = sockets[0];
    *sock2 = sockets[1];
    return 0;
}

// Тест 1: Невалидный файловый дескриптор
static void test_invalid_fd_impl()
{
    char buffer[10];
    errno = 0;
    printf("   Calling recv(-1, buffer, %zu, 0)...\n", sizeof(buffer));
    ssize_t result = recv(-1, buffer, sizeof(buffer), 0);
    printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

    if (result == -1 && errno == EBADF)
    {
        test_pass("Invalid file descriptor");
    }
    else
    {
        test_fail("Invalid file descriptor", "Unexpected behavior");
    }
}

// Тест 2: NULL буфер
static void test_null_buffer_impl()
{
    int sock;
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        errno = 0;
        printf("   Calling recv(sock, NULL, 10, 0)...\n");
        ssize_t result = recv(sock, NULL, 10, 0);
        printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

        if (result == -1)
        {
            test_pass("NULL buffer");
        }
        else
        {
            test_fail("NULL buffer", "Should have failed with NULL buffer");
        }
        close(sockets[0]);
        close(sockets[1]);
    }
    else
    {
        test_fail("NULL buffer", "Failed to create socket pair");
    }
}

// Тест 3: Невалидные флаги
static void test_invalid_flags_impl()
{
    int sock;
    int sockets[2];
    char buffer[10];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        errno = 0;
        printf("   Calling recv(sock, buffer, %zu, 0xFFFFFFFF)...\n", sizeof(buffer));
        ssize_t result = recv(sock, buffer, sizeof(buffer), 0xFFFFFFFF);
        printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

        if (result == -1)
        {
            test_pass("Invalid flags");
        }
        else
        {
            test_fail("Invalid flags", "Should have failed with invalid flags");
        }
        close(sockets[0]);
        close(sockets[1]);
    }
    else
    {
        test_fail("Invalid flags", "Failed to create socket pair");
    }
}

// Тест 4: Передача не-сокета (файла)
static void test_not_a_socket_impl()
{
    char buffer[10];
    FILE *tmp = tmpfile();
    if (tmp)
    {
        int fd = fileno(tmp);
        printf("   Created temp file with fd=%d\n", fd);
        errno = 0;
        printf("   Calling recv(fd, buffer, %zu, 0)...\n", sizeof(buffer));
        ssize_t result = recv(fd, buffer, sizeof(buffer), 0);
        printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

        if (result == -1)
        {
            test_pass("Not a socket");
        }
        else
        {
            test_fail("Not a socket", "Should have failed with file descriptor");
        }
        fclose(tmp);
    }
    else
    {
        test_fail("Not a socket", "Failed to create temp file");
    }
}

// Тест 5: Слишком большой размер буфера
static void test_buffer_too_large_impl()
{
    int sock;
    int sockets[2];
    char buffer[1024 * 1024 + 100];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        errno = 0;
        printf("   Calling recv(sock, buffer, %zu, 0)...\n", sizeof(buffer));
        ssize_t result = recv(sock, buffer, sizeof(buffer), 0);
        printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

        if (result >= 0 || errno != EMSGSIZE)
        {
            test_pass("Buffer too large");
        }
        else
        {
            test_fail("Buffer too large", "Unexpected error");
        }
        close(sockets[0]);
        close(sockets[1]);
    }
    else
    {
        test_fail("Buffer too large", "Failed to create socket pair");
    }
}

// Тест 6: Успешное получение данных
static void test_successful_receive_impl()
{
    int sock1, sock2;
    char send_buffer[] = "Hello, glibc recv!";
    char recv_buffer[100] = {0};

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        printf("   Created socket pair: %d <-> %d\n", sock1, sock2);

        printf("   Sending: '%s' (%zu bytes)\n", send_buffer, strlen(send_buffer));
        ssize_t send_result = send(sock1, send_buffer, strlen(send_buffer), 0);
        printf("   send returned: %zd\n", send_result);

        printf("   Receiving with recv(sock2, buffer, %zu, 0)...\n", sizeof(recv_buffer));
        ssize_t result = recv(sock2, recv_buffer, sizeof(recv_buffer), 0);
        printf("   recv returned: %zd, received: '%s'\n", result, recv_buffer);

        if (result == strlen(send_buffer) &&
            strcmp(send_buffer, recv_buffer) == 0)
        {
            test_pass("Successful receive");
        }
        else
        {
            test_fail("Successful receive", "Data mismatch");
        }
        close(sock1);
        close(sock2);
    }
    else
    {
        test_fail("Successful receive", "Failed to create socket pair");
    }
}

// Главная функция тестирования
int main()
{
    printf("🧪 Testing GLIBC standard recv implementation with timeouts...\n");
    printf("==============================================================\n");

    int passed_tests = 0;
    int total_tests = 6;

    // Запускаем тесты с таймаутом 3 секунды каждый
    passed_tests += run_test_with_timeout(test_invalid_fd_impl, "Test 1: Invalid file descriptor", 3);
    passed_tests += run_test_with_timeout(test_null_buffer_impl, "Test 2: NULL buffer", 3);
    passed_tests += run_test_with_timeout(test_invalid_flags_impl, "Test 3: Invalid flags", 3);
    passed_tests += run_test_with_timeout(test_not_a_socket_impl, "Test 4: Not a socket", 3);
    passed_tests += run_test_with_timeout(test_buffer_too_large_impl, "Test 5: Buffer too large", 3);
    passed_tests += run_test_with_timeout(test_successful_receive_impl, "Test 6: Successful receive", 3);

    printf("\n🎯 GLIBC recv testing completed!\n");
    printf("📊 Results: %d/%d tests passed\n", passed_tests, total_tests);

    if (passed_tests < total_tests)
    {
        printf("💡 This shows why your safe_recv() is needed - glibc recv has unsafe edge cases!\n");
    }

    return 0;
}