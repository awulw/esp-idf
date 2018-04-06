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
	uint8_t* buf;
	size_t buf_size;
	recive_t recive_cb;
}uart_context_t;

void serial_init(hm_serial_t *handler, int port, int baud_rate, const char pattern_chr, size_t buf_size, recive_t recv_cb, bool queue)
{
	uart_context_t *uart = calloc(1, sizeof(uart_context_t));
	uart->buf = (uint8_t*) malloc(buf_size);

	uart->buf_size = buf_size;
	uart_config_t config = {
			        .baud_rate = baud_rate,
			        .data_bits = UART_DATA_8_BITS,
			        .parity = UART_PARITY_DISABLE,
			        .stop_bits = UART_STOP_BITS_1,
			        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
			    };
	memcpy(&uart->config, &config, sizeof(uart_config_t));
	uart_param_config((uart_port_t)port, &uart->config);
	uart_set_pin(port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	uart_driver_install(port, buf_size * 2, buf_size * 2, 20, queue ? &uart->queue : NULL, 0);
	if (pattern_chr)
		{
			handler->pattern_det = true;
			uart_enable_pattern_det_intr(port, pattern_chr, 1, 10000, 10, 10);
			uart_pattern_queue_reset(port, 20);
			ESP_LOGI(TAG, "patern inited %d", (int)handler);
		}
	uart->recive_cb = recv_cb;

	handler->uart_num = port;

	handler->priv_data = uart;
	ESP_LOGI(TAG, "init %d", (int)handler->priv_data);
	ESP_LOGI(TAG, "init handler %d", (int)handler);

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
		delay = 0;
	return uart_read_bytes(handler->uart_num, data, data_len, delay);
}

void serial_wait_for_data_recv(hm_serial_t *handler, uint32_t time_out)
{
	uart_event_type_t ev_type;
	if (handler->pattern_det)
	{
		ev_type = UART_PATTERN_DET;
	}
	else
	{
		ev_type = UART_DATA;
	}
	uart_context_t *uart = handler->priv_data;
	uart_event_t event = {.type = UART_EVENT_MAX};
	while (xQueueReceive(uart->queue, (void * )&event, time_out / portTICK_PERIOD_MS))
	{
		if (event.type == UART_EVENT_MAX || event.type == ev_type) break;
	}

}

void serial_task(hm_serial_t *hm_serial)
{
	uart_context_t *uart = hm_serial->priv_data;
	uart_event_t event;
	//while (1);
	//ESP_LOGI(TAG, "UART2 %d serial %d", (int)hm_serial);
	if (!uart)
	{
		ESP_LOGI(TAG, "Serial is not initialized");
		goto exit;
	}
	int bytes;
	while (true)
	{
		serial_wait_for_data_recv(hm_serial, 1000);
		bytes = serial_recive(hm_serial, uart->buf, uart->buf_size, 0);
		if (uart->recive_cb) uart->recive_cb(hm_serial, uart->buf, bytes);

//		ESP_LOGI(TAG, "patern %d", (int)hm_serial->pattern_det);
//		if (xQueueReceive(uart->queue, (void * )&event, (portTickType)portMAX_DELAY))
//		{
//			switch(event.type)
//			{
//				case UART_BUFFER_FULL:
//				case UART_FIFO_OVF:
//					ESP_LOGI(TAG, "BUFF OVERFLOW");
//					uart_flush_input(hm_serial->uart_num);
//					xQueueReset(uart->queue);
//					break;
//				case UART_DATA:
//					//if (hm_serial->pattern_det) break;
//					//int bytes = serial_recive(hm_serial, uart->buf, event.size, 0);
//					//if (uart->recive_cb) uart->recive_cb(hm_serial, uart->buf, bytes);
//					if (hm_serial->pattern_det)
//					{
//						break;
//					}
//				case UART_PATTERN_DET:
//
//						bytes = serial_recive(hm_serial, uart->buf, uart->buf_size, 0);
//						if (uart->recive_cb) uart->recive_cb(hm_serial, uart->buf, bytes);
//
//					break;
//				default:
//				    ESP_LOGI(TAG, "uart event type: %d", event.type);
//				    break;
//			}
//		}
	}
	exit:
	vTaskDelete(NULL);

}
