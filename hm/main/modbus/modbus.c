/*
 * modbus.c
 *
 *  Created on: Apr 6, 2018
 *      Author: adam
 */

#include "modbus.h"
#include <stdlib.h>

#define MODBUS_FRAME_MAX_SIZE 128

typedef struct
{
	uint8_t data[MODBUS_FRAME_MAX_SIZE];
	size_t len;
}modbus_frame_t;

typedef struct
{
	modbus_frame_t frame;
}modbus_t;

handler_modbus_t *modbus_create(hm_serial_t *rs485, modbus_type_t type)
{
	handler_modbus_t *modbus = malloc(sizeof(handler_modbus_t));
	modbus->rs485 = rs485;
	if (!modbus) return NULL;

	modbus->priv = malloc(sizeof(modbus_t));
	if (!modbus->priv) return NULL;
	return modbus;
}

static void data_to_frame(uint8_t  *data, uint8_t len, modbus_frame_t *frame)
		{

		}
static void frame_to_data(modbus_frame_t *frame, uint8_t  *data, uint8_t *len)
		{

		}

void modbus_task(void *parm)
{

}

static void modbus_wait_for_ivent()
{

}

void modbus_transaction(handler_modbus_t *handler, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout)
{
	hm_serial_t *rs485 = handler->rs485;
	modbus_t *modbus = handler->priv;
	uint8_t *data_to_send;
	uint8_t data_to_send_len;

	data_to_frame(data_in, data_in_len, &modbus->frame);

	//serial_send(rs485, modbus->frame.data, modbus->frame.len);
	serial_send(rs485, data_in, data_in_len);
	serial_wait_for_data_recv(rs485, timeout);
	//modbus->frame.len = serial_recive(rs485, modbus->frame.data, MODBUS_FRAME_MAX_SIZE, timeout);
	*data_out_len = serial_recive(rs485, data_out, MODBUS_FRAME_MAX_SIZE, 0);

	//frame_to_data(&modbus->frame, data_out, data_out_len);
}
