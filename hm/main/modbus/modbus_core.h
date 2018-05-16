/*
 * modbus_core.h
 *
 *  Created on: May 7, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_CORE_H_
#define HM_MAIN_MODBUS_MODBUS_CORE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "modbus.h"

typedef struct modbus_core_t modbus_core_t;


typedef enum
{
	MODBUS_RTU,
	MODBUS_ASCII,
	MODBUS_MASTER,
	MODBUS_SLAVE
}modbus_type_t;

modbus_core_t *modbus_core_create(bus_t *bus);

void modbus_core_task();
modbus_err_t modbus_core_transaction(modbus_core_t *core, const uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len);

#endif /* HM_MAIN_MODBUS_MODBUS_CORE_H_ */
