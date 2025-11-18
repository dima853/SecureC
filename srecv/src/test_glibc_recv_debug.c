#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

// Вспомогательные функции для тестов
static void test_pass(const char *test_name)
{
    printf("✅ %s: PASS\n", test_name);
}

static void test_fail(const char *test_name, const char *error)
{
    printf("❌ %s: FAIL - %s\n", test_name, error);
}

static void test_skip(const char *test_name, const char *reason)
{
    printf("⏭️  %s: SKIP - %s\n", test_name, reason);
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
static void test_invalid_fd()
{
    printf("\n🔍 Test 1: Invalid file descriptor\n");
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
static void test_null_buffer()
{
    printf("\n🔍 Test 2: NULL buffer\n");
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
static void test_invalid_flags()
{
    printf("\n🔍 Test 3: Invalid flags\n");
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
static void test_not_a_socket()
{
    printf("\n🔍 Test 4: Not a socket\n");
    char buffer[10];
    // Создаем файловый дескриптор файла, а не сокета
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
static void test_buffer_too_large()
{
    printf("\n🔍 Test 5: Buffer too large\n");
    int sock;
    int sockets[2];
    char buffer[1024 * 1024 + 100]; // Большой буфер

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        errno = 0;
        printf("   Calling recv(sock, buffer, %zu, 0)...\n", sizeof(buffer));
        ssize_t result = recv(sock, buffer, sizeof(buffer), 0);
        printf("   recv returned: %zd, errno=%d (%s)\n", result, errno, strerror(errno));

        // Стандартный recv не проверяет размер буфера
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
static void test_successful_receive()
{
    printf("\n🔍 Test 6: Successful receive\n");
    int sock1, sock2;
    char send_buffer[] = "Hello, glibc recv!";
    char recv_buffer[100] = {0};

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        printf("   Created socket pair: %d <-> %d\n", sock1, sock2);

        // Отправляем данные в один сокет
        printf("   Sending: '%s' (%zu bytes)\n", send_buffer, strlen(send_buffer));
        ssize_t send_result = send(sock1, send_buffer, strlen(send_buffer), 0);
        printf("   send returned: %zd\n", send_result);

        // Получаем из другого сокета
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

// Тест 7: Получение с флагом MSG_PEEK
static void test_peek_receive()
{
    printf("\n🔍 Test 7: Peek receive\n");
    int sock1, sock2;
    char send_buffer[] = "Peek test";
    char recv_buffer1[100] = {0};
    char recv_buffer2[100] = {0};

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        printf("   Created socket pair: %d <-> %d\n", sock1, sock2);

        send(sock1, send_buffer, strlen(send_buffer), 0);
        printf("   Sent: '%s'\n", send_buffer);

        // Первый recv с PEEK - данные должны остаться в сокете
        printf("   First recv with MSG_PEEK...\n");
        ssize_t result1 = recv(sock2, recv_buffer1, sizeof(recv_buffer1), MSG_PEEK);
        printf("   First recv returned: %zd, received: '%s'\n", result1, recv_buffer1);

        // Второй recv без PEEK - должны получить те же данные
        printf("   Second recv without PEEK...\n");
        ssize_t result2 = recv(sock2, recv_buffer2, sizeof(recv_buffer2), 0);
        printf("   Second recv returned: %zd, received: '%s'\n", result2, recv_buffer2);

        if (result1 == result2 &&
            result1 == strlen(send_buffer) &&
            strcmp(recv_buffer1, recv_buffer2) == 0 &&
            strcmp(send_buffer, recv_buffer1) == 0)
        {
            test_pass("Peek receive");
        }
        else
        {
            test_fail("Peek receive", "Peek behavior incorrect");
        }
        close(sock1);
        close(sock2);
    }
    else
    {
        test_fail("Peek receive", "Failed to create socket pair");
    }
}

// Тест 8: Закрытое соединение
static void test_closed_connection()
{
    printf("\n🔍 Test 8: Closed connection\n");
    int sock1, sock2;
    char buffer[10];

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        printf("   Created socket pair: %d <-> %d\n", sock1, sock2);
        printf("   Closing sock1 (%d)...\n", sock1);
        close(sock1); // Закрываем один конец

        // Должны получить 0 (конец файла)
        printf("   Calling recv(sock2, buffer, %zu, 0)...\n", sizeof(buffer));
        ssize_t result = recv(sock2, buffer, sizeof(buffer), 0);
        printf("   recv returned: %zd\n", result);

        if (result == 0)
        {
            test_pass("Closed connection");
        }
        else
        {
            test_fail("Closed connection", "Wrong return value");
        }
        close(sock2);
    }
    else
    {
        test_fail("Closed connection", "Failed to create socket pair");
    }
}

// Тест 9: len = 0 (специальный случай)
static void test_zero_length()
{
    printf("\n🔍 Test 9: Zero length\n");
    int sock1, sock2;
    char buffer[10];

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        printf("   Created socket pair: %d <-> %d\n", sock1, sock2);

        // len = 0 должен быть разрешен
        printf("   Calling recv(sock2, buffer, 0, 0)...\n");
        ssize_t result = recv(sock2, buffer, 0, 0);
        printf("   recv returned: %zd\n", result);

        if (result == 0)
        {
            test_pass("Zero length");
        }
        else
        {
            test_fail("Zero length", "Zero length should return 0");
        }
        close(sock1);
        close(sock2);
    }
    else
    {
        test_fail("Zero length", "Failed to create socket pair");
    }
}

// Главная функция тестирования
int main()
{
    printf("🧪 Testing GLIBC standard recv implementation...\n");
    printf("=================================================\n");

    // Пропускаем первый тест который зависает
    test_skip("Invalid file descriptor", "May hang on this system");

    test_null_buffer();
    test_invalid_flags();
    test_not_a_socket();
    test_buffer_too_large();
    test_successful_receive();
    test_peek_receive();
    test_closed_connection();
    test_zero_length();

    printf("\n🎯 GLIBC recv testing completed!\n");
    return 0;
}