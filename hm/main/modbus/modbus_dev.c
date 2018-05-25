/*
 * modbus_dev.c

 *
 *  Created on: May 14, 2018
 *      Author: adam
 */
#include <string.h>
#include "esp_log.h"
#include "modbus_dev.h"
#include "modbus_const.h"
#include "modbus_hub.h"

#define MODBUS_DEV_MAX 100
#define MODBUS_DEV_DEFAULT_RESP_LATENCY 10 //ms
#define MODBUS_DEV_STR_SIZE 32

typedef struct bus_t bus_t;

typedef enum{
	MODBUS_DEV_DISCONNECTED,
	MODBUS_DEV_DISCOVERING,
	MODBUS_DEV_CONNECTED
}dev_state_t;

struct modbus_dev_t
{
	uint8_t addrres;
	uint8_t reg[128];
	uint8_t flash[128];
	uint8_t state;
	int respond_latency;
	bus_t *bus;
	char name[MODBUS_DEV_STR_SIZE];
	char frimware_ver[MODBUS_DEV_STR_SIZE];
	modbus_master_dev_t *master;
	char discocery_step;
	void *statistic;
};



static modbus_dev_t *modbus_get_by_addr(struct modbus_master_dev_t *master, uint8_t addr);
static modbus_err_t modbus_dev_transaction(struct modbus_dev_t *dev, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len);

struct modbus_master_dev_t
{
	bus_t *bus;
	void *core;
	void *hub;
	uint8_t pull_count;
	uint8_t discovery_count;
	modbus_dev_t *slave_devs[MODBUS_DEV_MAX];
	size_t slave_devs_len;
};



modbus_err_t modbus_dev_proc_data(modbus_dev_t *master, const uint8_t *data, const uint8_t data_len)
{
	return MODBUS_OK;
}

void modbus_dev_raport_err(modbus_dev_t *dev, modbus_err_t ret)
{
	ESP_LOGI("ERR RAPORT: ", "Dev name: %s, dev addr: %d, err code %d", dev->name, dev->addrres, ret);
	return;
}

static modbus_err_t modbus_dev_send_token(modbus_dev_t *dev)
{
	uint8_t buf[36];
	uint8_t buf_len = sizeof(buf);
	uint8_t data_in[2];
	data_in[MODBUS_FUNCTION] = TOKEN;
	return modbus_dev_transaction(dev, data_in, sizeof(data_in), buf, &buf_len);
}

void modbus_devs_poll(struct modbus_master_dev_t *master)
{
	if (master->pull_count >= master->slave_devs_len) master->pull_count = 0;

	modbus_dev_t *dev = master->slave_devs[master->pull_count];
	if (dev->state == MODBUS_DEV_CONNECTED) modbus_dev_send_token(dev);
	master->pull_count++;
}

void modbus_devs_poll_broadcast(struct modbus_master_dev_t *master)
{
	modbus_dev_send_token(master->slave_devs[0]);
}

static modbus_err_t modbus_get_func_data(modbus_dev_t *dev, const uint8_t modbus_f_code, uint8_t *buf, uint8_t *buf_size)
{
	uint8_t data_in[2];
	data_in[MODBUS_FUNCTION] = modbus_f_code;
	modbus_err_t ret = modbus_dev_transaction(dev , data_in, sizeof(data_in), buf, buf_size);
	buf[0] = '\0';
	if (ret == MODBUS_OK)
		memmove(buf, buf + 2, *buf_size - 2);
	return ret;
}

static modbus_err_t modbus_get_indentity(modbus_dev_t *dev)
{
	uint8_t len = MODBUS_DEV_STR_SIZE;
	if (dev->addrres == 1) return 0; //remove it
	return modbus_get_func_data(dev, INDENTITY, (uint8_t *)dev->name, &len);
}

static modbus_err_t modbus_get_frimware_ver(modbus_dev_t *dev)
{
	uint8_t len = MODBUS_DEV_STR_SIZE;
	if (dev->addrres == 1) return 0; //remove it
	return modbus_get_func_data(dev, FIRMWARE_INFO, (uint8_t *)dev->frimware_ver, &len);
}

static modbus_err_t modbus_get_flash(modbus_dev_t *dev)
{
	return MODBUS_OK;
}

