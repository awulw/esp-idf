/*
 * serial.c
 *
 *  Created on: Apr 3, 2018
 *      Author: adam
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "serial.h"

static const char *TAG = "SERIAL";

typedef struct{
	uart_config_t config;
	QueueHandle_t queue;
	uart_port_t port;
	uint8_t* buf;
	size_t buf_size;
	recive_t recive_cb;
}uart_context_t;

void serial_init(hm_serial_t *handler, int port, int baud_rate, const char pattern_chr, size_t buf_size, recive_t recv_cb)
{
	uart_context_t *uart = malloc(sizeof(uart_context_t));
	uart->buf = (uint8_t*) malloc(buf_size);
	uart->buf_size = buf_size;
	uart_config_t config = {
			        .baud_rate = baud_rate,
			        .data_bits = UART_DATA_8_BITS,
			        .parity = UART_PARITY_DISABLE,
			        .stop_bits = UART_STOP_BITS_1,
			        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
			    };

	uart_param_config((uart_port_t)port, &config);
	uart_set_pin(port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(port, buf_size * 2, buf_size * 2, 20, uart->queue, 0);
	if (pattern_chr)
		{
			handler->pattern_det = true;
			uart_enable_pattern_det_intr(port, pattern_chr, 1, 10000, 10, 10);
		}
	uart_pattern_queue_reset(port, 20);

	uart->recive_cb = recv_cb;
	uart->port = (uart_port_t)port;
	// memcpy(&config, &uart->config, sizeof(uart_config_t));


	handler->priv_data = uart;
}


//void serial_init(uart_port_t port, QueueHandle_t *queue, uart_config_t *config, size_t buf_size, const char pattern_chr)
//{
//	uart_param_config(port, config);
//	uart_set_pin(port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
//	uart_driver_install(port, buf_size * 2, buf_size * 2, 20, queue, 0);
//	uart_enable_pattern_det_intr(port, '\r', pattern_chr, 10000, 10, 10);
//	//Reset the pattern queue length to record at most 20 pattern positions.
//	uart_pattern_queue_reset(port, 20);
//}


int serial_send(hm_serial_t *handler, const uint8_t* data, size_t data_len)
{
	return uart_write_bytes(handler->uart_num, (const char*) data, data_len);
}

int serial_recive(hm_serial_t *handler, uint8_t* data, size_t data_len, uint32_t time_out)
{
	TickType_t delay;
	if (time_out)
		delay = time_out / portTICK_PERIOD_MS;
	else
		delay = portMAX_DELAY;
	return uart_read_bytes(handler->uart_num, data, data_len, delay);
}

void serial_task(hm_serial_t *hm_serial)
{
	uart_context_t *uart = hm_serial->priv_data;
	uart_event_t event;

	esp_log_level_set(TAG, ESP_LOG_INFO);
	if (!uart)
	{
		ESP_LOGI(TAG, "Serial is not initialized");
		return;
	}

	while (true)
	{
		if (xQueueReceive(uart->queue, (void * )&event, (portTickType)portMAX_DELAY))
		{
			switch(event.type)
			{
				case UART_BUFFER_FULL:
				case UART_FIFO_OVF:
					ESP_LOGI(TAG, "BUFF OVERFLOW");
					uart_flush_input(uart->port);
					xQueueReset(uart->queue);
					break;
				case UART_DATA:
					if (hm_serial->pattern_det) break;
				case UART_PATTERN_DET:
					if (hm_serial->pattern_det)
						{
							serial_recive(hm_serial, uart->buf, event.size, 0);
							if (uart->recive_cb) uart->recive_cb(hm_serial, uart->buf, event.size);
						}
					break;
				default:
				    ESP_LOGI(TAG, "uart event type: %d", event.type);
				    break;
			}
		}
	}

}
