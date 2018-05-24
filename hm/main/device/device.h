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

void (*device_notification_cb)(void *device);

device_hub_t *device_hub_create();

device_t *device_create(device_hub_t *hub, void *dev_context, dev_type_t type);
void device_add_chr(device_t *dev, chr_type_t type, void *driver_context, io_driver_t *io_driver);



#endif /* HM_DEVICE_DEVICE_H_ */

