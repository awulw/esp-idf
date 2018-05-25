/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#ifndef HOMEKIT_HK_ACCESSORY_H
#define HOMEKIT_HK_ACCESSORY_H

#include "../hk_api.h"
#include "hk_service.h"

#define HK_MAX_ACCESSORIES 100

/** Accessory base type */
typedef struct hk_acc_base
{
    hk_accessory_id_t aid;
    hk_srv_base_t *srv[HK_MAX_SERVICES];
    size_t srv_size;
} hk_acc_base_t;

#endif /* HOMEKIT_HK_ACCESSORY_H */
