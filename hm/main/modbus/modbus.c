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

hm_serial_t *serial_create(int uart_num, int baud_rate, const char pattern_chr, size_t buf_size, serial_driver_t *serial_driver)
{
	hm_serial_t *serial = calloc(1, sizeof(hm_serial_t));
	if (serial == NULL) return NULL;
	serial->driver = serial_driver;
	serial_driver->init(serial, uart_num, baud_rate, pattern_chr, buf_size);
	return serial;
}

handler_modbus_t *modbus_create(hm_serial_t *rs485, modbus_driver_t *modbus_driver)
{
	handler_modbus_t *modbus = malloc(sizeof(handler_modbus_t));
	modbus->rs485 = rs485;
	if (!modbus) return NULL;


	modbus_dev_t *modbus_dev = malloc(sizeof(modbus_dev_t));
	if (!modbus_dev) return NULL;

	modbus_dev->frame.data = malloc(MODBUS_FRAME_MAX_SIZE);
	modbus_dev->frame.data_size = MODBUS_FRAME_MAX_SIZE;
	modbus_dev->driver = modbus_driver;

	modbus->priv = modbus_dev;
	return modbus;
}



modbus_err_t modbus_transaction(handler_modbus_t *handler, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout)
{
	hm_serial_t *rs485 = handler->rs485;
	modbus_dev_t *modbus_dev = handler->priv;
	serial_driver_t *serial = rs485->driver;
	modbus_err_t ret;
	*data_out_len = 0;

	ret = modbus_dev->driver->data_to_frame(data_in, data_in_len, &modbus_dev->frame);
	if (ret) return ret;

	serial->flush(rs485);
	serial->set_rts(rs485, 1);
	serial->send(rs485, modbus_dev->frame.data, modbus_dev->frame.data_len);
	serial->wait_send_done(rs485, timeout);
	serial->set_rts(rs485, 0);

	if (data_out)
	{
		ret = serial->wait_for_data_recv(rs485, timeout);
		if (ret) return ret;
		modbus_dev->frame.data_len = serial->recive(rs485, modbus_dev->frame.data, modbus_dev->frame.data_size, 0);

		ret = modbus_dev->driver->frame_to_data(&modbus_dev->frame, data_out, data_out_len);
		if (ret) return ret;
	}
	return MODBUS_OK;
}
