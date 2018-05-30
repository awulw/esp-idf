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
#include "../device/device.h"
#include <string.h>

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

modbus_core_t *modbus_core_create(bus_t *bus, uint8_t addr_from, uint8_t addr_to, void *hub)
{
	modbus_core_t *core = calloc(1, sizeof(modbus_core_t));

	core->bus = bus;
	core->master = modbus_master_new(bus, hub);
	modbus_master_dev_t *master = core->master;
	modbus_master_set_core(master, core);

	modbus_dev_add(master, 0);
	for (uint8_t i=addr_from; i<addr_to + 1; i++)
	{
		modbus_dev_add(master, i);
	}
	core->queue = xQueueCreate(20, sizeof(msg_t));
	return core;
}

static void modbus_core_msg_proccess(struct modbus_core_t *core, uint64_t timeout)
{
	msg_t msg;
	uint8_t buf_len;
	if (xQueueReceive(core->queue, &msg, timeout / portTICK_PERIOD_MS) == pdTRUE)
		{
			buf_len = msg.data_size;
			msg.status = modbus_master_transaction(core->master, msg.data, msg.data_len, msg.data, &buf_len);
			msg.data_len = buf_len;
			if (msg.return_queue) xQueueSendToFront(msg.return_queue, (void *)&msg, 0);
		}
}

void modbus_core_task(void *parm)
{
	modbus_core_t *core = parm;

	while (1)
	{
		modbus_core_msg_proccess(core, 5);
		modbus_rutine(core->master, 50);
	}
}

void modbus_core_send_msg(modbus_core_t *core, msg_t *msg)
{
	xQueueSendToFront(core->queue, (void *)msg, 0);
}
int modbus_core_recv_msg(modbus_core_t *core, msg_t *msg)
{
	return (xQueueReceive(msg->return_queue, msg, 100));
}

modbus_err_t modbus_core_transaction(modbus_core_t *core, const uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len)
{
	uint8_t buf[128];
	msg_t msg;
	memcpy(buf, data_in, data_in_len);
	msg.data_len = data_in_len;
	msg.data = buf;
	msg.data_size= sizeof(buf);


	if (data_out == NULL)
	{
		msg.return_queue = NULL;
		modbus_core_send_msg(core, &msg);
		return MODBUS_OK;
	}

	msg.return_queue = xQueueCreate(1, sizeof(msg_t));
	modbus_core_send_msg(core, &msg);
	if (modbus_core_recv_msg(core, &msg) != pdTRUE) goto exit;
	memcpy(data_out, buf, msg.data_len);
	*data_out_len = msg.data_len;

	exit:
		vQueueDelete(msg.return_queue);
		return msg.status;
}


