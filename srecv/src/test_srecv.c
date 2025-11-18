#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

// Объявляем твою функцию
ssize_t srecv(int fd, void *buf, size_t len, int flags);

// Вспомогательные функции для тестов
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
        return -1;
    }
    *sock1 = sockets[0];
    *sock2 = sockets[1];
    return 0;
}

// Тест 1: Невалидный файловый дескриптор
static void test_invalid_fd()
{
    char buffer[10];
    ssize_t result = srecv(-1, buffer, sizeof(buffer), 0);

    if (result == -1 && errno == EBADF)
    {
        test_pass("Invalid file descriptor");
    }
    else
    {
        test_fail("Invalid file descriptor", "Wrong error code");
    }
}

// Тест 2: NULL буфер
static void test_null_buffer()
{
    int sock;
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        ssize_t result = srecv(sock, NULL, 10, 0);

        if (result == -1 && errno == EINVAL)
        {
            test_pass("NULL buffer");
        }
        else
        {
            test_fail("NULL buffer", "Wrong error code");
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
    int sock;
    int sockets[2];
    char buffer[10];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        // Используем невалидные флаги
        ssize_t result = srecv(sock, buffer, sizeof(buffer), 0xFFFFFFFF);

        if (result == -1 && errno == EINVAL)
        {
            test_pass("Invalid flags");
        }
        else
        {
            test_fail("Invalid flags", "Wrong error code");
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
    char buffer[10];
    // Создаем файловый дескриптор файла, а не сокета
    FILE *tmp = tmpfile();
    if (tmp)
    {
        int fd = fileno(tmp);
        ssize_t result = srecv(fd, buffer, sizeof(buffer), 0);

        if (result == -1 && errno == ENOTSOCK)
        {
            test_pass("Not a socket");
        }
        else
        {
            test_fail("Not a socket", "Wrong error code");
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
    int sock;
    int sockets[2];
    char buffer[1024 * 1024 + 100]; // Больше чем MAX_SAFE_SIZE

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0)
    {
        sock = sockets[0];
        ssize_t result = srecv(sock, buffer, sizeof(buffer), 0);

        if (result == -1 && errno == EMSGSIZE)
        {
            test_pass("Buffer too large");
        }
        else
        {
            test_fail("Buffer too large", "Wrong error code");
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
    int sock1, sock2;
    char send_buffer[] = "Hello, safe recv!";
    char recv_buffer[100] = {0};

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        // Отправляем данные в один сокет
        send(sock1, send_buffer, strlen(send_buffer), 0);

        // Получаем из другого сокета
        ssize_t result = srecv(sock2, recv_buffer, sizeof(recv_buffer), 0);

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
    int sock1, sock2;
    char send_buffer[] = "Peek test";
    char recv_buffer1[100] = {0};
    char recv_buffer2[100] = {0};

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        send(sock1, send_buffer, strlen(send_buffer), 0);

        // Первый recv с PEEK - данные должны остаться в сокете
        ssize_t result1 = srecv(sock2, recv_buffer1, sizeof(recv_buffer1), MSG_PEEK);

        // Второй recv без PEEK - должны получить те же данные
        ssize_t result2 = srecv(sock2, recv_buffer2, sizeof(recv_buffer2), 0);

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
    int sock1, sock2;
    char buffer[10];

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        close(sock1); // Закрываем один конец

        // Должны получить 0 (конец файла)
        ssize_t result = srecv(sock2, buffer, sizeof(buffer), 0);

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
    int sock1, sock2;
    char buffer[10];

    if (create_test_socket_pair(&sock1, &sock2) == 0)
    {
        // len = 0 должен быть разрешен
        ssize_t result = srecv(sock2, buffer, 0, 0);

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
    printf("🧪 Testing safe recv implementation...\n\n");

    test_invalid_fd();
    test_null_buffer();
    test_invalid_flags();
    test_not_a_socket();
    test_buffer_too_large();
    test_successful_receive();
    test_peek_receive();
    test_closed_connection();
    test_zero_length();

    printf("\n🎯 Testing completed!\n");
    return 0;
}