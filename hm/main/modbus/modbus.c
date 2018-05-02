/*
 * modbus.c
 *
 *  Created on: Apr 6, 2018
 *      Author: adam
 */

#include "modbus.h"
#include <stdlib.h>


#define MODBUS_FRAME_MAX_SIZE 128
#define RECIVE_CONTINUE_TIMEOUT 0



typedef struct
{
	modbus_frame_t frame;
	modbus_driver_t *driver;
	uint8_t recive_continue_timeout;

}modbus_dev_t;

extern modbus_driver_t ascii_driver;
extern modbus_driver_t rtu_driver;

handler_modbus_t *modbus_create(hm_serial_t *rs485, modbus_type_t type)
{
	handler_modbus_t *modbus = malloc(sizeof(handler_modbus_t));
	modbus->rs485 = rs485;
	if (!modbus) return NULL;


	modbus_dev_t *modbus_dev = malloc(sizeof(modbus_dev_t));
	if (!modbus_dev) return NULL;

	modbus_dev->frame.data = malloc(MODBUS_FRAME_MAX_SIZE);
	modbus_dev->frame.data_size = MODBUS_FRAME_MAX_SIZE;
	if (type == MODBUS_ASCII)
	{
		modbus_dev->driver = &ascii_driver;
		modbus_dev->recive_continue_timeout = 0;
	}else if (type == MODBUS_RTU) {
		modbus_dev->driver = &rtu_driver;
		modbus_dev->recive_continue_timeout = 0;
	}

	modbus->priv = modbus_dev;

	return modbus;
}



void modbus_task(void *parm)
{

}



modbus_err_t modbus_transaction(handler_modbus_t *handler, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout)
{
	hm_serial_t *rs485 = handler->rs485;
	modbus_dev_t *modbus_dev = handler->priv;

	if (modbus_dev->driver->data_to_frame(data_in, data_in_len, &modbus_dev->frame) != 0) return MODBUS_ERR_INPUT;
	serial_set_rts(rs485, 1);
	serial_send(rs485, modbus_dev->frame.data, modbus_dev->frame.data_len);
	serial_wait_send_done(rs485, timeout);
	serial_set_rts(rs485, 0);

	if (data_out)
	{
		serial_wait_for_data_recv(rs485, timeout);
		modbus_dev->frame.data_len = serial_recive(rs485, modbus_dev->frame.data, modbus_dev->frame.data_size, modbus_dev->recive_continue_timeout);

		if (modbus_dev->driver->frame_to_data(&modbus_dev->frame, data_out, data_out_len) != 0) return MODBUS_ERR_OUTPUT;
	}
	return MODBUS_OK;
}
