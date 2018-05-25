/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <pthread.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "esp_log.h"

#include <string.h>
#include "console/console.h"
#include "modbus/serial.h"
#include "modbus/driver_ascii.h"
#include "modbus/modbus.h"
#include "modbus/modbus_core.h"

#include "device/device.h"

static hm_serial_t *debug_serial;

void hk_console_printf(const char *text, ...)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    char buffer[128];
    uint32_t length;
    va_list arg_list;
    if (text != NULL)
    {
        /* Print the string to the buffer.*/
        va_start(arg_list, text);
        length = vsnprintf(buffer, sizeof(buffer), text, arg_list);
        va_end(arg_list);
        {
        	serial_send(debug_serial, (uint8_t *)buffer, length);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void app_main()
{
	//hm_serial_t *rs485 = serial_create(0, 115200, NULL, 1024);
	//bus_t *bus = bus_create(rs485, serial_get_driver(rs485), &rtu_driver);

	hm_serial_t *rs485 = serial_create(1, 115200, "\r", 1024);
	debug_serial = serial_create(0, 115200, "\r", 1024);
	bus_t *bus = bus_create(rs485, serial_get_driver(rs485), &ascii_driver);

	device_hub_t *dev_hub = device_hub_create();

	modbus_core_t *core = modbus_core_create(bus, 1, 5, dev_hub);

	xTaskCreate(modbus_core_task, "modbas_core_task", 8192, core, 10, NULL);
	uint8_t buf[128];
	uint8_t data_in[] = {0x01, 0x81, 0x00, 0x6b, 0x00, 0x03};
	uint8_t len = 0;
	uint8_t ret = 0;

	while(1)
	{
		sleep(1);
		len = 2;
		buf[0]=0;
		ret = modbus_core_transaction(core, data_in, 2, buf, &len);

		ESP_LOGI("response: ", "ret = %d, len = %d, data = %02x %02x %02x %02x %02x %02x %02x %02x", ret, len, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		//modbus_core_send_msg(core, &msg);
	}
}
