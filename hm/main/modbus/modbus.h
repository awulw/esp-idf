/*
 * modbus.h
 *
 *  Created on: Apr 6, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_H_
#define HM_MAIN_MODBUS_MODBUS_H_

#include "serial.h"

typedef enum
{
	MODBUS_OK,
	MODBUS_ERR_INPUT,
	MODBUS_ERR_OUTPUT
}modbus_err_t;


typedef enum
{
	MODBUS_ASCII,
	MODBUS_RTU
}modbus_type_t;

typedef struct
{
	hm_serial_t *rs485;
	modbus_type_t type;
	void *priv;

}handler_modbus_t;

typedef struct
{
	uint8_t *data;
	size_t data_len;
	size_t data_size;
}modbus_frame_t;

typedef struct
{
	uint8_t (*frame_to_data)(modbus_frame_t *frame ,uint8_t *data, uint8_t *date_len);
	uint8_t (*data_to_frame)(uint8_t *data, uint8_t date_len, modbus_frame_t *frame);
}modbus_driver_t;


handler_modbus_t *modbus_create(hm_serial_t *rs485, modbus_type_t type);
void modbus_task(void *parm);
modbus_err_t modbus_transaction(handler_modbus_t *handler, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout);

#endif /* HM_MAIN_MODBUS_MODBUS_H_ */

