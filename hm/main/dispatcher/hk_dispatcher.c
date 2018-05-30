/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#include "hk_dispatcher.h"
#include "../device/hap/hk_characteristic.h"
#include "../http_server/hk_http.h"
#include "../json/hk_json.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define CHR_MAX_ON_SINGLE_REQUEST 64

extern void hk_console_printf(const char *text, ...);
extern hk_ret_t hk_http_enqueue_http_respond(const char *http_status, const char *content_type, int fd,
                                      hk_byte_stream_t *payload);

void hk_dispatcher_init(hk_dispatcher_acc_map_t *acc_map)
{
    acc_map->hk_dispatcher_incomming_queue = calloc(1, sizeof(queue_t));
    queue_init(acc_map->hk_dispatcher_incomming_queue, QUOEUE_MAX_LENGTH);
    acc_map->acc_map_szie = 0;
}

static size_t hk_dispatcher_parse_id_from_str(hk_chr_request_t *chr, const char *str, size_t str_len)
{
    char *str_cp = hk_strndup(str, str_len);
    char *ptr = strstr(str_cp, "id=");
    if (strstr(str_cp, "meta=1"))
        chr->meta = true;
    if (strstr(str_cp, "perms=1"))
        chr->perms = true;
    if (strstr(str_cp, "type=1"))
        chr->type = true;
    if (strstr(str_cp, "ev=1"))
        chr->ev = true;
    if (ptr == NULL)
    {
        free(str_cp);
        return 0;
    }



	#define VALUE_STR "value="
	char *v_ptr = strstr(ptr, VALUE_STR);
	if (v_ptr)
		sscanf(v_ptr + sizeof(VALUE_STR) - 1, "%d", &(chr->value.i32));

    ptr += 2;
    for (size_t i = 0; i < CHR_MAX_ON_SINGLE_REQUEST; i++)
    {
    	//if (str_cp - ptr > str_len - 2) break;

    	sscanf(ptr + 1, "%d", (int *)(&(chr->id[i].aid)));
        ptr = strstr(ptr, ".");
        if (ptr == NULL)
        {
            free(str_cp);
            return 0;
        }

        //if (str_cp - ptr > str_len - 2) break;

        sscanf(ptr + 1, "%d", (int *)(&(chr->id[i].iid)));
        ptr = strstr(ptr, ",");
        if (ptr == NULL)
        {
            free(str_cp);
            chr->id_size = i + 1;
            return i + 1;
        }
    }

    free(str_cp);
    return 0;
}

hk_acc_base_t *hk_dispatcher_find_aid(hk_dispatcher_acc_map_t *map, hk_accessory_id_t aid)
{
    for (size_t i = 0; i < map->acc_map_szie; i++)
    {
        if (map->acc[i]->aid == aid)
            return map->acc[i];
    }
    return NULL;
}

static void hk_dispatcher_read_chr(hk_dispatcher_acc_map_t *acc_map, const hk_chr_request_t *chr_requsest, size_t size,
                                   hk_chr_respond_t *chr_respond)
{
    hk_acc_base_t *acc;
    hk_chr_base_t *chr;
    for (size_t i = 0; i < size; i++)
    {
        memset(&chr_respond[i], 0, sizeof(hk_chr_respond_t));
        chr_respond[i].status = HK_HAP_STATUS_RESOURCES_DOES_NOT_EXIST;
        chr_respond[i].id.iid = chr_requsest->id[i].iid;
        chr_respond[i].id.aid = chr_requsest->id[i].aid;
        chr_respond[i].chr = NULL;
        acc = hk_dispatcher_find_aid(acc_map, chr_requsest->id[i].aid);
        if (acc)
        {
        	chr = hk_chr_find_by_iid(acc, chr_requsest->id[i].iid);
            if (chr)
            {

                if (chr->metadata->permissions.paired_read == true)
                {
                    chr_respond[i].status = 0;
                    if (chr->on_read != NULL)
                    {
                        chr_respond[i].status = chr->on_read(chr);
                    }
                    chr_respond[i].chr = chr;
                }
                else
                {
                    chr_respond[i].status = HK_HAP_STATUS_W_ONLY_CHR;
                }
            }
        }
    }
}

static void hk_dispatcher_get_accessories(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_msg_respond_t *respond)
{
    if (hk_json_put_acc(acc_map, respond->data_stream) == HK_RET_SUCCESS)
    {
        respond->status = HK_DISPATCHER_RESPOND_OK;
        return;
    }
    respond->status = HK_DISPATCHER_RESPOND_UNPROCESSABLE_ENTRY;
}

