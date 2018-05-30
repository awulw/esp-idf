/*
 * device.c
 *
 *  Created on: May 22, 2018
 *      Author: adam
 */
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "device.h"


#include "services/hk_lightbulb.h"

extern void hk_console_printf(const char *text, ...);

struct device_hub_t
{
	hk_acc_base_t *acc;
};

struct device_t
{
	hk_srv_base_t *srv;
	void *device_context;
};

static hk_hap_status_t on_read_b(void *chr_ptr)
{
	int ret;
    hk_console_printf("ON READ CALBACK\n\n");
    hk_app_chr_t *chr = chr_ptr;
    io_driver_t *drv = chr->user_driver;
    uint8_t value[5];

    ret = drv->read(chr->user_device, chr->user_driver_context, value, 1);
    if (ret == 0)
    {
    	chr->base.value.b = value[0];
    	return HK_RET_SUCCESS;
    }
    return HK_RET_FAIL;
}
hk_acc_base_t *device_get_acc(device_hub_t *hub)
{
	return hub->acc;
}

static hk_hap_status_t on_write_b(void *chr_ptr)
{
	int ret;
    hk_app_chr_t *chr = chr_ptr;
    io_driver_t *drv = chr->user_driver;
    uint8_t value = chr->base.value.b;
    hk_console_printf("ON write CALBACK value=%d\n\n", chr->base.value.i32);
    //ret = drv->write(chr->user_device, chr->user_driver_context, &value, 1);
    //if (ret == 0)
    {
    	return HK_RET_SUCCESS;
    }
    return HK_RET_FAIL;
}

device_hub_t *device_hub_create()
{
	device_hub_t *hub = calloc(1, sizeof(device_hub_t));
	hub->acc = hk_app_create_acc(1);
	return hub;
}

device_t *device_create(device_hub_t *hub, void *dev_context, dev_type_t type)
{
	if (hub == NULL)
		return NULL;
	device_t *device = calloc(1, sizeof(device_t));
	//switch (type)
	{
	//case DEV_LIGHT_BULB:
		device->srv = hk_lightbulb_new(6);
	//	break;
	}
	device->device_context = dev_context;
	hk_app_add_srv(hub->acc, device->srv);

	//...


	//device_add(hub, device);
	return device;
}

void device_add_chr_b(device_t *dev, chr_type_t type, void *driver_context, io_driver_t *io_driver)
{
	if (dev == NULL)
		return;



	//chr onoff
	hk_app_chr_t *chr = hk_app_chr_new_b(on_read_b, on_write_b);
	chr->user_driver_context = driver_context;
	chr->user_driver = io_driver;
	chr->user_device = dev->device_context;





	hk_app_add_chr(dev->srv, chr);


	return;
}
