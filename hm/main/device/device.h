/*
 * device.h
 *
 *  Created on: May 22, 2018
 *      Author: adam
 */

#ifndef HM_DEVICE_DEVICE_H_
#define HM_DEVICE_DEVICE_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hap/hk_app.h"

typedef enum
{
	CHR_ONOFF,
	CHR_DIN,
	CHR_DOUT
}chr_type_t;

typedef enum
{
	DEV_LIGHT_BULB,
	DEV_LIGHT_LED,
	DEV_LIGHT_ANALOG,
}dev_type_t;


typedef struct
{
	int (*read)(void *dev_context, void *driver_context, uint8_t *buf, uint8_t buf_len);
	int (*write)(void *dev_context, void *driver_context, uint8_t *buf, uint8_t buf_len);
}io_driver_t;

typedef struct device_hub_t device_hub_t;
typedef struct device_t device_t;
typedef struct device_srv_t device_srv_t;

void (*device_notification_cb)(void *device);

device_hub_t *device_hub_create();

device_t *device_create(device_hub_t *hub, void *dev_context);
device_srv_t *device_srv_create(device_t *device, dev_type_t type);
void device_chr_create_b(device_srv_t *dev_srv, chr_type_t type, void *driver_context, io_driver_t *io_driver);



#endif /* HM_DEVICE_DEVICE_H_ */

