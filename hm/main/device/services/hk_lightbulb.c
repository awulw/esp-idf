/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include "hk_lightbulb.h"

#include "../hap/hk_app.h"

#include <string.h>
#include "../../utility/hk_byte_stream.h"

#define SERVICE_NAME "Lightbulb"
extern void hk_console_printf(const char *text, ...);

enum
{
    // required characteristics
    CHR_ON = 0, ///< On characteristic.
    // optional characteristics
    CHR_BRIGHTNESS, ///< Brightness characteristic.
    CHR_HUE,        ///< Hue characteristic.
    CHR_NAME,       ///< Name characteristic.
    CHR_SATURATION, ///< Saturation characteristic.
    CHR_COLOR_TEMP, ///< Color temperature characteristic.
    CHR_COUNT       ///< Lightbulb characteristics count.
};

/* TODO(SB): Add implementation of below functions if needed - required when cooperate with peripherals.
static hk_status_code_t hk_lightbulb_get_on(const hk_chr_base_t *chr, hk_value_t *const value)

static hk_status_code_t hk_lightbulb_set_on(const hk_chr_base_t *chr, hk_value_t value)

static hk_status_code_t hk_lightbulb_get_brightness(const hk_chr_base_t *chr, hk_value_t *const value)

static hk_status_code_t hk_lightbulb_set_brightness(const hk_chr_base_t *chr, hk_value_t value)

static hk_status_code_t hk_lightbulb_get_hue(const hk_chr_base_t *chr, hk_value_t *const value)

static hk_status_code_t hk_lightbulb_set_hue(const hk_chr_base_t *chr, hk_value_t value)

static hk_status_code_t hk_lightbulb_get_saturation(const hk_chr_base_t *chr, hk_value_t *const value)

static hk_status_code_t hk_lightbulb_set_saturation(const hk_chr_base_t *chr, hk_value_t value)

static hk_status_code_t hk_lightbulb_get_color_temp(const hk_chr_base_t *chr, hk_value_t *const value)

static hk_status_code_t hk_lightbulb_set_color_temp(const hk_chr_base_t *chr, hk_value_t value)
*/

hk_srv_base_t *hk_lightbulb_new(size_t chr_size)
{
    // Allocate memory
    hk_srv_base_t *service = hk_srv_new(chr_size);
    if (service == NULL)
    {
        hk_console_printf("%s service construction error - out of resources \n", SERVICE_NAME);
        return NULL;
    }

    // Initialize service
    service->iid = hk_app_get_next_iid();
    service->metadata.uuid[2] = HK_SERVICE_UUID_LIGHTBULB >> 8;
    service->metadata.uuid[3] = HK_SERVICE_UUID_LIGHTBULB;
    service->metadata.primary = false;
    service->metadata.hidden = false;

    // Initialize characteristics


    // TODO(SB): add persistent storage keys
//    ((hk_app_chr_t *)service->chr[CHR_ON])->base.metadata = &hk_chr_metadata_on;
//    ((hk_app_chr_t *)service->chr[CHR_ON])->base.value.format = HK_VALUE_FORMAT_BOOL;
//    ((hk_app_chr_t *)service->chr[CHR_ON])->base.value.b = false;
//
//    ((hk_app_chr_t *)service->chr[CHR_BRIGHTNESS])->base.metadata = &hk_chr_metadata_brightness;
//    ((hk_app_chr_t *)service->chr[CHR_BRIGHTNESS])->base.value.format = HK_VALUE_FORMAT_I32;
//    ((hk_app_chr_t *)service->chr[CHR_BRIGHTNESS])->base.value.i32 = 0;
//
//    ((hk_app_chr_t *)service->chr[CHR_HUE])->read = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_HUE])->write = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_HUE])->base.metadata = &hk_chr_metadata_hue;
//    ((hk_app_chr_t *)service->chr[CHR_HUE])->base.value.format = HK_VALUE_FORMAT_FLOAT;
//    ((hk_app_chr_t *)service->chr[CHR_HUE])->base.value.f = 0.0;
//
//    ((hk_app_chr_t *)service->chr[CHR_NAME])->read = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_NAME])->write = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.metadata = &hk_chr_metadata_name;
//    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.format = HK_VALUE_FORMAT_STR;
//    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.str = hk_strdup(SERVICE_NAME);
//
//    ((hk_app_chr_t *)service->chr[CHR_SATURATION])->read = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_SATURATION])->write = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_SATURATION])->base.metadata = &hk_chr_metadata_saturation;
//    ((hk_app_chr_t *)service->chr[CHR_SATURATION])->base.value.format = HK_VALUE_FORMAT_FLOAT;
//    ((hk_app_chr_t *)service->chr[CHR_SATURATION])->base.value.f = 0.0;
//
//    ((hk_app_chr_t *)service->chr[CHR_COLOR_TEMP])->read = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_COLOR_TEMP])->write = NULL;
//    ((hk_app_chr_t *)service->chr[CHR_COLOR_TEMP])->base.metadata = &hk_chr_metadata_color_temperature;
//    ((hk_app_chr_t *)service->chr[CHR_COLOR_TEMP])->base.value.format = HK_VALUE_FORMAT_U32;
//    ((hk_app_chr_t *)service->chr[CHR_COLOR_TEMP])->base.value.u32 = 50;

    hk_console_printf("%s service created - IID: %d \n", SERVICE_NAME, service->iid);
    return service;
}
