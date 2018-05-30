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
	void *acc_map;
	int (*registration_cb)(void *, void *);
};

struct device_t
{
	hk_acc_base_t *acc;
	void *device_context;
};

struct device_srv_t
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
    uint8_t value;

    ret = drv->read(chr->user_device, chr->user_driver_context, &value, 1);
    if (ret == 0)
    {
    	chr->base.value.b = value;
    	return HK_RET_SUCCESS;
    }
    return HK_RET_FAIL;
}



static hk_hap_status_t on_write_b(void *chr_ptr)
{
	int ret;
    hk_app_chr_t *chr = chr_ptr;
    io_driver_t *drv = chr->user_driver;
    uint8_t value = chr->base.value.b;
    hk_console_printf("ON write CALBACK value=%d\n\n", chr->base.value.i32);
    ret = drv->write(chr->user_device, chr->user_driver_context, &value, 1);
    if (ret == 0)
    {
    	return HK_RET_SUCCESS;
    }
    return HK_RET_FAIL;
}

device_hub_t *device_hub_create(void *acc_map, void *registration_cb)
{
	device_hub_t *hub = calloc(1, sizeof(device_hub_t));
	hub->registration_cb = registration_cb;
	hub->acc_map = acc_map;
	return hub;
}



device_t *device_create(device_hub_t *hub, void *dev_context)
{
	if (hub == NULL)
		return NULL;
	device_t *device = calloc(1, sizeof(device_t));
	device->acc = hk_app_create_acc();
	device->device_context = dev_context;
	hub->registration_cb(hub->acc_map, device->acc);
	return device;
}

device_srv_t *device_srv_create(device_t *device, dev_type_t type)
{
	if (device == NULL)
			return NULL;
		device_srv_t *dev_srv = calloc(1, sizeof(device_srv_t));
		dev_srv->srv = hk_lightbulb_new(6);
		dev_srv->device_context = device->device_context;
		hk_app_add_srv(device->acc, dev_srv->srv);
		return dev_srv;
}

void device_chr_create_b(device_srv_t *dev_srv, chr_type_t type, void *driver_context, io_driver_t *io_driver)
{
	if (dev_srv == NULL)
		return;
	//chr onoff
	hk_app_chr_t *chr = hk_app_chr_new_b(on_read_b, on_write_b);
	chr->user_driver_context = driver_context;
	chr->user_driver = io_driver;
	chr->user_device = dev_srv->device_context;
	hk_app_add_chr(dev_srv->srv, chr);
	return;
}
