/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#ifndef HK_HTTP_H
#define HK_HTTP_H

#include "hk_http_reactor.h"
#include "platform_socket_api.h"
#include "../utility/hk_byte_stream.h"
#include "../dispatcher/hk_dispatcher.h"
#include "../device/hk_api.h"

#define HTTP_END_OF_HEDERS "\r\n\r\n"
#define HTTP_STATUS_OK "200 OK"
#define HTTP_STATUS_NO_CONTENT "204 No Content"
#define HTTP_STATUS_MULTI_STATUS "207 Multi Status"
#define HTTP_STATUS_BAD_REQUEST "400 Bad Request"
#define HTTP_STATUS_NOT_FOUND "404 Not found"
#define HTTP_STATUS_TOO_MANY_REQUESTS "429 Too Many Requests"
#define HTTP_STATUS_INTERNAL_SERVER_ERROR "500 Internal Server Error"
#define HTTP_STATUS_NOT_IMPLEMENTED "501 Not Implemented"
#define HTTP_STATUS_SERVICE_UNAVAILABLE "503 Service unavailable"

#define HTTP_CONTENT_TYPE_JSON "application/hap+json"
#define HTTP_CONTENT_TYPE_TLV "application/pairing+tlv8"

HK_NOT_NULL_ARGS(1)
void hk_parse_request(hk_http_socket_t *socket);

HK_NOT_NULL_ARGS(1, 2)
void hk_http_respond_without_content(hk_http_socket_t *socket, const char *status_code);
hk_ret_t hk_http_enqueue_http_respond(const char *http_status, const char *content_type, int fd,
                                      hk_byte_stream_t *payload);

#endif // HK_HTTP_H
