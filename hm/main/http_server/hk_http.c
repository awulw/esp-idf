/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#include "hk_http.h"
#include "../dispatcher/hk_dispatcher.h"
#include "picohttpparser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void platform_socket_log(const char *text, ...);
#define LOG_WARNING platform_socket_log

#define HTTP_HEADER_FORMAT_STRING                                                                                      \
    "HTTP/1.1 %s\r\n"                                                                                                  \
    "Content-Length: %zu\r\n"                                                                                          \
    "Content-Type: %s\r\n"                                                                                             \
    "\r\n"

#define HTTP_HEADER_FORMAT_STRING_NO_CONTENT                                                                           \
    "HTTP/1.1 %s\r\n"                                                                                                  \
    "Content-Length: 0\r\n"                                                                                            \
    "\r\n"

#define MAX_HEADERS 15

typedef void (*hk_http_resource_callback_t)(hk_http_socket_t * /*socket*/, hk_byte_stream_t * /*payload*/);

typedef struct
{
    const char *method;
    const char *url;
    hk_http_resource_callback_t callback;
} hk_http_resource_t;

// HTTP resource callbacks

/* HAP */
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_accessories(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_get_characteristics(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_put_characteristics(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_identify(hk_http_socket_t *socket, hk_byte_stream_t *payload);

/* Pairing */
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_pair_setup(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_pair_verify(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_pairings(hk_http_socket_t *socket, hk_byte_stream_t *payload);

/* WAC */
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_auth_setup(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_configure(hk_http_socket_t *socket, hk_byte_stream_t *payload);
HK_NOT_NULL_ARGS(1, 2)
static void hk_http_configured(hk_http_socket_t *socket, hk_byte_stream_t *payload);

static const hk_http_resource_t g_resources[] = {
  /* HAP */
  {.method = "GET", .url = "/accessories", .callback = hk_http_accessories},
  {.method = "GET", .url = "/characteristics", .callback = hk_http_get_characteristics},
  {.method = "PUT", .url = "/characteristics", .callback = hk_http_put_characteristics},
  {.method = "POST", .url = "/identify", .callback = hk_http_identify},
  /* Pairing */
  {.method = "POST", .url = "/pair-setup", .callback = hk_http_pair_setup},
  {.method = "POST", .url = "/pair-verify", .callback = hk_http_pair_verify},
  {.method = "POST", .url = "/pairings", .callback = hk_http_pairings},
  /* W../AC */
  {.method = "POST", .url = "/auth-setup", .callback = hk_http_auth_setup},
  {.method = "POST", .url = "/configure", .callback = hk_http_configure},
  {.method = "POST", .url = "/configured", .callback = hk_http_configured}};


HK_NOT_NULL_ARGS(1, 2, 3)
static hk_ret_t hk_prepend_http_header(const char *status_code, const char *content_type, hk_byte_stream_t *payload)
{
    char temp[256];
    int ret = snprintf(temp, sizeof(temp), HTTP_HEADER_FORMAT_STRING, status_code, payload->data_size, content_type);
    if (ret <= 0 || (size_t)ret > sizeof(temp))
    {
        return HK_RET_FAIL;
    }

    if (!hk_byte_stream_prepend(payload, (const byte_t *)temp, ret))
    {
        return HK_RET_RESOURCES_EXHAUSTED;
    }

    return HK_RET_SUCCESS;
}

HK_NOT_NULL_ARGS(1, 2)
static hk_ret_t hk_prepend_http_header_no_content(const char *status_code, hk_byte_stream_t *payload)
{
    char temp[256];
    int ret = snprintf(temp, sizeof(temp), HTTP_HEADER_FORMAT_STRING_NO_CONTENT, status_code);
    if (ret <= 0 || (size_t)ret > sizeof(temp))
    {
        return HK_RET_FAIL;
    }

    if (!hk_byte_stream_prepend(payload, (const byte_t *)temp, ret))
    {
        return HK_RET_RESOURCES_EXHAUSTED;
    }

    return HK_RET_SUCCESS;
}

void hk_http_respond_without_content(hk_http_socket_t *socket, const char *status_code)
{
    hk_byte_stream_t payload;
    hk_byte_stream_construct(&payload);
    if (hk_prepend_http_header_no_content(status_code, &payload) != HK_RET_SUCCESS)
    {
        LOG_WARNING("Creating response without content failed");
        hk_byte_stream_destroy(&payload);
        hk_http_socket_close(socket);
        return;
    }

    hk_http_socket_send(socket, payload.data, payload.data_size);
    hk_byte_stream_destroy(&payload);
}

hk_ret_t hk_http_enqueue_http_respond(const char *http_status, const char *content_type, int fd,
                                      hk_byte_stream_t *payload)
{
    if (hk_prepend_http_header(http_status, content_type, payload) != HK_RET_SUCCESS)
    {
        platform_socket_log("Prepending http header with content failed");
        return HK_RET_FAIL;
    }
    // TODO (AW) Put respond/ notification in http queue
    const int ret = platform_socket_send(fd, (char *)payload->data, payload->data_size, 0);
    if (ret <= 0)
    {
        platform_socket_log("platform_send error: %d\n", ret);
        return HK_RET_FAIL;
    }
    return HK_RET_SUCCESS;
}

HK_NOT_NULL_ARGS(1, 2, 3, 4)
static void hk_http_respond(hk_http_socket_t *socket, const char *status_code, const char *content_type,
                            hk_byte_stream_t *payload)
{
    if (hk_prepend_http_header(status_code, content_type, payload) != HK_RET_SUCCESS)
    {
        platform_socket_log("Prepending http header with content failed");
        hk_http_socket_close(socket);
        return;
    }
    hk_http_socket_send(socket, payload->data, payload->data_size);
}

static size_t hk_http_heders_size(const char *data, size_t data_len)
{
    char *p_buf = hk_strndup(data, data_len);
    char *p_end = strstr(p_buf, HTTP_END_OF_HEDERS);
    if (p_end == NULL)
    {
        return 0;
    }
    size_t ret = p_end - p_buf;
    free(p_buf);
    return ret + strlen(HTTP_END_OF_HEDERS);
}

static size_t hk_http_payload_size(struct phr_header headers[], size_t num_headers)
{
    for (size_t i = 0; i < num_headers; ++i)
    {
        if (strncmp(headers[i].name, "Content-Length", strlen("Content-Length")) == 0)
        {
            // We need to create a zero terminated string for use by strtol
            char *length_string = hk_strndup(headers[i].value, headers[i].value_len);
            size_t payload_size = strtol(length_string, NULL, 10);
            free(length_string);
            return payload_size;
        }
    }
    return 0;
}

static void http_make_payload_post(hk_byte_stream_t *recv_buffer, size_t heders_size, size_t payload_size)
{
    memmove(recv_buffer->data, recv_buffer->data + heders_size, payload_size);
    recv_buffer->data_size = payload_size;
}
static void http_make_payload_get(hk_byte_stream_t *recv_buffer, const char *url)
{

    char *p_str_start = strstr(url, "?");

    if (p_str_start == NULL)
    {
        // No data in GET request
        recv_buffer->data_size = 0;
    }
    else
    {
        char *p_str_end = strstr(p_str_start, " ");

        if (p_str_end == NULL)
        {
            recv_buffer->data_size = 0;
            return;
        }
        p_str_start += 1;
        size_t url_size = p_str_end - p_str_start;
        recv_buffer->data_size = url_size;
        memmove(recv_buffer->data, p_str_start, url_size);
    }
}

void hk_parse_request(hk_http_socket_t *socket)
{
    const char *method = NULL;
    size_t method_len = 0;
    const char *path = NULL;
    size_t path_len = 0;
    int version = 0;
    struct phr_header headers[MAX_HEADERS] = {{0}};
    size_t num_headers = MAX_HEADERS;

    int ret = phr_parse_request((const char *)socket->recv_buffer.data, socket->recv_buffer.data_size, &method,
                                &method_len, &path, &path_len, &version, &headers[0], &num_headers, 0);
    if (ret < 0)
    {
        if (ret == -2)
        {
            // Request is partial, do not destroy socket buffer.
            return;
        }
        hk_byte_stream_destroy(&socket->recv_buffer);
        platform_socket_log("\nHTTP parse result: %d\n", ret);
        return;
    }
    size_t payload_size = hk_http_payload_size(headers, num_headers);

    size_t heders_size = hk_http_heders_size((char *)socket->recv_buffer.data, socket->recv_buffer.data_size);
    if ((heders_size + payload_size) > socket->recv_buffer.data_size)
    {
        // Request is partial, do not destroy socket buffer.
        platform_socket_log("Message in partial\n");
        return;
    }
    if ((heders_size + payload_size) < socket->recv_buffer.data_size)
    {
        platform_socket_log("Bad request from client: Content-Length is bigger then declared");
        hk_http_respond_without_content(socket, HTTP_STATUS_BAD_REQUEST);
        hk_byte_stream_destroy(&socket->recv_buffer);
        return;
    }

    for (size_t i = 0; i < sizeof(g_resources) / sizeof(hk_http_resource_t); ++i)
    {
        if (strncmp(path, g_resources[i].url, strlen(g_resources[i].url) - 1) == 0 &&
            strncmp(method, g_resources[i].method, method_len) == 0)
        {
            if (strncmp(method, "GET", method_len) == 0)
            {
                http_make_payload_get(&socket->recv_buffer, path);
            }
            else
            {
                http_make_payload_post(&socket->recv_buffer, heders_size, payload_size);
            }
            g_resources[i].callback(socket, &socket->recv_buffer);
            hk_byte_stream_destroy(&socket->recv_buffer);
            return;
        }
    }
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_FOUND);
    hk_byte_stream_destroy(&socket->recv_buffer);
}

/* HAP */
static void hk_http_accessories(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    hk_ret_t status =
      hk_dispatcher_send_http_request(socket->acc_map, HK_DISPATCHER_GET_ACCESSORIES, socket->fd, payload);
    if (status != HK_RET_SUCCESS)
    {
        platform_socket_log("hk_dispatcher_enqueue_http_request error %d", status);
        hk_http_respond_without_content(socket, HTTP_STATUS_SERVICE_UNAVAILABLE);
    }
}

static void hk_http_get_characteristics(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    hk_ret_t status =
      hk_dispatcher_send_http_request(socket->acc_map, HK_DISPATCHER_GET_CHARACTERISTICS, socket->fd, payload);
    if (status != HK_RET_SUCCESS)
    {
        platform_socket_log("hk_dispatcher_enqueue_http_request error %d", status);
        hk_http_respond_without_content(socket, HTTP_STATUS_SERVICE_UNAVAILABLE);
    }
}

static void hk_http_put_characteristics(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_ret_t status =
      hk_dispatcher_send_http_request(socket->acc_map, HK_DISPATCHER_PUT_CHARACTERISTICS, socket->fd, payload);
    if (status != HK_RET_SUCCESS)
    {
        platform_socket_log("hk_dispatcher_enqueue_http_request error %d", status);
        hk_http_respond_without_content(socket, HTTP_STATUS_SERVICE_UNAVAILABLE);
    }
}

static void hk_http_identify(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

/* Pairing */
static void hk_http_pair_setup(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

static void hk_http_pair_verify(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

static void hk_http_pairings(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

/* WAC */
static void hk_http_auth_setup(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

static void hk_http_configure(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}

static void hk_http_configured(hk_http_socket_t *socket, hk_byte_stream_t *payload)
{
    (void)payload;
    hk_http_respond_without_content(socket, HTTP_STATUS_NOT_IMPLEMENTED);
}
