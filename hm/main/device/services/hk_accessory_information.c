/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include "hk_accessory_information.h"

#include "../homekit/hk_byte_stream.h"

#include <string.h>

#define SERVICE_NAME "Accessory Information"
extern void hk_console_printf(const char *text, ...);

enum
{
    // required characteristics
    CHR_IDENTIFY = 0,      ///< Identify characteristic.
    CHR_MANUFACTURER,      ///< Manufacturer characteristic.
    CHR_MODEL,             ///< Model characteristic.
    CHR_NAME,              ///< Name characteristic.
    CHR_SERIAL_NUMBER,     ///< Serial Number characteristic.
    CHR_FIRMWARE_REVISION, ///< Firmware Revision characteristic.
    // optional characteristics
    CHR_HARDWARE_REVISION, ///< Hardware Revision characteristic.
    CHR_ACCESSORY_FLAGS,   ///< Accessory Flags characteristic.
    CHR_COUNT              ///< Accessory Information characteristics count.
};

hk_srv_base_t *hk_accessory_information_new(void)
{

    hk_srv_base_t *service = hk_srv_new(CHR_COUNT);
    if (service == NULL)
    {
        hk_console_printf("%s service construction error - out of resources", SERVICE_NAME);
        return NULL;
    }

    service->iid = hk_app_get_next_iid();
    service->metadata.uuid[2] = HK_SERVICE_UUID_ACCESSORY_INFORMATION >> 8;
    service->metadata.uuid[3] = HK_SERVICE_UUID_ACCESSORY_INFORMATION;
    service->metadata.primary = false;
    service->metadata.hidden = false;
    service->metadata.linked = NULL;
    service->metadata.linked_size = 0;

    for (size_t i = 0; i < service->chr_size; ++i)
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
    ((hk_app_chr_t *)service->chr[CHR_IDENTIFY])->base.metadata = &hk_chr_metadata_identify;
    ((hk_app_chr_t *)service->chr[CHR_IDENTIFY])->base.value.format = HK_VALUE_FORMAT_BOOL;
    ((hk_app_chr_t *)service->chr[CHR_IDENTIFY])->base.value.b = false;

    ((hk_app_chr_t *)service->chr[CHR_MANUFACTURER])->base.metadata = &hk_chr_metadata_manufacturer;
    ((hk_app_chr_t *)service->chr[CHR_MANUFACTURER])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_MANUFACTURER])->base.value.str = hk_strdup("Manufacturer");

    ((hk_app_chr_t *)service->chr[CHR_MODEL])->base.metadata = &hk_chr_metadata_model;
    ((hk_app_chr_t *)service->chr[CHR_MODEL])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_MODEL])->base.value.str = hk_strdup("Model");

    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.metadata = &hk_chr_metadata_name;
    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_NAME])->base.value.str = hk_strdup(SERVICE_NAME);

    ((hk_app_chr_t *)service->chr[CHR_SERIAL_NUMBER])->base.metadata = &hk_chr_metadata_serial_number;
    ((hk_app_chr_t *)service->chr[CHR_SERIAL_NUMBER])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_SERIAL_NUMBER])->base.value.str = hk_strdup("Serial Number");

    ((hk_app_chr_t *)service->chr[CHR_FIRMWARE_REVISION])->base.metadata = &hk_chr_metadata_firmware_revision;
    ((hk_app_chr_t *)service->chr[CHR_FIRMWARE_REVISION])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_FIRMWARE_REVISION])->base.value.str = hk_strdup("1.0");

    ((hk_app_chr_t *)service->chr[CHR_HARDWARE_REVISION])->base.metadata = &hk_chr_metadata_hardware_revision;
    ((hk_app_chr_t *)service->chr[CHR_HARDWARE_REVISION])->base.value.format = HK_VALUE_FORMAT_STR;
    ((hk_app_chr_t *)service->chr[CHR_HARDWARE_REVISION])->base.value.str = hk_strdup("1.0");

    ((hk_app_chr_t *)service->chr[CHR_ACCESSORY_FLAGS])->base.metadata = &hk_chr_metadata_accessory_flags;
    ((hk_app_chr_t *)service->chr[CHR_ACCESSORY_FLAGS])->base.value.format = HK_VALUE_FORMAT_U32;
    ((hk_app_chr_t *)service->chr[CHR_ACCESSORY_FLAGS])->base.value.u32 = 0;

    hk_console_printf("%s service created - IID: %d \n", SERVICE_NAME, service->iid);
    return service;
}
