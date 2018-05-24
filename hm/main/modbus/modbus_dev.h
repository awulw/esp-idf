/*
 * modbus_dev.h
 *
 *  Created on: May 14, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_DEV_H_
#define HM_MAIN_MODBUS_MODBUS_DEV_H_

// MODBUS COMMAND

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "modbus.h"
#include "../device/device.h"

typedef struct modbus_master_dev_t modbus_master_dev_t;
typedef struct modbus_dev_t modbus_dev_t;

modbus_master_dev_t *modbus_master_new(bus_t *bus, void *hub);
modbus_err_t modbus_master_transaction(modbus_master_dev_t *master, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len);
void modbus_devs_poll(modbus_master_dev_t *master);
void modbus_master_set_core(modbus_master_dev_t *master, void *core);
void *modbus_dev_get_core(modbus_dev_t *dev);
uint8_t modbus_dev_get_addr(modbus_dev_t *dev);
uint8_t *modbus_dev_get_reg(modbus_dev_t *dev);
void modbus_devs_poll_broadcast(struct modbus_master_dev_t *master);
void modbus_devs_discovery(struct modbus_master_dev_t *master);
modbus_err_t modbus_dev_add(modbus_master_dev_t *master, uint8_t addr);

void modbus_dev_add_reg_notification(modbus_dev_t *dev, uint8_t reg_nr, uint8_t reg_mask, void *context, void (*cb)(void *context));
#endif /* HM_MAIN_MODBUS_MODBUS_DEV_H_ */
