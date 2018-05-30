/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/
#include "hk_json.h"
#include "cJSON.h"
#include <stdio.h>

static const char *hk_json_format = "format";
static const char *hk_json_unit = "unit";
static const char *hk_json_minValue = "minValue";
static const char *hk_json_maxValue = "maxValue";
static const char *hk_json_minStep = "minStep";
static const char *hk_json_maxLen = "maxLen";
static const char *hk_json_value = "value";

#define HK_JSON_TYPE_STR_SIZE 5
#define HK_JSON_MAX_VALUE_STR_SIZE 64

extern void hk_console_printf(const char *text, ...);

static void hk_json_put_chr_format(cJSON *root, hk_chr_base_t *chr_r);
HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_unit(cJSON *root, hk_chr_base_t *chr_r);
HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_minmax(cJSON *root, hk_chr_base_t *chr_r);
HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_type(cJSON *root, hk_chr_base_t *chr_r);

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_meta(cJSON *root, hk_chr_base_t *chr)
{
    hk_json_put_chr_format(root, chr);
    hk_json_put_chr_unit(root, chr);
    hk_json_put_chr_minmax(root, chr);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_uuid2str(const uint8_t *uuid, char *uuid_str)
{
    unsigned int uuid_temp = 0x00FF & uuid[3];
    uuid_temp |= 0xFF00 & uuid[2] << 8;
    sprintf(uuid_str, "%X", uuid_temp);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_type(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    char str_type[HK_JSON_TYPE_STR_SIZE];
    hk_json_uuid2str(chr->metadata->uuid, str_type);
    cJSON_AddStringToObject(root, "type", str_type);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_ev(cJSON *root, hk_chr_respond_t *chr_r)
{
    if (chr_r->chr == NULL)
        return;
    cJSON_AddBoolToObject(root, "ev", chr_r->chr->metadata->permissions.events);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_perms(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    cJSON *perms_array = cJSON_CreateArray();
    if (chr->metadata->permissions.paired_read)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("pr"));
    if (chr->metadata->permissions.paired_write)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("pw"));
    if (chr->metadata->permissions.events)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("ev"));
    if (chr->metadata->permissions.additional_auth)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("aa"));
    if (chr->metadata->permissions.timed_write)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("tw"));
    if (chr->metadata->permissions.hidden)
        cJSON_AddItemToArray(perms_array, cJSON_CreateString("hd"));
    cJSON_AddItemToObject(root, "perms", perms_array);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_minmax(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    switch (chr->value.format)
    {
    	case HK_VALUE_FORMAT_BOOL:
    		break;
    	case HK_VALUE_FORMAT_STR:
    		break;
    	case HK_VALUE_FORMAT_TLV8:
    		break;
    	case HK_VALUE_FORMAT_DATA:
    		break;

        case HK_VALUE_FORMAT_U8:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.u8);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.u8);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.u8);
            break;
        case HK_VALUE_FORMAT_U16:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.u16);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.u16);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.u16);
            break;
        case HK_VALUE_FORMAT_U32:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.u32);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.u32);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.u32);
            break;
        case HK_VALUE_FORMAT_U64:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.u64);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.u64);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.u64);
            break;
        case HK_VALUE_FORMAT_I32:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.i32);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.i32);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.i32);
            break;
        case HK_VALUE_FORMAT_FLOAT:
            if (chr->metadata->validate.min)
                cJSON_AddNumberToObject(root, hk_json_minValue, chr->metadata->min.f);
            if (chr->metadata->validate.max)
                cJSON_AddNumberToObject(root, hk_json_maxValue, chr->metadata->max.f);
            if (chr->metadata->validate.step)
                cJSON_AddNumberToObject(root, hk_json_minStep, chr->metadata->step.f);
            break;
        case HK_VALUE_FORMAT_NULL:
        	break;
    }
}

