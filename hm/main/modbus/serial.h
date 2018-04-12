/*
 * serial.h
 *
 *  Created on: Apr 3, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_SERIAL_H_
#define HM_MAIN_MODBUS_SERIAL_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*recive_t)(void *handler ,uint8_t* data, size_t len);


typedef struct
{
	int uart_num;
	int baud_rate;
	bool pattern_det;
	void *priv_data;
}hm_serial_t;


void serial_init(hm_serial_t *handler, int uart_num, int baud_rate, const char pattern_chr, size_t buf_size, recive_t recv_cb,  bool queue);
void serial_task(hm_serial_t *handler);
int serial_send(hm_serial_t *handler, const uint8_t* data, size_t data_len);
int serial_recive(hm_serial_t *handler, uint8_t* data, size_t data_len, uint32_t time_out);
void serial_wait_for_data_recv(hm_serial_t *handler, uint32_t time_out);
void serial_wait_send_done(hm_serial_t *handler, uint32_t time_out);
void serial_set_rts(hm_serial_t *handler, int level);


#endif /* HM_MAIN_MODBUS_SERIAL_H_ */