static void hk_dispatcher_get_chr(hk_dispatcher_acc_map_t *acc_map, void *request, size_t request_size,
                                  hk_dispatcher_msg_respond_t *respond)
{
    hk_chr_request_t *chr_request = hk_chr_request_new(CHR_MAX_ON_SINGLE_REQUEST);
    hk_chr_respond_t chr_respond[CHR_MAX_ON_SINGLE_REQUEST];
    size_t chr_requsest_size;

    respond->status = HK_DISPATCHER_RESPOND_BAD_REQUEST;
    chr_requsest_size = hk_dispatcher_parse_id_from_str(chr_request, request, request_size);
    if (chr_requsest_size > 0)
    {
        hk_dispatcher_read_chr(acc_map, chr_request, chr_requsest_size, chr_respond);
        respond->status = HK_DISPATCHER_RESPOND_OK;
        if (chr_requsest_size > 1)
            respond->status = HK_DISPATCHER_RESPOND_MULTI_STATUS;
    }
    hk_json_put_chrs(chr_respond, chr_request, respond->data_stream, false);
    free(chr_request->id);
    free(chr_request);

}

void hk_dispatcher_write_chr(hk_dispatcher_acc_map_t *acc_map, hk_chr_write_t *chrs_w, size_t chr_to_write_size,
                             hk_chr_respond_t *chrs_respond)
{
    hk_acc_base_t *acc;
    hk_chr_base_t *chr;
    for (size_t i = 0; i < chr_to_write_size; i++)
    {
        chrs_respond[i].id.iid = chrs_w[i].id.iid;
        chrs_respond[i].id.aid = chrs_w[i].id.aid;
        chrs_respond[i].status = 0;
        chrs_respond[i].chr = NULL;
        acc = hk_dispatcher_find_aid(acc_map, chrs_w[i].id.aid);
        if (acc == NULL)
        {
            chrs_respond[i].status = HK_HAP_STATUS_RESOURCES_DOES_NOT_EXIST;
            continue;
        }
        chr = hk_chr_find_by_iid(acc, chrs_w[i].id.iid);
        if (chr == NULL)
        {
            chrs_respond[i].status = HK_HAP_STATUS_RESOURCES_DOES_NOT_EXIST;
            continue;
        }
        if (!hk_chr_validate_value(&(chrs_w[i].value), chr->metadata))
        {
            chrs_respond[i].status = HK_HAP_STATUS_INVALID_VALUE;
            continue;
        }
        if (chr->metadata->permissions.paired_write == false)
        {
            chrs_respond[i].status = HK_HAP_STATUS_R_ONLY_CHR;
            continue;
        }
        chrs_respond[i].chr = chr;
        hk_chr_copy_value(&chr->value, &chrs_w[i].value);
        if (chr->on_write != NULL)
        {
            chrs_respond[i].status = chr->on_write(chr);
        }
    }
}

static void hk_dispatcher_put_chr(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_msg_t *msg,
                                  hk_dispatcher_msg_respond_t *respond)
{
    hk_chr_write_t chr_to_write[CHR_MAX_ON_SINGLE_REQUEST];
    hk_chr_respond_t chr_respond[CHR_MAX_ON_SINGLE_REQUEST];
    size_t chr_to_write_size;
    respond->status = HK_DISPATCHER_RESPOND_BAD_REQUEST;

    if (msg->format == HK_MSG_JSON)
    {
		if (hk_json_parse_chr_id(msg->payload, chr_to_write, &chr_to_write_size) != HK_RET_SUCCESS)
		{
			respond->status = HK_DISPATCHER_RESPOND_BAD_REQUEST;
			return;
		}
    }

    if (msg->format == HK_MSG_STR)
    {
    	//TODO Only int format is supported, need to add others.
    	hk_chr_request_t *chr_request = hk_chr_request_new(1);
    	hk_dispatcher_parse_id_from_str(chr_request, msg->payload, msg->payload_size);

    	chr_to_write[0].id.aid = chr_request->id[0].aid;
		chr_to_write[0].id.iid = chr_request->id[0].iid;
		chr_to_write[0].value.i32 = chr_request->value.i32;
		chr_to_write_size = 1;
		free(chr_request->id);
		free(chr_request);
    }

    hk_dispatcher_write_chr(acc_map, chr_to_write, chr_to_write_size, chr_respond);
    hk_chr_request_t chr_request = {NULL, chr_to_write_size, 0, 0, 0, 0, {0}};
    if (hk_chr_respond_check_status(chr_respond, &chr_request))
    {
        hk_json_put_chrs(chr_respond, &chr_request, respond->data_stream, true);
        respond->status = HK_DISPATCHER_RESPOND_OK;
        if (chr_to_write_size > 1)
        {
            respond->status = HK_DISPATCHER_RESPOND_MULTI_STATUS;
        }
        return;
    }
    respond->status = HK_DISPATCHER_RESPOND_NO_CONTENT;
}

