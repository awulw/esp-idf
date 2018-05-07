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

#include "console/console.h"
#include "modbus/modbus_trafic.h"
#include "modbus/serial.h"
#include "modbus/driver_ascii.h"
#include "modbus/modbus.h"



void recv_ex(void *handler ,uint8_t* data, size_t len)
{
	ESP_LOGI("cc","callback");
	//serial_send(handler, data, len);
}

static void uart_event_task(void *pvParameters)
{
	while (1)
	{

	}
}

void app_main()
{
	hm_serial_t *rs485 = serial_create(0, 115200, '\r', 1024);

	modbus_t *modbus = modbus_create(rs485, serial_get_driver(rs485), &ascii_driver);


	//xTaskCreate(serial_task, "modbas_trafic_task", 8192, rs485, 10, NULL);
	uint8_t buf[128];
	uint8_t len =0;
	uint8_t ret;
	while(1)
	{
		ret = modbus_transaction(modbus, (uint8_t *)"text\n", 4, buf, &len, 1000);
		buf[len]=0;
		ESP_LOGI("re", "%s len {%d}  ret = %d\n", buf, len , ret);
	}

	//xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}
