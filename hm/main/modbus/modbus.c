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


struct bus_t
{
	modbus_frame_t frame;
	modbus_driver_t *modbus_driver;
	serial_driver_t *rs485_driver;
	uint8_t recive_continue_timeout;
	void *rs485_context;
};


bus_t *bus_create(void *rs485_context, serial_driver_t *rs485_driver, modbus_driver_t *modbus_driver)
{
	bus_t *modbus = malloc(sizeof(bus_t));
	if (!modbus) return NULL;

	modbus->rs485_context = rs485_context;
	modbus->frame.data = malloc(MODBUS_FRAME_MAX_SIZE);
	modbus->frame.data_size = MODBUS_FRAME_MAX_SIZE;
	modbus->modbus_driver = modbus_driver;
	modbus->rs485_driver = rs485_driver;
	return modbus;
}

modbus_err_t bus_send(bus_t *modbus_dev, const uint8_t *data_in, uint8_t data_in_len)
{
	serial_driver_t *rs485_driver = modbus_dev->rs485_driver;
	void *rs485 = modbus_dev->rs485_context;
	modbus_err_t ret;

	ret = modbus_dev->modbus_driver->data_to_frame(data_in, data_in_len, &modbus_dev->frame);
	if (ret) return ret;

	rs485_driver->flush(rs485);
	rs485_driver->set_rts(rs485, 1);
	rs485_driver->send(rs485, modbus_dev->frame.data, modbus_dev->frame.data_len);
	rs485_driver->wait_send_done(rs485, 100);
	rs485_driver->set_rts(rs485, 0);
	return MODBUS_OK;
}

modbus_err_t bus_recv(bus_t *modbus_dev, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout)
{
	serial_driver_t *rs485_driver = modbus_dev->rs485_driver;
	void *rs485 = modbus_dev->rs485_context;
	modbus_err_t ret;

	ret = rs485_driver->wait_for_data_recv(rs485, timeout);
	if (ret) return ret;
	modbus_dev->frame.data_len = rs485_driver->recive(rs485, modbus_dev->frame.data, modbus_dev->frame.data_size);
	ret = modbus_dev->modbus_driver->frame_to_data(&modbus_dev->frame, data_out, data_out_len);
	return ret;
}

modbus_err_t bus_transaction(bus_t *modbus_dev, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout)
{
	modbus_err_t ret;

	ret = bus_send(modbus_dev, data_in, data_in_len);
	if (ret) return ret;

	if (data_out)
	{
		ret = bus_recv(modbus_dev, data_out, data_out_len, timeout);
	}
	return ret;
}
