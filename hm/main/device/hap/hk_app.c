/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include <stdio.h>

#include "../hk_api.h"
#include "hk_app.h"
#include "../services/hk_lightbulb.h"
#include "../services/hk_accessory_information.h"


extern void hk_console_printf(const char *text, ...);

/*-------------------------------------------------------------------------
 * Function Definitions
 *-----------------------------------------------------------------------*/

hk_instance_id_t hk_app_get_next_iid(void)
{
    static hk_instance_id_t s_iid = 1;
    return s_iid++;
}

hk_app_chr_t *hk_app_chr_new(void)
{
    return calloc(1, sizeof(hk_app_chr_t));
}

void hk_app_chr_delete(hk_app_chr_t *chr)
{
    if (chr == NULL)
    {
        return;
    }
    // TODO(SB): Add free for persistent storage keys.
    hk_chr_delete(&chr->base);
}

void hk_app_srv_delete(hk_srv_base_t *srv)
{
    if (srv == NULL)
    {
        return;
    }
    for (size_t i = 0; i < srv->chr_size; ++i)
    {
        hk_app_chr_delete((hk_app_chr_t *)srv->chr[i]);
        srv->chr[i] = NULL;
    }
    hk_srv_delete(srv);
}

hk_acc_base_t *hk_app_create_acc(hk_accessory_id_t aid)
{
    hk_acc_base_t *acc = calloc(1, sizeof(hk_acc_base_t));
    if (acc == NULL)
        return NULL;
    acc->aid = aid;
    acc->srv[0] = hk_accessory_information_new();
    acc->srv_size = 1;
    return acc;
}

hk_ret_t hk_app_add_srv(hk_acc_base_t *acc, hk_srv_base_t *srv)
{
    if (acc->srv_size >= HK_MAX_SERVICES)
    {
        return HK_RET_RESOURCES_EXHAUSTED;
    }
    acc->srv[acc->srv_size++] = srv;
    return HK_RET_SUCCESS;
}

hk_ret_t hk_app_add_chr(hk_srv_base_t *srv, hk_app_chr_t *chr)
{
	if (srv->chr_size >= srv->chr_cap)
	{
		return HK_RET_RESOURCES_EXHAUSTED;
	}
	srv->chr[srv->chr_size++] = chr;
	return HK_RET_SUCCESS;
}

hk_hap_status_t on_read(void *chr_ptr)
{
    hk_console_printf("ON READ CALBACK\n\n");
    hk_chr_base_t *chr = chr_ptr;

    return 0;
}
hk_hap_status_t on_write(void *chr_ptr)
{
    hk_chr_base_t *chr = chr_ptr;
    hk_console_printf("ON write %d CALBACK\n\n", chr->value.i32);
    return 0;
}



/**
   @brief This function is used to initialize the Homekit module.

   This function must be called before any other Homekit functions.

   @return
    - true if Homekit was initialized successfully.
    - false if initialization failed.
*/
//static bool homekit_initialize(hk_dispatcher_acc_map_t *acc_map)
//{
//    const hk_configuration_t hk_configuration = {.setup_code = "097-26-810",
//                                                 .interface_id = 0,
//                                                 .friendly_name = "HK-PERUN",
//                                                 .manufacturer = "Corporation",
//                                                 .model = "Model",
//                                                 .wac_device_id = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
//
//    };
//
//    // if (!hk_core_init(&hk_configuration))
//    {
//        if (hk_mdns_start(&hk_configuration) != HK_RET_SUCCESS)
//        {
//            hk_console_printf("MDNS start error\n");
//        }
  //      hk_acc_base_t *acc = hk_app_create_acc(1);
//        hk_srv_base_t *light_bulb = hk_lightbulb_new(true, NULL, 0);
//        hk_srv_base_t *light_bulb2 = hk_lightbulb_new(false, NULL, 0);
//        hk_srv_base_t *light_bulb3 = hk_lightbulb_new(false, NULL, 0);
//
//        light_bulb->chr[0]->on_read = on_read;
//        light_bulb->chr[0]->on_write = on_write;
//
//        hk_app_add_srv(acc, light_bulb);
//        hk_app_add_srv(acc, light_bulb2);
//        hk_app_add_srv(acc, light_bulb3);
//
//        hk_acc_base_t *acc2 = hk_app_create_acc(2);
//        hk_app_add_srv(acc2, light_bulb);
//        hk_dispatcher_register_accessory(acc_map, acc);
//        hk_dispatcher_register_accessory(acc_map, acc2);
//        return HK_RET_SUCCESS;
//    }
//    return HK_RET_FAIL;
//}
//
///**
//   @brief This function represents the main thread of execution.
//
//   It will finish initialization of the sample and then function as a
//   receive thread for the console.
//*/
//void *homekit_app_main_task(void *arg)
//{
//    hk_dispatcher_acc_map_t *acc_map = (hk_dispatcher_acc_map_t *)arg;
//
//    /* Initiailze the CLI. */
//    if (homekit_initialize(acc_map) == HK_RET_SUCCESS)
//    {
//        while (true)
//        {
//            usleep(1000);
//        }
//    }
//    else
//    {
//    }
//}
