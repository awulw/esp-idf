/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

/** @file hk_switch.h

    @brief Homekit switch sample based on Apple-defined service.*/

#ifndef HK_SWITCH_H
#define HK_SWITCH_H

#include "../hap/hk_app.h"

/** Switch service constructor.

    @param [in]primary Primary flag.
    @param [in]linked Linked services list.
    @param [in]linked_size Linked services list size.

    @return Service base type pointer to switch service instance. */
hk_srv_base_t *hk_switch_new(bool primary, const hk_instance_id_t *linked, size_t linked_size);

#endif /* HK_SWITCH_H */
