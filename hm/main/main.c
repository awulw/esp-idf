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

void app_main()
{
	//xTaskCreate(console_task, "console_task", 8192, NULL, 10, NULL);
	xTaskCreate(modbas_trafic_task, "modbas_trafic_task", 8192, NULL, 10, NULL);

}
