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
#include "sys/time.h"

#include "serial.h"

#define RTS_PIN UART_PIN_NO_CHANGE

struct hm_serial_t
{
	int uart_num;
	int baud_rate;
	uint64_t end_frame_timeout;
	bool pattern_det;
	void *driver;
	void *priv_data;
};

typedef struct{
	uart_config_t config;
	QueueHandle_t queue;
	uint8_t* buf;
	size_t buf_size;
}uart_context_t;

static uint64_t usec() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return (tv.tv_sec *1000000  + (tv.tv_usec));
}

static void serial_init(hm_serial_t *handler, int port, int baud_rate, const char *pattern_chr, size_t buf_size)
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
	uart_set_pin(port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, RTS_PIN, UART_PIN_NO_CHANGE);

	uart_driver_install(port, buf_size * 2, buf_size * 2, 100, &uart->queue, 0);
	if (pattern_chr)
		{
			handler->pattern_det = true;
			uart_enable_pattern_det_intr(port, *pattern_chr, 1, 10000, 10, 10);
			uart_pattern_queue_reset(port, 20);
		}
	handler->end_frame_timeout = 35000000 / (baud_rate); //3.5 chr for modbus rtu
	handler->uart_num = port;
	handler->priv_data = uart;

}

int serial_send(hm_serial_t *handler, const uint8_t* data, size_t data_len)
{
	return uart_write_bytes(handler->uart_num, (const char*) data, data_len);
}

static int serial_recive(hm_serial_t *handler, uint8_t* data, size_t data_len)
{
	int bytes = uart_read_bytes(handler->uart_num, data, data_len, 0);
	if (handler->pattern_det)
	{
		return bytes;
	}
	int bytes_before;
	uint64_t m = usec();
	uint64_t time_out = handler->end_frame_timeout;
	while (usec() - m < time_out)
	{
		bytes_before = bytes;
		bytes += uart_read_bytes(handler->uart_num, data + bytes, data_len - bytes, 0);
		if (bytes_before != bytes)
		{
			m = usec();
		}
		if (data_len == bytes) break;
	}
	return bytes;
}

static modbus_err_t serial_wait_for_data_recv(hm_serial_t *handler, uint32_t time_out)
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
	xQueueReset(uart->queue);
	if (event.type == UART_EVENT_MAX)
	{
		return MODBUS_ERR_RECV_TIMEOUT;
	}
	return MODBUS_OK;

}

static void serial_wait_send_done(hm_serial_t *handler, uint32_t time_out)
{
	uart_wait_tx_done(handler->uart_num, time_out / portTICK_PERIOD_MS);
}

static void serial_set_rts(hm_serial_t *handler, int level)
{
	#if RTS_PIN > 0
	    uart_set_rts(handler->uart_num, level);
	#endif
}

static void serial_flush(hm_serial_t *handler)
{
	uart_flush(handler->uart_num);
}


static serial_driver_t esp32_serial =
		{
				.init = serial_init,
				.send = serial_send,
				.recive = serial_recive,
				.wait_for_data_recv = serial_wait_for_data_recv,
				.wait_send_done = serial_wait_send_done,
				.set_rts = serial_set_rts,
				.flush = serial_flush
		};

hm_serial_t *serial_create(int uart_num, int baud_rate, const char *pattern_chr, size_t buf_size)
{
       hm_serial_t *serial = calloc(1, sizeof(hm_serial_t));
       if (serial == NULL) return NULL;
       serial_init(serial, uart_num, baud_rate, pattern_chr, buf_size);
       serial->driver = &esp32_serial;
       return serial;
}

serial_driver_t *serial_get_driver(hm_serial_t *handler)
{
	return handler->driver;
}
