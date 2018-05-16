/*
 * modbus_dev.c

 *
 *  Created on: May 14, 2018
 *      Author: adam
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"

#include "modbus_dev.h"

#define MODBUS_DEV_MAX 100
#define MODBUS_DEV_DEFAULT_RESP_LATENCY 10 //ms
typedef struct bus_t bus_t;

typedef enum{
	MODBUS_DEV_DISCONNECTED,
	MODBUS_DEV_CONNECTED
}dev_state_t;

typedef struct
{
	uint8_t addrres;
	uint8_t reg[256];
	uint8_t state;
	int respond_latency;
	bus_t *bus;
	char name[32];
	char frimware_ver[32];

	void *statistic;
}modbus_dev_t;

modbus_dev_t *modbus_get_by_addr(struct modbus_master_dev_t *master, uint8_t addr);

struct modbus_master_dev_t
{
	bus_t *bus;
	uint8_t pull_count;
	uint8_t discovery_count;
	modbus_dev_t *slave_devs[MODBUS_DEV_MAX];
	size_t slave_devs_len;
};

modbus_err_t modbus_dev_proc_data(modbus_master_dev_t *dev, const uint8_t *data, const uint8_t data_len)
{
	return MODBUS_OK;
}

void modbus_dev_raport_err(modbus_dev_t *dev, modbus_err_t ret)
{
	ESP_LOGI("ERR RAPORT: ", "Dev name: %s, dev addr: %d, err code %d", dev->name, dev->addrres, ret);
	return;
}

modbus_err_t modbus_dev_send_token(modbus_dev_t *dev)
{
	uint8_t data_out_len = 20;
	uint8_t data_out[data_out_len];
	uint8_t data_in[] = {dev->addrres, TOKEN};
	modbus_err_t ret = bus_transaction(dev->bus , data_in, sizeof(data_in), data_out, &data_out_len, dev->respond_latency);
	if (ret != MODBUS_OK)
		{
			modbus_dev_raport_err(dev, ret);
			return ret;
		}
	return modbus_dev_proc_data(dev, data_out, data_out_len);
}

void modbus_devs_poll(struct modbus_master_dev_t *master)
{
	if (master->pull_count >= master->slave_devs_len) master->pull_count = 0;

	modbus_dev_t *dev = master->slave_devs[master->pull_count];
	if (dev->state == MODBUS_DEV_CONNECTED) modbus_dev_send_token(dev);
	master->pull_count++;
}

static void modbus_discovert(modbus_dev_t *dev)
{
	uint8_t data_in[] = {dev->addrres, INDENTITY};
	uint8_t buf[34];
	uint8_t len = sizeof(buf);
	modbus_err_t ret = bus_transaction(dev->bus , data_in, sizeof(data_in), buf, &len, dev->respond_latency);
	if (ret != MODBUS_OK)
		return;
	strncpy(dev->name, (char *)buf + 2, len - 2);
	data_in[1] = FIRMWARE_INFO;
	ret = bus_transaction(dev->bus , data_in, sizeof(data_in), buf, &len, dev->respond_latency);
	if (ret != MODBUS_OK)
		return;
	strncpy(dev->frimware_ver, (char *)buf + 2, len - 2);
	dev->state = MODBUS_DEV_CONNECTED;
}

void modbus_devs_discovery(struct modbus_master_dev_t *master)
{

	if (master->discovery_count >= master->slave_devs_len) master->discovery_count = 0;

	modbus_dev_t *dev = master->slave_devs[master->discovery_count];
	if (dev->state == MODBUS_DEV_DISCONNECTED)
	{
		modbus_discovert(dev);
	}
	master->discovery_count++;
}

modbus_err_t modbus_dev_read(modbus_dev_t *dev, uint8_t *data, uint8_t *data_len)
{
	return MODBUS_OK;
}


modbus_err_t modbus_dev_transaction(struct modbus_master_dev_t *master, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len)
{
	modbus_dev_t *dev;
	modbus_err_t ret;

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

	ret = bus_transaction(dev->bus , data_in, data_in_len, data_out, data_out_len, dev->respond_latency);
	if (ret != MODBUS_OK)
	{
		*data_out_len = 0;
		modbus_dev_raport_err(dev, ret);
		return ret;
	}
	return modbus_dev_proc_data(master, data_out, *data_out_len);
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

modbus_dev_t *modbus_get_by_addr(struct modbus_master_dev_t *master, uint8_t addr)
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
	if (dev == NULL) return MODBUS_ERR_ALLOCATOR;
	dev->bus = master->bus;
	master->slave_devs[master->slave_devs_len++] = dev;

	return MODBUS_OK;
}

modbus_master_dev_t *modbus_master_new(bus_t *bus)
{
	modbus_master_dev_t *master = calloc(1, sizeof(modbus_master_dev_t));
	if (master == NULL) return NULL;
	master->bus = bus;
	master->slave_devs_len = 0;
	return master;
}


