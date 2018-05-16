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
#include "modbus_rutine.h"
#include "modbus_dev.h"
#include <string.h>


extern modbus_driver_t rtu_driver;
extern modbus_driver_t ascii_driver;

struct modbus_core_t
{
	QueueHandle_t queue;
	bus_t *bus;
	modbus_master_dev_t *master;
};

typedef struct
{
	int msg_id;
	int status;
	uint8_t *data;
	uint8_t data_len;
	uint8_t data_size;
	QueueHandle_t return_queue;
}msg_t;

modbus_core_t *modbus_core_create(bus_t *bus)
{
	modbus_core_t *core = calloc(1, sizeof(modbus_core_t));

	core->bus = bus;
	core->master = modbus_master_new(bus);

	modbus_master_dev_t *master = core->master;
	modbus_dev_add(master, 1);
	modbus_dev_add(master, 2);

	core->queue = xQueueCreate(20, sizeof(msg_t));
	return core;
}



void modbus_core_task(void *parm)
{
	modbus_core_t *core = parm;
	msg_t msg;
	uint8_t buf_len;

	while (1)
	{
		if (xQueueReceive(core->queue, &msg, 10) == pdTRUE)
		{
			buf_len = msg.data_size;
			msg.status = modbus_dev_transaction(core->master, msg.data, msg.data_len, msg.data, &buf_len);
			msg.data_len = buf_len;
			//bus_transaction(core->bus , msg.data, msg.data_len, msg.data, &(msg.data_len), 2000);
			if (msg.return_queue) xQueueSendToFront(msg.return_queue, (void *)&msg, 0);
		}
		modbus_rutine(core->master, 50);
		//modbus_core_proccess();
	}
}

void modbus_core_send_msg(modbus_core_t *core, msg_t *msg)
{
	xQueueSendToFront(core->queue, (void *)msg, 0);
}
int modbus_core_recv_msg(modbus_core_t *core, msg_t *msg)
{
	return (xQueueReceive(msg->return_queue, msg, 10000));
}

modbus_err_t modbus_core_transaction(modbus_core_t *core, const uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len)
{
	uint8_t buf[128];
	msg_t msg;
	memcpy(buf, data_in, data_in_len);
	msg.data_len = data_in_len;
	msg.data = buf;
	msg.data_size= sizeof(buf);
	msg.return_queue = xQueueCreate(1, sizeof(msg_t));
	*data_out_len = 0;
	modbus_core_send_msg(core, &msg);
	if (modbus_core_recv_msg(core, &msg) != pdTRUE) goto exit;
	memcpy(data_out, buf, msg.data_len);
	*data_out_len = msg.data_len;

	exit:
		vQueueDelete(msg.return_queue);
		return msg.status;
}


