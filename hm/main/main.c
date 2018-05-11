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
#include "modbus/modbus_trafic.h"
#include "modbus/serial.h"
#include "modbus/driver_ascii.h"
#include "modbus/modbus.h"
#include "modbus/modbus_core.h"




void app_main()
{


	modbus_core_t *core = modbus_core_create(MODBUS_ASCII);


	xTaskCreate(modbus_core_task, "modbas_core_task", 8192, core, 10, NULL);
	uint8_t buf[128];
	uint8_t data_in[] = {0x11, 0x03, 0x00, 0x6b, 0x00, 0x03};
	uint8_t len = 0;
	uint8_t ret = 0;
	while(1)
	{

		//sleep(1);
		ret = modbus_core_transaction(core, data_in, 6, buf, &len);

		buf[len]=0;
		ESP_LOGI("response: ", "ret = %d, len = %d, data = %02x %02x %02x %02x %02x %02x %02x %02x", ret, len, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		//modbus_core_send_msg(core, &msg);
	}

	//xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}
