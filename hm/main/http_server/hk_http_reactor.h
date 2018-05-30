/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#ifndef HK_REACTOR_H
#define HK_REACTOR_H

#include "../device/hk_api.h"
#include "../utility/hk_byte_stream.h"
#include "../dispatcher/hk_dispatcher.h"

#define HK_IP_TIMEOUT 2000 // ms
#define HK_IP_PORT 8080
#define HK_IP_SOCKETS_SIZE 9 // 8 + 1 (5.2.3 TCP Requirements)
#define HK_IP_MAX_BUF_LEN 1500
#define HK_WIFI_CONNECTED_MASK 0xff
#define HAP_HTTP_RECV_BUFFER_SIZE 1576
#define HAP_HTTP_RECV_DYNAMIC_BUFER_MAX_LENGTH 10000
#define HAP_HTTP_OUTGOING_FRAGMENT_MAX_SIZE 1400

typedef struct hk_fragment_queue_item
{
    struct hk_fragment_queue_item *previous, *next;
    size_t size;
    uint8_t data[HAP_HTTP_OUTGOING_FRAGMENT_MAX_SIZE]; // TODO(DJ): use byte stream
} hk_fragment_queue_item_t;

typedef int hk_http_socket_fd_t;

typedef struct
{
    hk_http_socket_fd_t fd;
    bool close_after_flush;
    hk_byte_stream_t recv_buffer;
    hk_dispatcher_acc_map_t *acc_map;
    // hk_pairing_state_machine_t session_state_machine;
} hk_http_socket_t;

void hk_reactor_task(void *arg);

HK_NOT_NULL_ARGS(1, 2)
void hk_http_socket_send(hk_http_socket_t *socket, const uint8_t *data, size_t length);

HK_NOT_NULL_ARGS(1)
void hk_http_socket_close(hk_http_socket_t *socket);

#endif // HK_REACTOR_H