hk_ret_t hk_dispatcher_msg_merge(const hk_dispatcher_msg_t *msg, hk_byte_stream_t *payload)
{
    if (payload->data_size + sizeof(hk_dispatcher_msg_t) > payload->data_capacity)
    {
        if (!hk_byte_stream_reserve(payload, payload->data_size + sizeof(hk_dispatcher_msg_t)))
        {
            return HK_RET_FAIL;
        }
    }
    memmove(payload->data + sizeof(hk_dispatcher_msg_t), payload->data, payload->data_size);
    memmove(payload->data, msg, sizeof(hk_dispatcher_msg_t));
    payload->data_size += sizeof(hk_dispatcher_msg_t);
    return HK_RET_SUCCESS;
}

void hk_dispatcher_msg_split(hk_dispatcher_msg_t **msg, const hk_byte_stream_t *raw_data)
{
    *msg = (hk_dispatcher_msg_t *)raw_data->data;
    (*msg)->payload_size = raw_data->data_size - sizeof(hk_dispatcher_msg_t);
    (*msg)->payload = raw_data->data + sizeof(hk_dispatcher_msg_t);
}

hk_ret_t hk_dispatcher_enqueue_msg(hk_dispatcher_acc_map_t *acc_map, hk_byte_stream_t *payload,
                                   hk_dispatcher_msg_t *msg)
{
    if (hk_dispatcher_msg_merge(msg, payload) == HK_RET_SUCCESS)
    {
        return queue_push(acc_map->hk_dispatcher_incomming_queue, payload->data, payload->data_size);
    }
    return HK_RET_RESOURCES_EXHAUSTED;
}

static void hk_dispatcher_send_respond(const hk_dispatcher_msg_t *msg, hk_dispatcher_msg_respond_t *respond)
{
    if (msg->msg_id.is_from_http_serwer)
    {
        switch (respond->status)
        {
            case HK_DISPATCHER_RESPOND_OK:
                hk_http_enqueue_http_respond(HTTP_STATUS_OK, HTTP_CONTENT_TYPE_JSON, msg->msg_id.fd,
                                             respond->data_stream);
                break;
            case HK_DISPATCHER_RESPOND_MULTI_STATUS:
                hk_http_enqueue_http_respond(HTTP_STATUS_MULTI_STATUS, HTTP_CONTENT_TYPE_JSON, msg->msg_id.fd,
                                             respond->data_stream);
                break;
            case HK_DISPATCHER_RESPOND_NO_CONTENT:
                hk_http_enqueue_http_respond(HTTP_STATUS_NO_CONTENT, "", msg->msg_id.fd, respond->data_stream);
                break;
            default:
                hk_http_enqueue_http_respond(HTTP_STATUS_SERVICE_UNAVAILABLE, HTTP_CONTENT_TYPE_JSON, msg->msg_id.fd,
                                             respond->data_stream);
        }
        return;
    }
    if (msg->queue_return != NULL)
    {
        queue_push(msg->queue_return, respond->data_stream->data, respond->data_stream->data_size);
        return;
    }
    	else
    {
    	write(msg->msg_id.fd, respond->data_stream->data, respond->data_stream->data_size);
    }
}

