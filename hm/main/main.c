/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "console/console.h"
#include "modbus/modbus_trafic.h"
#include "modbus/serial.h"




void recv_ex(void *handler ,uint8_t* data, size_t len)
{
	serial_send(handler, data, len);
}



void app_main()
{
	hm_serial_t modbus1;

	serial_init(&modbus1, 0, 115200, '\r', 1024, recv_ex);
	xTaskCreate((TaskFunction_t)serial_task, "modbas_trafic_task", 8192, (void*) (&modbus1), 10, NULL);
}
