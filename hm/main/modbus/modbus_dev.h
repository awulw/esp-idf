/*
 * modbus_dev.h
 *
 *  Created on: May 14, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_DEV_H_
#define HM_MAIN_MODBUS_MODBUS_DEV_H_

// MODBUS COMMAND

#define INDENTITY 0x00
#define READ_N_REG 0x01
#define WRITE_N_REG 0x02
#define READ_REG 0x03
#define WRITE_REG 0x04
#define WRITE_N_REG_IN_EEPPROM 0x05
#define WRITE_AND 0x06
#define WRITE_OR 0x07
#define WRITE_EXOR 0x08

#define TOKEN 0x0C

#define FIRMWARE_INFO 0x20

#include "modbus.h"
typedef struct modbus_master_dev_t modbus_master_dev_t;
modbus_master_dev_t *modbus_master_new(bus_t *bus);
modbus_err_t modbus_dev_transaction(modbus_master_dev_t *master, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len);
void modbus_devs_poll(modbus_master_dev_t *master);
void modbus_devs_discovery(struct modbus_master_dev_t *master);
modbus_err_t modbus_dev_add(modbus_master_dev_t *master, uint8_t addr);
#endif /* HM_MAIN_MODBUS_MODBUS_DEV_H_ */
