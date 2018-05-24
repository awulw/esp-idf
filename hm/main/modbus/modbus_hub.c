/*
 * modbus_hub.c
 *
 *  Created on: May 23, 2018
 *      Author: adam
 */

#include "modbus_const.h"
#include "../device/device.h"
#include "modbus_core.h"
#include "modbus_dev.h"

enum{
	DRIVER_IO_REG_NR,
	DRIVER_IO_BIT_MASK
};

static int modbus_send_to_queue(modbus_dev_t *modbus_dev, uint8_t *data, uint8_t data_len)
{
	modbus_core_t *core = modbus_dev_get_core(modbus_dev);
	modbus_core_transaction(core, data, data_len, NULL, NULL);
	return 0;
}

static int modbus_io_bit_read(void *modbus_dev, void *driver_context, uint8_t *buf, uint8_t buf_len)
{
	uint8_t *driver_data = driver_context;
	uint8_t *reg = modbus_dev_get_reg(modbus_dev);

	if (reg[driver_data[0]] & driver_data[1])
	{
		*buf = 1;
	}
	else
	{
		*buf = 0;
	}
	return 0;
}

static int modbus_io_bit_write(void *modbus_dev, void *driver_context, uint8_t *buf, uint8_t buf_len)
{
	uint8_t *driver_data = driver_context;
	uint8_t value = (*buf) & 0x01;
	uint8_t buf_to_send[4]; //ADDR, FUNC, DATA, DATA + 1
	buf_to_send[MODBUS_ADDRESS] = modbus_dev_get_addr(modbus_dev);
	buf_to_send[MODBUS_DATA] = driver_data[DRIVER_IO_REG_NR]; //reg_nr

	if (value)
	{
		buf_to_send[MODBUS_FUNCTION] = WRITE_EXOR;
		buf_to_send[MODBUS_DATA + 1] = driver_data[DRIVER_IO_BIT_MASK]; //bit_mask
	}
	else
	{
		buf_to_send[MODBUS_FUNCTION] = WRITE_AND;
		buf_to_send[MODBUS_DATA + 1] = ~driver_data[DRIVER_IO_BIT_MASK]; //bit_mask
	}
	return modbus_send_to_queue(modbus_dev, buf_to_send, sizeof(buf_to_send));
}

static io_driver_t io_driver_reg_mask = {.read = modbus_io_bit_read, .write = modbus_io_bit_write};

static void chr_register_onoff(device_t *device, modbus_dev_t *modbus_dev, uint8_t reg_nr, uint8_t bit_mask)
{
	uint8_t *drv_context = malloc(2 * sizeof(uint8_t));
	drv_context[DRIVER_IO_REG_NR] = reg_nr;
	drv_context[DRIVER_IO_BIT_MASK] = bit_mask;
	device_add_chr(device, CHR_ONOFF, drv_context, &io_driver_reg_mask);
	modbus_dev_add_reg_notification(modbus_dev, reg_nr, bit_mask, device, device_notification_cb);
}

void modbus_hub_register(modbus_dev_t *m_dev, device_hub_t *hub)
{
	device_t *light = device_create(hub, m_dev, DEV_LIGHT_BULB);
	chr_register_onoff(light, m_dev, REG_LIGHT_CONTROL, 1);
}
