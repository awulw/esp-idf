/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

/** @file hk_service.h

    @brief Homekit service API.*/

#ifndef HOMEKIT_HK_SERVICE_H
#define HOMEKIT_HK_SERVICE_H

#include "../hk_api.h"
#include "hk_characteristic.h"

#define HK_MAX_SERVICES 100

// forward declaration
typedef struct hk_acc_base hk_acc_base_t;

typedef struct
{
    uint8_t uuid[16];         ///< Service type UUID.
    bool primary;             ///< Primary property flag.
    bool hidden;              ///< Hidden property flag.
    hk_instance_id_t *linked; ///< Linked instance IDs array.
    size_t linked_size;       ///< Size of linked instance IDs array.
} hk_srv_metadata_t;

/** Service base structure. */
typedef struct
{
    hk_instance_id_t iid;       ///< Service instance ID.
    hk_srv_metadata_t metadata; ///< Service metadata.

    hk_chr_base_t **chr; ///< Service's characteristics.
    size_t chr_size;     ///< Service's characteristics size.
    size_t chr_cap;     ///< Service's characteristics size.
} hk_srv_base_t;

/** Finds pointer to service by instance ID in specified accessory.

    @param [in] accessory Accessory instance pointer.
    @param [in] iid Service instance ID.

    @return Pointer of service instance with specified instance ID. */
HK_NOT_NULL_ARGS(1)
hk_srv_base_t *hk_srv_find_by_iid(const hk_acc_base_t *accessory, hk_instance_id_t iid);

/** Copies service's metadata.

    @param dst Pointer to destination metadata.
    @param src Pointer to source metadata.

    @return True if copying ok. False otherwise. */
HK_NOT_NULL_ARGS(1, 2)
bool hk_srv_copy_metadata(hk_srv_metadata_t *dst, const hk_srv_metadata_t *src);

/** Allocates memory for service.

    @param [in] chr_size Size of the service as characteristics number.

    @return Memory pointer of type \ref hk_srv_base_t. */
hk_srv_base_t *hk_srv_new(size_t chr_size);

/** Frees memory of service.

    @param [in] service Pointer to service.

    @return None. */
void hk_srv_delete(hk_srv_base_t *service);

/** Short service UUIDs defined in HAP R9 spec. */
enum
{
    HK_SERVICE_UUID_ACCESSORY_INFORMATION = 0x3E,
    HK_SERVICE_UUID_FAN = 0x40,
    HK_SERVICE_UUID_GARAGE_DOOR_OPENER = 0x41,
    HK_SERVICE_UUID_LIGHTBULB = 0x43,
    HK_SERVICE_UUID_LOCK_MANAGEMENT = 0x44,
    HK_SERVICE_UUID_LOCK_MECHANISM = 0x45,
    HK_SERVICE_UUID_OUTLET = 0x47,
    HK_SERVICE_UUID_SWITCH = 0x49,
    HK_SERVICE_UUID_THERMOSTAT = 0x4A,
    HK_SERVICE_UUID_AIR_QUALITY_SENSOR = 0x8D,
    HK_SERVICE_UUID_SECURITY_SYSTEM = 0x7E,
    HK_SERVICE_UUID_CARBON_MONOXIDE_SENSOR = 0x7F,
    HK_SERVICE_UUID_CONTACT_SENSOR = 0x80,
    HK_SERVICE_UUID_DOOR = 0x81,
    HK_SERVICE_UUID_HUMIDITY_SENSOR = 0x82,
    HK_SERVICE_UUID_LEAK_SENSOR = 0x83,
    HK_SERVICE_UUID_LIGHT_SENSOR = 0x84,
    HK_SERVICE_UUID_MOTION_SENSOR = 0x85,
    HK_SERVICE_UUID_OCCUPANCY_SENSOR = 0x86,
    HK_SERVICE_UUID_SMOKE_SENSOR = 0x87,
    HK_SERVICE_UUID_STATELESS_PROGRAMMABLE_SWITCH = 0x89,
    HK_SERVICE_UUID_TEMPERATURE_SENSOR = 0x8A,
    HK_SERVICE_UUID_WINDOW = 0x8B,
    HK_SERVICE_UUID_WINDOW_COVERING = 0x8C,
    HK_SERVICE_UUID_BATTERY_SERVICE = 0x96,
    HK_SERVICE_UUID_CARBON_DIOXIDE_SENSOR = 0x97,
    HK_SERVICE_UUID_CAMERA_RTP_STREAM_MANAGEMENT = 0x110,
    HK_SERVICE_UUID_MICROPHONE = 0x112,
    HK_SERVICE_UUID_SPEAKER = 0x113,
    HK_SERVICE_UUID_DOORBELL = 0x121,
    HK_SERVICE_UUID_FAN_V2 = 0xB7,
    HK_SERVICE_UUID_SLAT = 0xB9,
    HK_SERVICE_UUID_FILTER_MAINTENANCE = 0xBA,
    HK_SERVICE_UUID_AIR_PURIFIER = 0xBB,
    HK_SERVICE_UUID_SERVICE_LABEL = 0xCC
};

#endif /* HOMEKIT_HK_SERVICE_H */