static void hk_dispatcher_process_msg(hk_dispatcher_acc_map_t *acc_map, hk_byte_stream_t *msg_stream)
{
    hk_dispatcher_msg_respond_t respond;
    hk_byte_stream_t respond_data_stream;
    hk_byte_stream_construct(&respond_data_stream);
    respond.data_stream = &respond_data_stream;
    hk_dispatcher_msg_t *msg;
    hk_dispatcher_msg_split(&msg, msg_stream);

    switch (msg->request)
    {
        case HK_DISPATCHER_GET_CHARACTERISTICS:

            hk_dispatcher_get_chr(acc_map, (void *)msg->payload, msg->payload_size, &respond);
            hk_dispatcher_send_respond(msg, &respond);
            break;
        case HK_DISPATCHER_GET_ACCESSORIES:
            hk_dispatcher_get_accessories(acc_map, &respond);
            hk_dispatcher_send_respond(msg, &respond);
            break;
        case HK_DISPATCHER_PUT_CHARACTERISTICS:
            hk_dispatcher_put_chr(acc_map, msg, &respond);
            hk_dispatcher_send_respond(msg, &respond);
            break;
        case HK_DISPATCHER_FORWARD_MSG: // forward
            if (hk_byte_stream_reserve(respond.data_stream, msg->payload_size))
            {
                memcpy(respond.data_stream->data, msg->payload, msg->payload_size);
                respond.data_stream->data_size = msg->payload_size;
                respond.status = HK_DISPATCHER_RESPOND_OK;
            }
            else
            {
                respond.status = HK_DISPATCHER_RESPOND_UNPROCESSABLE_ENTRY;
            }
            hk_dispatcher_send_respond(msg, &respond);
            break;
            // TODO (AW) implement other request
    }
    hk_byte_stream_destroy(respond.data_stream);
}

void hk_dispatcher_task(void *arg)
{
    hk_byte_stream_t msg_buf;
    hk_byte_stream_construct(&msg_buf);
    hk_dispatcher_acc_map_t *acc_map;
    acc_map = (hk_dispatcher_acc_map_t *)arg;

    while (1)
    {
        queue_wait_until_empty(acc_map->hk_dispatcher_incomming_queue);
        while (!queue_is_empty(acc_map->hk_dispatcher_incomming_queue))
        {
            size_t data_size = queue_pop_data_size(acc_map->hk_dispatcher_incomming_queue);
            if (!hk_byte_stream_reserve(&msg_buf, data_size))
            {
                // TODO (AW) error handling
                queue_pop(acc_map->hk_dispatcher_incomming_queue, NULL, NULL);
                continue;
            }
            queue_pop(acc_map->hk_dispatcher_incomming_queue, msg_buf.data, &msg_buf.data_size);
            hk_dispatcher_process_msg(acc_map, &msg_buf);
        }
    }
}

hk_ret_t hk_dispatcher_send_http_request(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_request_t request, int fd,
                                         hk_byte_stream_t *payload)
{
    hk_dispatcher_msg_t msg;
    msg.request = request;
    msg.msg_id.fd = fd;
    msg.msg_id.is_from_http_serwer = true;

    return hk_dispatcher_enqueue_msg(acc_map, payload, &msg);
}

hk_ret_t hk_dispatcher_register_accessory(hk_dispatcher_acc_map_t *acc_map, hk_acc_base_t *acc)
{
    if (acc_map->acc_map_szie >= HK_DISPATCHER_MAX_ACC)
        return HK_RET_FAIL;
    acc_map->acc[acc_map->acc_map_szie] = acc;
    acc_map->acc_map_szie++;
    return HK_RET_SUCCESS;
}

hk_dispatcher_acc_map_t *hk_dispatcher_new(void)
{
    hk_dispatcher_acc_map_t *acc_map;
    acc_map = calloc(1, sizeof(hk_dispatcher_acc_map_t));
    hk_dispatcher_init(acc_map);
    return acc_map;
}

void hk_dispatcher_delete(hk_dispatcher_acc_map_t *acc_map)
{
    free(acc_map->hk_dispatcher_incomming_queue);
    free(acc_map);
}


hk_ret_t hk_dispatcher_transaction(hk_dispatcher_acc_map_t *acc_map, hk_dispatcher_request_t request, byte_t *data, size_t data_len, int fd)
{
	//queue_t queue_return;
	hk_dispatcher_msg_t msg;

	//queue_init(&queue_return, QUOEUE_MAX_LENGTH);
	msg.queue_return = NULL;
	msg.request = request;
	msg.msg_id.fd = fd;
	msg.msg_id.is_from_http_serwer = false;
	msg.format = HK_MSG_STR;

	hk_byte_stream_t payload;
	hk_byte_stream_construct(&payload);

	if (data != NULL)
	{
		hk_byte_stream_append(&payload, (byte_t *)data, data_len);
	}

	hk_dispatcher_enqueue_msg(acc_map, &payload, &msg);
	hk_byte_stream_destroy(&payload);
    return 0;
}