HK_NOT_NULL_ARGS(1)
static void hk_json_put_chr_format(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    switch (chr->metadata->format)
    {
		case HK_VALUE_FORMAT_TLV8:
			break;
		case HK_VALUE_FORMAT_DATA:
			break;
        case HK_VALUE_FORMAT_BOOL:
            cJSON_AddStringToObject(root, hk_json_format, "bool");
            break;
        case HK_VALUE_FORMAT_U8:
            cJSON_AddStringToObject(root, hk_json_format, "uint8"); // need to check if dont behaves as char
            break;
        case HK_VALUE_FORMAT_U16:
            cJSON_AddStringToObject(root, hk_json_format, "uint16");
            break;
        case HK_VALUE_FORMAT_U32:
            cJSON_AddStringToObject(root, hk_json_format, "uint");
            break;
        case HK_VALUE_FORMAT_U64:
            cJSON_AddStringToObject(root, hk_json_format, "uint32");
            break;
        case HK_VALUE_FORMAT_I32:
            cJSON_AddStringToObject(root, hk_json_format, "int");
            break;
        case HK_VALUE_FORMAT_FLOAT:
            cJSON_AddStringToObject(root, hk_json_format, "float");
            break;
        case HK_VALUE_FORMAT_STR:
            cJSON_AddStringToObject(root, hk_json_format, "string");
            break;
        case HK_VALUE_FORMAT_NULL:
            cJSON_AddStringToObject(root, hk_json_format, "null");
            break;
    }
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_chr_unit(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    switch (chr->metadata->unit)
    {
        case HK_UNIT_UNITLESS:
            cJSON_AddStringToObject(root, hk_json_unit, "untiless");
            break;
        case HK_UNIT_CELSIUS:
            cJSON_AddStringToObject(root, hk_json_unit, "celsius"); // need to check if dont behaves as char
            break;
        case HK_UNIT_PERCENTAGE:
            cJSON_AddStringToObject(root, hk_json_unit, "percentage");
            break;
        case HK_UNIT_ARCDEGREES:
            cJSON_AddStringToObject(root, hk_json_unit, "arcdegrees");
            break;
        case HK_UNIT_LUX:
            cJSON_AddStringToObject(root, hk_json_unit, "lux");
            break;
        case HK_UNIT_SECONDS:
            cJSON_AddStringToObject(root, hk_json_unit, "seconds");
            break;
    }
    return;
}

HK_NOT_NULL_ARGS(1)
static void hk_json_put_chr_value(cJSON *root, hk_chr_base_t *chr)
{
    if (chr == NULL)
        return;
    switch (chr->value.format)
    {
		case HK_VALUE_FORMAT_TLV8:
			break;
		case HK_VALUE_FORMAT_DATA:
			break;
		case HK_VALUE_FORMAT_NULL:
			break;
        case HK_VALUE_FORMAT_BOOL:
            cJSON_AddBoolToObject(root, hk_json_value, chr->value.b);
            break;
        case HK_VALUE_FORMAT_U8:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.u8); // need to check if dont behaves as char
            break;
        case HK_VALUE_FORMAT_U16:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.u16);
            break;
        case HK_VALUE_FORMAT_U32:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.u32);
            break;
        case HK_VALUE_FORMAT_U64:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.u64);
            break;
        case HK_VALUE_FORMAT_I32:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.i32);
            break;
        case HK_VALUE_FORMAT_FLOAT:
            cJSON_AddNumberToObject(root, hk_json_value, chr->value.f);
            break;
        case HK_VALUE_FORMAT_STR:
            cJSON_AddStringToObject(root, hk_json_value, chr->value.str);
    }
}

