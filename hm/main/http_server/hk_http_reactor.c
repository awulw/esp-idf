/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hk_http_reactor.h"
#include "platform_socket_api.h"
#include "hk_http.h"

#define HK_REACTOR_LISTEN_BACKCLOG 2

extern void platform_socket_log(const char *text, ...);

typedef enum {
    HTTP_STOPPED = 0, // Reactor has not been started yet
    HTTP_RUNNING,     // Reactor is running
    HTTP_CLOSING      // Reactor is closing

} hk_http_reactor_state_t;

typedef struct
{
    hk_http_reactor_state_t state;
    hk_http_socket_t sockets[HK_IP_SOCKETS_SIZE];
    hk_http_socket_fd_t acceptor_fd_ipv4;
    hk_http_socket_fd_t acceptor_fd_ipv6;
    hk_dispatcher_acc_map_t *acc_map;
} hk_http_reactor_t;

void hk_http_socket_send(hk_http_socket_t *socket, const byte_t *data, size_t length)
{
    if (socket->fd <= 0)
    {
        return;
    }
    // TODO(DJ): Add outgoing fragmentation or fix the exsting one

    const int ret = platform_socket_send(socket->fd, (char *)data, length, 0);
    if (ret <= 0)
    {
        platform_socket_log("send error: %d\n", ret);
        return;
    }
}

HK_NOT_NULL_ARGS(1)
static void hk_http_reactor_accept(hk_http_reactor_t *reactor, hk_http_socket_fd_t listening_socket)
{
    union {
        struct sockaddr sock_addr;
        struct sockaddr_in sock_addr_ip4;
        struct sockaddr_in6 sock_addr_ip6;
    } sockaddr_union;
    size_t sockaddr_size = sizeof(sockaddr_union);
    memset(&sockaddr_union, 0, sockaddr_size);
    hk_http_socket_fd_t fd = platform_socket_accept(listening_socket, &sockaddr_union.sock_addr, &sockaddr_size);
    if (fd <= 0)
    {
        platform_socket_log("Accept: called with invalid fd\n");
        return;
    }

    hk_http_socket_t *socket = NULL;
    for (int i = 0; i < HK_IP_SOCKETS_SIZE - 1; ++i)
    {
        if (reactor->sockets[i].fd == 0)
        {
            socket = &reactor->sockets[i];
            break;
        }
    }

    if (socket == NULL)
    {
        hk_http_socket_close(&reactor->sockets[HK_IP_SOCKETS_SIZE - 1]);
        socket = &reactor->sockets[HK_IP_SOCKETS_SIZE - 1];
        platform_socket_log("Accept: no free socket slots\n");
    }

    memset(socket, 0, sizeof(*socket));
    socket->fd = fd;
    // hk_pairing_state_machine_construct(&socket->session_state_machine);
    platform_socket_log("Accepted client on socket: %d\n", fd);
    platform_socket_log("from IP address: %s\n", inet_ntoa(sockaddr_union.sock_addr_ip4.sin_addr));
}

void hk_http_socket_close(hk_http_socket_t *socket)
{
    hk_byte_stream_destroy(&socket->recv_buffer);
    if (socket->fd > 0)
    {
        platform_socket_log("Conection %d colosed\n", socket->fd);
        platform_socket_close(socket->fd);
    }
    // hk_pairing_state_machine_destroy(&socket->session_state_machine);
    memset(socket, 0, sizeof(*socket));
}

HK_NOT_NULL_ARGS(1)
static void hk_http_socket_receive_fragment(hk_http_socket_t *socket)
{
    char recv_buffer[HAP_HTTP_RECV_BUFFER_SIZE];
    while (true)
    {
        const int32_t recv_length = platform_socket_recv(socket->fd, recv_buffer, sizeof(recv_buffer), 0);

        if (recv_length > 0)
        {
            // TODO(DJ): Check if socket has session security enabled

            if (socket->recv_buffer.data_size + recv_length > HAP_HTTP_RECV_DYNAMIC_BUFER_MAX_LENGTH)
            {
                hk_http_respond_without_content(socket, HTTP_STATUS_SERVICE_UNAVAILABLE);
                platform_socket_log("Fragmentation error, message  greater then %d\n",
                                    HAP_HTTP_RECV_DYNAMIC_BUFER_MAX_LENGTH);
                hk_http_socket_close(socket);
                return;
            }

            if (hk_byte_stream_append(&socket->recv_buffer, (byte_t *)recv_buffer, recv_length) != true)
            {
                hk_http_respond_without_content(socket, HTTP_STATUS_SERVICE_UNAVAILABLE);
                platform_socket_log("error: No enough resource to handle http payload\n");
                hk_http_socket_close(socket);
                return;
            }

            platform_socket_log("Received data %d on socket fd %d\n", recv_length, socket->fd);
            platform_socket_log("Current buffer size %d\n", socket->recv_buffer.data_size);

            hk_parse_request(socket);
            return;
        }

        if (recv_length == 0)
        {
            hk_http_socket_close(socket);
            return;
        }

        if (recv_length == -1)
        {
            platform_socket_log("platform_socket_recv returned -1 on sock %d\n", socket->fd);
            hk_http_socket_close(socket);
            return;
        }
    }
}

// HK_NOT_NULL_ARGS(1, 2)
static void hk_http_reactor_check_sockets(hk_http_reactor_t *reactor, fd_set *read_set)
{
    for (int i = 0; i < HK_IP_SOCKETS_SIZE; ++i)
    {
        hk_http_socket_t *socket = &reactor->sockets[i];
        if (socket->fd > 0)
        {
            if (!socket->close_after_flush)
            {
                FD_SET(socket->fd, read_set);
            }
        }
    }

    if (reactor->acceptor_fd_ipv4 > 0)
    {
        FD_SET(reactor->acceptor_fd_ipv4, read_set);
    }

    if (reactor->acceptor_fd_ipv6 > 0)
    {
        FD_SET(reactor->acceptor_fd_ipv6, read_set);
    }
}

