/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

/** @file hk_lightbulb.h

    @brief Homekit lightbulb sample based on Apple-defined service.*/

#ifndef HK_LIGHTBULB_H
#define HK_LIGHTBULB_H

#include "../hap/hk_service.h"

/** Lightbulb service constructor.

    @param [in]primary Primary flag.
    @param [in]linked Linked services list.
    @param [in]linked_size Linked services list size.

    @return Service base type pointer to lightbulb service instance. */
hk_srv_base_t *hk_lightbulb_new(size_t chr_size);

#endif /* HK_LIGHTBULB_H */
