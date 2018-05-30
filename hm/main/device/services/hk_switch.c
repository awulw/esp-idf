/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include "hk_switch.h"

#include "../../utility/hk_byte_stream.h"
extern void hk_console_printf(const char *text, ...);

#include <string.h>

#define SERVICE_NAME "Switch"

enum
{
    // required characteristics
    CHR_ON = 0, ///< On characteristic.
    // optional characteristics
    CHR_NAME, ///< Name characteristic.
    CHR_COUNT ///< Switch characteristics count.
};

hk_srv_base_t *hk_switch_new(bool primary, const hk_instance_id_t *linked, size_t linked_size)
{
    if (linked_size > HK_MAX_SERVICES)
    {
    	hk_console_printf("%s service error - linked services size too big: %d/%d", SERVICE_NAME, linked_size, HK_MAX_SERVICES);
        return NULL;
    }

    hk_srv_base_t *service = hk_srv_new(CHR_COUNT);
    if (service == NULL)
    {
    	hk_console_printf("%s service construction error - out of resources", SERVICE_NAME);
        return NULL;
    }

    service->iid = hk_app_get_next_iid();
    service->metadata.uuid[2] = HK_SERVICE_UUID_SWITCH >> 8;
    service->metadata.uuid[3] = HK_SERVICE_UUID_SWITCH;
    service->metadata.primary = primary;
    service->metadata.hidden = false;
    if (linked_size > 0)
    {
        service->metadata.linked = malloc(sizeof(hk_instance_id_t) * linked_size);
        if (service->metadata.linked == NULL)
        {
        	hk_console_printf("%s service construction error - out of resources", SERVICE_NAME);
            hk_app_srv_delete(service);
            return NULL;
        }
        memcpy(service->metadata.linked, linked, linked_size);
        service->metadata.linked_size = linked_size;
    }

    for (size_t i = 0; i < service->chr_cap; ++i)
    {
        service->chr[i] = (hk_chr_base_t *)hk_app_chr_new();
        if (service->chr[i] == NULL)
        {
        	hk_console_printf("%s service construction error - out of resources", SERVICE_NAME);
            hk_app_srv_delete(service);
            return NULL;
        }
        service->chr[i]->iid = hk_app_get_next_iid();
    }

    // TODO(MD): add persistent storage keys
    ((hk_app_chr_t *)service->chr[CHR_ON])->base.metadata = &hk_chr_metadata_on;
    ((hk_app_chr_t *)service->chr[CHR_ON])->base.value.format = HK_VALUE_FORMAT_BOOL;
    ((hk_app_chr_t *)service->chr[CHR_ON])->base.value.b = false;

    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.metadata = &hk_chr_metadata_name;
    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.str = hk_strdup(SERVICE_NAME);

    hk_console_printf("%s service created - IID: %l", SERVICE_NAME, service->iid);
    return service;
}