HK_NOT_NULL_ARGS(1)
static void hk_http_reactor_select(hk_http_reactor_t *reactor)
{
    fd_set read_set;
    FD_ZERO(&read_set);
    hk_http_reactor_check_sockets(reactor, &read_set);
    const int ret = platform_socket_select(&read_set, NULL, NULL, HK_IP_TIMEOUT);
    if (ret == 0)
    {
        // Timed out
        return;
    }

    if (FD_ISSET(reactor->acceptor_fd_ipv4, &read_set))
    {
        hk_http_reactor_accept(reactor, reactor->acceptor_fd_ipv4);
    }
    //    if (FD_ISSET(reactor->acceptor_fd_ipv6, &read_set))
    //    {
    //        hk_http_reactor_accept(reactor, reactor->acceptor_fd_ipv6);
    //    }

    for (int i = 0; i < HK_IP_SOCKETS_SIZE; ++i)
    {
        hk_http_socket_t *socket = &reactor->sockets[i];
        if (socket->fd <= 0)
        {
            continue;
        }
        if (FD_ISSET(socket->fd, &read_set))
        {
            socket->acc_map = reactor->acc_map;
            hk_http_socket_receive_fragment(socket);
        }
    }
}

void http_socket_option(hk_http_socket_fd_t tcp_socket, int32_t option, int32_t value)
{
    if (platform_socket_set_option(tcp_socket, SOL_SOCKET, option, &value, sizeof(value)) < 0)
    {
        platform_socket_log("HTTP:Unable to set socket option(0x%X)\n", value);
    }
}

hk_http_socket_fd_t hk_http_reactor_create_socket(int32_t socket_family)
{
    union {
        struct sockaddr sock_addr;
        struct sockaddr_in sock_addr_ip4;
        struct sockaddr_in6 sock_addr_ip6;
    } sockaddr_union;

    memset(&sockaddr_union, 0, sizeof(sockaddr_union));
    const hk_http_socket_fd_t fd = platform_socket_socket(socket_family, SOCK_STREAM, 0);
    if (fd < 0)
    {
        platform_socket_log("qapi_socket failed %d\n", fd);
        return fd;
    }

    http_socket_option(fd, SO_REUSEADDR, 1);
    if (socket_family == AF_INET)
    {
        sockaddr_union.sock_addr_ip4.sin_family = socket_family;
        sockaddr_union.sock_addr_ip4.sin_port = htons(HK_IP_PORT);
        sockaddr_union.sock_addr_ip4.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        // sockaddr_union.sock_addr_ip6.sin6_family = socket_family;
        // sockaddr_union.sock_addr_ip6.sin6_port = htons(HK_IP_PORT);
        // sockaddr_union.sock_addr_ip6.sin6_addr.s6_addr = htonl (INADDR_ANY);
        platform_socket_log("ip6 not implemented\n");
        return -1;
    }
    int32_t ret = platform_socket_bind(fd, &sockaddr_union.sock_addr, sizeof(sockaddr_union));
    if (ret != 0)
    {
        platform_socket_log("platform_socket_bind failed: %d\n", ret);
        platform_socket_close(fd);
        return ret;
    }

    ret = platform_socket_listen(fd, 1);
    if (ret != 0)
    {
        platform_socket_log("platform_socket_listen failed: %d\n", ret);
        platform_socket_close(fd);
        return ret;
    }

    return fd;
}

hk_ret_t hk_http_reactor_init(hk_http_reactor_t *reactor)
{
    memset(reactor, 0, sizeof(*reactor));
    reactor->acceptor_fd_ipv4 = hk_http_reactor_create_socket(AF_INET);
    reactor->acceptor_fd_ipv6 = hk_http_reactor_create_socket(AF_INET6);
    // reactor->acceptor_fd_ipv6 = -1; // TODO(DJ): Investigate IPv6
    if (reactor->acceptor_fd_ipv4 < 0 && reactor->acceptor_fd_ipv6 < 0)
    {
        platform_socket_log("Failed to create both ipv4 and ipv6 sockets\n");
        return HK_RET_FAIL;
    }

    reactor->state = HTTP_RUNNING;
    return HK_RET_SUCCESS;
}

void hk_http_reactor_deinit(hk_http_reactor_t *reactor)
{
    if (reactor->state == HTTP_RUNNING)
    {
        reactor->state = HTTP_CLOSING;
    }
    for (int i = 0; i < HK_IP_SOCKETS_SIZE; ++i)
    {
        hk_http_socket_close(&reactor->sockets[i]);
    }
    platform_socket_close(reactor->acceptor_fd_ipv4);
    platform_socket_close(reactor->acceptor_fd_ipv6);
}

void hk_reactor_task(void *arg)
{
    hk_http_reactor_t reactor;
    hk_ret_t status = hk_http_reactor_init(&reactor);
    reactor.acc_map = (hk_dispatcher_acc_map_t *)arg;
    if (status != HK_RET_SUCCESS)
    {
        platform_socket_log("HTTP reactor init fail, Status:%d\n", status);
        return NULL;
    }

    platform_socket_log("HTTP reactor start!\n");
    while (reactor.state == HTTP_RUNNING)
    {
        hk_http_reactor_select(&reactor);
    }
    hk_http_reactor_deinit(&reactor);
}