static void modbus_discovery(modbus_dev_t *dev, device_hub_t *hub)
{
	if (dev->state == MODBUS_DEV_CONNECTED)
		return;

	switch (dev->discocery_step)
	{
		case 0:
			if (modbus_get_indentity(dev) == MODBUS_OK)
			{
				dev->discocery_step++;
				dev->state = MODBUS_DEV_DISCOVERING;

			}
			break;
		case 1:
			if (modbus_get_frimware_ver(dev) == MODBUS_OK)
				dev->discocery_step++;
			break;
		case 2:
			if (modbus_get_flash(dev) == MODBUS_OK)
			{
				dev->discocery_step++;
			}
			break;
		case 3:
			modbus_hub_register(dev, hub);
			dev->discocery_step++;
			dev->state = MODBUS_DEV_CONNECTED;
			break;
	}
}

void modbus_devs_discovery(struct modbus_master_dev_t *master)
{

	if (master->discovery_count >= master->slave_devs_len) master->discovery_count = 0;

	modbus_dev_t *dev = master->slave_devs[master->discovery_count];
	if ((dev->state != MODBUS_DEV_CONNECTED) && (dev->addrres != 0))
	{
		modbus_discovery(dev, master->hub);
	}
	master->discovery_count++;
}

static modbus_err_t modbus_dev_transaction(struct modbus_dev_t *dev, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len)
{
	modbus_err_t ret = 0;

	data_in[MODBUS_ADDRESS] = dev->addrres;

	//TODO retransmission
	if (data_in[MODBUS_FUNCTION] & 0x80) //NO RESPNSE
	{
		data_out = NULL;
		*data_out_len = 0;
	}
	ret = bus_transaction(dev->bus, data_in, data_in_len, data_out, data_out_len, dev->respond_latency);
	if ((ret != MODBUS_OK) && (dev->addrres != 0) && (dev->state == MODBUS_DEV_CONNECTED))
		{
			modbus_dev_raport_err(dev, ret);
		}
	if (ret != MODBUS_OK)
	{
		*data_out_len = 0;
		return ret;
	}
	return modbus_dev_proc_data(dev, data_out, *data_out_len);
}



modbus_err_t modbus_master_transaction(struct modbus_master_dev_t *master, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len)
{
	modbus_dev_t *dev;

	if (data_in_len < 2)
	{
		*data_out_len = 0;
		return MODBUS_ERR_INPUT;//addr, func ...;
	}

	dev = modbus_get_by_addr(master, data_in[0]);
	if (dev == NULL)
	{
		*data_out_len = 0;
		return MODBUS_ERR_ADDR_NOT_EXISTS;
	}
	if (dev->state != MODBUS_DEV_CONNECTED)
	{
		*data_out_len = 0;
		return MODBUS_ERR_ADDR_NOT_CONNECTED;
	}
	return modbus_dev_transaction(dev, data_in, data_in_len, data_out, data_out_len);
}

modbus_dev_t *modbus_dev_new(uint8_t addr)
{
	modbus_dev_t * dev = calloc(1, sizeof(modbus_dev_t));
	if (dev == NULL) return NULL;
	dev->addrres = addr;
	dev->respond_latency = MODBUS_DEV_DEFAULT_RESP_LATENCY;
	strcpy(dev->name, "Unknown");
	return dev;
}

static modbus_dev_t *modbus_get_by_addr(struct modbus_master_dev_t *master, uint8_t addr)
{
	for (uint8_t a = 0; a < master->slave_devs_len; a++)
	{
		if (master->slave_devs[a]->addrres == addr)
			return master->slave_devs[a];
	}
	return NULL;
}

modbus_err_t modbus_dev_add(modbus_master_dev_t *master, uint8_t addr)
{
	if (modbus_get_by_addr(master, addr)) return MODBUS_ERR_ADDR_EXISTS;
	modbus_dev_t *dev = modbus_dev_new(addr);
	dev->master = master;
	if (dev == NULL) return MODBUS_ERR_ALLOCATOR;
	dev->bus = master->bus;
	master->slave_devs[master->slave_devs_len++] = dev;

	return MODBUS_OK;
}

modbus_master_dev_t *modbus_master_new(bus_t *bus, void *hub)
{
	modbus_master_dev_t *master = calloc(1, sizeof(modbus_master_dev_t));
	if (master == NULL) return NULL;
	master->bus = bus;
	master->hub = hub;
	master->slave_devs_len = 0;
	return master;
}

void modbus_master_set_core(modbus_master_dev_t *master, void *core)
{
	master->core = core;
}

void *modbus_dev_get_core(modbus_dev_t *dev)
{
	return dev->master->core;
}

uint8_t modbus_dev_get_addr(modbus_dev_t *dev)
{
	return dev->addrres;
}

uint8_t *modbus_dev_get_reg(modbus_dev_t *dev)
{
	return dev->reg;
}

void modbus_dev_add_reg_notification(modbus_dev_t *dev, uint8_t reg_nr, uint8_t reg_mask, void *context, void (*cb)(void *context))
{
	//TODO
}

