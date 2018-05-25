/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include <string.h>

#include "hk_service.h"
#include "hk_accessory.h"

hk_srv_base_t *hk_srv_find_by_iid(const hk_acc_base_t *accessory, hk_instance_id_t iid)
{
    for (size_t i = 0; i < accessory->srv_size; ++i)
    {
        if (accessory->srv[i]->iid == iid)
        {
            return accessory->srv[i];
        }
    }
    return NULL;
}

bool hk_srv_copy_metadata(hk_srv_metadata_t *dst, const hk_srv_metadata_t *src)
{
    memcpy(dst, src, sizeof(hk_srv_metadata_t));
    if (src->linked != NULL)
    {
        dst->linked = calloc(sizeof(hk_instance_id_t), src->linked_size);
        if (dst->linked == NULL)
        {
            return false;
        }
        memcpy(dst->linked, src->linked, sizeof(hk_instance_id_t) * dst->linked_size);
    }
    return true;
}

hk_srv_base_t *hk_srv_new(size_t chr_size)
{
    if (chr_size > HK_MAX_CHARACTERISTICS)
    {
        return NULL;
    }
    hk_srv_base_t *service = calloc(1, sizeof(hk_srv_base_t));
    if (service == NULL)
    {
        return NULL;
    }
    service->chr = calloc(chr_size, sizeof(hk_chr_base_t *));
    if (service->chr == NULL)
    {
        free(service);
        return NULL;
    }
    service->chr_size = 0;
    service->chr_cap = chr_size;

    return service;
}

void hk_srv_delete(hk_srv_base_t *service)
{
    if (service == NULL)
    {
        return;
    }
    if (service->chr != NULL)
    {
        for (size_t i = 0; i < service->chr_size; ++i)
        {
            if (service->chr[i] != NULL)
            {
                hk_chr_delete(service->chr[i]);
            }
        }
        free(service->chr);
    }
    free(service);
}
