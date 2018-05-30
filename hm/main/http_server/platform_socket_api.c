/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#include "platform_socket_api.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void hk_console_printf(const char *text, ...);

int32_t platform_socket_socket(int32_t family, int32_t type, int32_t protocol)
{
    return socket(family, type, protocol);
}

int32_t platform_socket_bind(int32_t sock, struct sockaddr *addr, size_t addrlen)
{
    return bind(sock, addr, addrlen);
}

int32_t platform_socket_listen(int32_t sock, int32_t backlog)
{
    return listen(sock, backlog);
}

int32_t platform_socket_accept(int32_t sock, struct sockaddr *cliaddr, size_t *addrlen)
{
    return accept(sock, cliaddr, (socklen_t *)addrlen);
}

int32_t platform_socket_select(fd_set *read_fd_set, fd_set *write_fd_set, fd_set *ex, uint32_t timeout_ms)
{
    struct timeval tv = {0, 0};
    if (timeout_ms > 0)
    {
        tv.tv_sec = (long)(timeout_ms / 1000);         // ms --> s
        tv.tv_usec = (long)(timeout_ms % 1000) * 1000; // ms --> us
    }
    int32_t retval = select(FD_SETSIZE, read_fd_set, write_fd_set, ex, &tv);
    return retval;
}

int32_t platform_socket_send(int32_t sock, char *buf, int32_t len, int32_t flags)
{
    (void)flags;
    return write(sock, buf, len);
}

int32_t platform_socket_recv(int32_t sock, char *buf, int32_t len, int32_t flags)
{
    (void)flags;
    return read(sock, buf, len);
}

int32_t platform_socket_close(int32_t sock)
{
    return close(sock);
}

int32_t platform_socket_set_option(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    return setsockopt(sockfd, level, optname, optval, optlen);
}

int32_t platform_socket_setsockopt(int sockfd, int level, int optname, void *optval, socklen_t optlen)
{
    return setsockopt(sockfd, level, optname, optval, optlen);
}

void platform_socket_log(const char *text, ...)
{
    char buffer[128];
    va_list arg_list;
    if (text != NULL)
    {
        /* Print the string to the buffer.*/
        va_start(arg_list, text);
        vsnprintf(buffer, sizeof(buffer), text, arg_list);
        va_end(arg_list);
        {
            hk_console_printf("HTTP LOG: %s", buffer);
        }
    }
}
