/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#ifndef HK_DISPATCHER_H_
#define HK_DISPATCHER_H_

#include "../utility/hk_byte_stream.h"
#include "../utility/hk_queue.h"
#include "../device/hk_api.h"
#include "../device/hap/hk_characteristic.h"
#include "../device/hap/hk_accessory.h"
#define HK_DISPATCHER_MAX_ACC 10

typedef struct
{
    hk_acc_base_t *acc[HK_DISPATCHER_MAX_ACC];
    size_t acc_map_szie;
    queue_t *hk_dispatcher_incomming_queue;
} hk_dispatcher_acc_map_t;

typedef enum {
    HK_DISPATCHER_GET_ACCESSORIES = 0,
    HK_DISPATCHER_GET_CHARACTERISTICS,
    HK_DISPATCHER_PUT_CHARACTERISTICS,
    HK_DISPATCHER_FORWARD_MSG, // RESPOND
} hk_dispatcher_request_t;

typedef struct
{
    int fd;
    bool is_from_http_serwer;
    int port;
} hk_msg_id_t;

typedef enum
{
    HK_MSG_STR,
	HK_MSG_JSON
} hk_msg_format_t;

typedef struct
{
    hk_dispatcher_request_t request;
    // hk_dispatcher_
    hk_msg_id_t msg_id;
    void *payload;
    size_t payload_size;
    queue_t *queue_return;
    hk_msg_format_t format;
} hk_dispatcher_msg_t;

typedef enum {
    HK_DISPATCHER_RESPOND_OK,
    HK_DISPATCHER_RESPOND_NO_CONTENT,
    HK_DISPATCHER_RESPOND_MULTI_STATUS,
    HK_DISPATCHER_RESPOND_BAD_REQUEST,
    HK_DISPATCHER_RESPOND_UNPROCESSABLE_ENTRY,
} hk_dispatcher_respond_status_t;

typedef struct
{
    hk_byte_stream_t *data_stream;
    hk_dispatcher_respond_status_t status;
} hk_dispatcher_msg_respond_t;

void hk_dispatcher_task(void *arg);
hk_ret_t hk_dispatcher_enqueue_msg(hk_dispatcher_acc_map_t *acc_map, hk_byte_stream_t *payload,
                                   hk_dispatcher_msg_t *msg);
hk_ret_t hk_dispatcher_send_http_request(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_request_t type, int fd,
                                         hk_byte_stream_t *payload);
hk_ret_t hk_dispatcher_register_accessory(hk_dispatcher_acc_map_t *acc_map, hk_acc_base_t *acc);
void hk_dispatcher_msg_split(hk_dispatcher_msg_t **msg, const hk_byte_stream_t *raw_data);
hk_ret_t hk_dispatcher_msg_merge(const hk_dispatcher_msg_t *msg, hk_byte_stream_t *payload);

hk_dispatcher_acc_map_t *hk_dispatcher_new(void);
void hk_dispatcher_delete(hk_dispatcher_acc_map_t *acc_map);
hk_ret_t hk_dispatcher_transaction(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_request_t request, byte_t *data, size_t data_len, int fd);
#endif /* HK_DISPATCHER_H_ */
