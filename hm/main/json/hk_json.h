/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#ifndef SRC_JSON_HK_JSON_H_
#define SRC_JSON_HK_JSON_H_

#include "../dispatcher/hk_dispatcher.h"
#include "../device/hap/hk_characteristic.h"

HK_NOT_NULL_ARGS(1, 2, 3)
hk_ret_t hk_json_put_chrs(hk_chr_respond_t *chr, hk_chr_request_t *r_chr, hk_byte_stream_t *data_stream, bool noValue);
HK_NOT_NULL_ARGS(1, 2, 3)
hk_ret_t hk_json_parse_chr_id(char *data, hk_chr_write_t *chr_to_write, size_t *chr_to_write_size);
HK_NOT_NULL_ARGS(1, 2)
hk_ret_t hk_json_put_acc(hk_dispatcher_acc_map_t *acc_map, hk_byte_stream_t *data_stream);

#endif /* SRC_JSON_HK_JSON_H_ */
