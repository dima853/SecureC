#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

ssize_t srecv(int fd, void *buf, size_t len, int flags)
{
    if (fd < 0)
    {
        errno = EBADF;
        return -1;
    }

    if (buf == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (flags & ~(MSG_OOB | MSG_PEEK | MSG_WAITALL | MSG_DONTWAIT | MSG_NOSIGNAL))
    {
        errno = EINVAL;
        return -1;
    }

    int socket_type = 0;
    socklen_t type_len = sizeof(socket_type);

    /* int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
    s: The socket descriptor of the socket to query.
    level: The protocol level at which the option is defined (e.g., SOL_SOCKET).
    optname: The name of the socket option you want to retrieve (e.g., SO_ACCEPTCONN).
    optval: A pointer to a buffer where the option's value will be stored.
    optlen: A pointer to a socklen_t variable. It should be initialized with the size of the optval buffer and will be updated with the actual length of the returned option value.

    SOL_SOCKET
    is a constant used in the socket API to specify the socket layer itself, rather than a specific protocol layer like TCP or IP

    SO_TYPE
    is a socket option used with the getsockopt() function in the C programming language to retrieve the socket's current type (e.g., stream, datagram) [1]. It is not used with setsockopt() as the socket type is set when the socket is created and cannot be changed later
    */
    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &socket_type, &type_len) < 0)
    {
        errno = ENOTSOCK; // not a socket
        return -1;
    }

    const size_t MAX_SAFE_SIZE = 1024 * 1024;
    if (len > MAX_SAFE_SIZE)
    {
        errno = EMSGSIZE; // message too long
        return -1;
    }

    ssize_t result = -1;
    int retry_count = 3;

    do
    {
        result = recv(fd, buf, len, flags);
        if (result >= 0)
        {
            break;
        }
    } while (errno == EINTR && --retry_count > 0);

    return result;
}