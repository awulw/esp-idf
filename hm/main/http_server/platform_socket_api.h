/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#ifndef PLATFORM_SOCKET_API_H_
#define PLATFORM_SOCKET_API_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <netdb.h>
#include <string.h>

int32_t platform_socket_socket(int32_t family, int32_t type, int32_t protocol);
int32_t platform_socket_bind(int32_t sock, struct sockaddr *addr, size_t addrlen);
int32_t platform_socket_listen(int32_t sock, int32_t backlog);
int32_t platform_socket_accept(int32_t sock, struct sockaddr *cliaddr, size_t *addrlen);
int32_t platform_socket_select(fd_set *rd, fd_set *wr, fd_set *ex, uint32_t timeout_ms);
int32_t platform_socket_send(int32_t sock, char *buf, int32_t len, int32_t flags);
int32_t platform_socket_recv(int32_t sock, char *buf, int32_t len, int32_t flags);
int32_t platform_socket_close(int32_t sock);
int32_t platform_socket_set_option(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int32_t platform_socket_setsockopt(int sockfd, int level, int optname, void *optval, socklen_t optlen);

#endif /* PLATFORM_SOCKET_API_H_ */
