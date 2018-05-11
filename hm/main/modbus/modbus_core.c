/*
 * modbus_core.c
 *
 *  Created on: May 7, 2018
 *      Author: adam
 */

#include "modbus_core.h"
#include "serial.h"
#include "modbus.h"
#include "driver_ascii.h"
#include "esp_log.h"
#include "modbus_conf.h"

extern modbus_driver_t rtu_driver;
extern modbus_driver_t ascii_driver;

struct modbus_core_t
{
	QueueHandle_t queue;
	modbus_t *bus;
	modbus_master_dev_t *master;
};

typedef struct
{
	int msg_id;
	int status;
	uint8_t *data;
	uint8_t data_len;
	QueueHandle_t return_queue;
}msg_t;

modbus_core_t *modbus_core_create(modbus_type_t type)
{
	modbus_core_t *core = calloc(1, sizeof(modbus_core_t));
	hm_serial_t *rs485;
	modbus_t *bus;

	if (type == MODBUS_ASCII)
	{
		rs485 = serial_create(0, 115200, "\r", 1024);
		bus = modbus_create(rs485, serial_get_driver(rs485), &ascii_driver);
	}
	//else if (type == MODBUS_RTU)
	else
	{
		rs485= serial_create(0, 115200, NULL, 1024);
		bus= modbus_create(rs485, serial_get_driver(rs485), &rtu_driver);

	}
	core->bus = bus;
	core->master = modbus_master_create(bus);
	core->queue = xQueueCreate(10, sizeof(msg_t));

	return core;
}

void modbus_core_task(void *parm)
{
	modbus_core_t *core = parm;
	msg_t msg;


	while (1)
	{
		if (xQueueReceive(core->queue, &msg, 100000) == pdTRUE)
		{
			msg.status = modbus_transaction(core->bus , msg.data, msg.data_len, msg.data, &(msg.data_len), 2000);
			if (msg.return_queue) xQueueSendToFront(msg.return_queue, (void *)&msg, 0);
		}
	}
}

void modbus_core_send_msg(modbus_core_t *core, msg_t *msg)
{
	xQueueSendToFront(core->queue, (void *)msg, 0);
}
int modbus_core_recv_msg(modbus_core_t *core, msg_t *msg)
{
	return (xQueueReceive(msg->return_queue, msg, 100000));
}

modbus_err_t modbus_core_transaction(modbus_core_t *core, uint8_t *data_in_out, uint8_t data_in_len, uint8_t *data_out_len)
{
	msg_t msg;
	msg.data = data_in_out;
	msg.data_len = data_in_len;
	msg.return_queue = xQueueCreate(1, sizeof(msg_t));
	*data_out_len = 0;
	modbus_core_send_msg(core, &msg);
	if (modbus_core_recv_msg(core, &msg) != pdTRUE) goto exit;
	*data_out_len = msg.data_len;

	exit:
		vQueueDelete(msg.return_queue);
		return msg.status;
}


