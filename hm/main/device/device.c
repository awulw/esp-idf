/*
 * device.c
 *
 *  Created on: May 22, 2018
 *      Author: adam
 */

#include "device.h"

device_hub_t *device_hub_create()
{
	return NULL;
}

device_t *device_create(device_hub_t *hub, void *dev_context, dev_type_t type)
{
	return NULL;
}
void device_add_chr(device_t *dev, chr_type_t type, void *driver_context, io_driver_t *io_driver)
{
	return;
}
