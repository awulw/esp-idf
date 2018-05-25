/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

/** @file hk_accessory_information.h

    @brief Homekit Accessory Information sample based on Apple-defined service.*/

#ifndef HK_ACCESSORY_INFORMATION_H
#define HK_ACCESSORY_INFORMATION_H

#include "../hap/hk_service.h"
#include "../hap/hk_app.h"

/** Accessory Information service constructor.

    @param [in]primary Primary flag.
    @param [in]linked Linked services list.
    @param [in]linked_size Linked services list size.

    @return Service base type pointer to Accessory Information service instance. */
hk_srv_base_t *hk_accessory_information_new(void);

#endif /* HK_ACCESSORY_INFORMATION_H */