hk_ret_t hk_json_put_chrs(hk_chr_respond_t *chr, hk_chr_request_t *chr_request, hk_byte_stream_t *data_stream,
                          bool noValue)
{
    hk_ret_t ret = HK_RET_FAIL;
    cJSON *root = cJSON_CreateObject();
    cJSON *chr_o[HK_DISPATCHER_MAX_ACC];
    cJSON *array = cJSON_CreateArray();
    cJSON *chr_json;
    bool show_status = hk_chr_respond_check_status(chr, chr_request);
    hk_accessory_id_t prv_aid = 0;

    for (size_t i = 0; i < chr_request->id_size; i++)
    {

        chr_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(chr_json, "aid", chr[i].id.aid);
        cJSON_AddNumberToObject(chr_json, "iid", chr[i].id.iid);
        if (show_status)
            cJSON_AddNumberToObject(chr_json, "status", chr[i].status);
        if (chr_request->type)
            hk_json_put_chr_type(chr_json, (chr[i]).chr);
        if (chr_request->meta)
            hk_json_put_chr_meta(chr_json, (chr[i]).chr);
        if (chr_request->perms)
            hk_json_put_perms(chr_json, (chr[i]).chr);
        if (chr_request->ev)
            hk_json_put_chr_ev(chr_json, &(chr[i]));
        if (!noValue && (chr[i].status == 0))
            hk_json_put_chr_value(chr_json, (chr[i]).chr);
        cJSON_AddItemToArray(array, chr_json);
    }
    cJSON_AddItemToObject(root, "characteristics", array);
    char *json = cJSON_Print(root);

    if (hk_byte_stream_append(data_stream, (uint8_t *)json, strlen(json)))
    {
        ret = HK_RET_SUCCESS;
    }
    ret = HK_RET_FAIL;
    cJSON_Delete(root);
    return ret;
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_joson_put_all_chr(cJSON *srv, hk_chr_base_t **chr, size_t chr_size)
{
    cJSON *chr_array = cJSON_CreateArray();
    for (size_t chr_nr = 0; chr_nr < chr_size; chr_nr++)
    {
        cJSON *chr_o = cJSON_CreateObject();
        hk_json_put_chr_type(chr_o, chr[chr_nr]);
        cJSON_AddNumberToObject(chr_o, "iid", chr[chr_nr]->iid);
        hk_json_put_chr_value(chr_o, chr[chr_nr]);
        hk_json_put_perms(chr_o, chr[chr_nr]);
        hk_json_put_chr_format(chr_o, chr[chr_nr]);
        cJSON_AddItemToArray(chr_array, chr_o);
    }
    cJSON_AddItemToObject(srv, "characteristics", chr_array);
}

HK_NOT_NULL_ARGS(1, 2)
static void hk_json_put_all_srv(cJSON *acc, hk_srv_base_t **srv, size_t srv_size)
{
    cJSON *srv_array = cJSON_CreateArray();
    for (size_t srv_nr = 0; srv_nr < srv_size; srv_nr++)
    {
        cJSON *srv_o = cJSON_CreateObject();
        char str_type[HK_JSON_TYPE_STR_SIZE];
        hk_json_uuid2str(srv[srv_nr]->metadata.uuid, str_type);
        cJSON_AddStringToObject(srv_o, "type", str_type);
        cJSON_AddNumberToObject(srv_o, "iid", srv[srv_nr]->iid);
        hk_joson_put_all_chr(srv_o, srv[srv_nr]->chr, srv[srv_nr]->chr_size);
        cJSON_AddItemToArray(srv_array, srv_o);
    }
    cJSON_AddItemToObject(acc, "services", srv_array);
}

hk_ret_t hk_json_put_acc(hk_dispatcher_acc_map_t *acc_map, hk_byte_stream_t *data_stream)
{
    hk_ret_t ret = HK_RET_FAIL;
    cJSON *root = cJSON_CreateObject();
    cJSON *acc_array = cJSON_CreateArray();

    for (size_t acc_nr = 0; acc_nr < acc_map->acc_map_szie; acc_nr++)
    {
        cJSON *acc = cJSON_CreateObject();
        cJSON_AddNumberToObject(acc, "aid", acc_map->acc[acc_nr]->aid);
        hk_json_put_all_srv(acc, acc_map->acc[acc_nr]->srv, acc_map->acc[acc_nr]->srv_size);
        cJSON_AddItemToArray(acc_array, acc);
    }
    cJSON_AddItemToObject(root, "accessories", acc_array);

    char *json = cJSON_Print(root);

    if (hk_byte_stream_append(data_stream, (uint8_t *)json, strlen(json)))
    {
        ret = HK_RET_SUCCESS;
    }
    else
    {
        ret = HK_RET_RESOURCES_EXHAUSTED;
    }
    cJSON_Delete(root);
    return ret;
}
hk_ret_t hk_json_parse_chr_id(char *data, hk_chr_write_t *chr_to_write, size_t *chr_to_write_size)
{
    cJSON *root = cJSON_Parse(data);
    cJSON *chrs_array = cJSON_GetObjectItem(root, "characteristics");

    int chr_nr = cJSON_GetArraySize(chrs_array);
    int aid;
    int iid;
    int value;

    if (chr_nr == 0)
    {
        cJSON_Delete(root);
        return HK_RET_FAIL;
    }
    for (int i = 0; i < chr_nr; i++)
    {
        cJSON *chr_o = cJSON_GetArrayItem(chrs_array, i);
        cJSON *aid_o = cJSON_GetObjectItem(chr_o, "aid");
        cJSON *iid_o = cJSON_GetObjectItem(chr_o, "iid");
        cJSON *value_o = cJSON_GetObjectItem(chr_o, "value");
        memset(&(chr_to_write[i].value), 0, sizeof(hk_value_t));
        if (!(cJSON_IsNumber(aid_o) && cJSON_IsNumber(iid_o)))
        {
            return HK_RET_FAIL;
        }
        sscanf(cJSON_Print(iid_o), "%d", &iid);
        sscanf(cJSON_Print(aid_o), "%d", &aid);
        if (cJSON_IsNumber(value_o))
        {
            char *value_str = cJSON_Print(value_o);
            if (strstr(value_str, ".") == NULL)
            {
                sscanf(cJSON_Print(value_o), "%d", &value);
                chr_to_write[i].value.i32 = value; // value;
                chr_to_write[i].value.format = HK_VALUE_FORMAT_I32;
            }
            else
            {
                float value;
                sscanf(cJSON_Print(value_o), "%f", &value);
                chr_to_write[i].value.f = value;
                chr_to_write[i].value.format = HK_VALUE_FORMAT_FLOAT;
            }
        }
        else if (cJSON_IsTrue(value_o))
        {
            chr_to_write[i].value.format = HK_VALUE_FORMAT_BOOL;
            chr_to_write[i].value.b = true;
        }
        else if (cJSON_IsFalse(value_o))
        {
            chr_to_write[i].value.format = HK_VALUE_FORMAT_BOOL;
            chr_to_write[i].value.b = false;
        }
        else
        {
            cJSON_Delete(root);
            return HK_RET_FAIL;
        }
        chr_to_write[i].id.iid = iid;
        chr_to_write[i].id.aid = aid;
        // hk_console_printf("iid %d aid %d value %d", (int)chr_to_write[i].id.iid, aid, chr_to_write[i].value.i32);
    }
    *chr_to_write_size = chr_nr;
    cJSON_Delete(root);
    return HK_RET_SUCCESS;
}
