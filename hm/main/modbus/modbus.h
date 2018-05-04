/*
 * modbus.h
 *
 *  Created on: Apr 6, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_H_
#define HM_MAIN_MODBUS_MODBUS_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*recive_t)(void *handler ,uint8_t* data, size_t len);


typedef struct
{
	int uart_num;
	int baud_rate;
	bool pattern_det;
	void *driver;
	void *priv_data;
}hm_serial_t;

typedef enum
{
	MODBUS_OK,
	MODBUS_ERR_INPUT,
	MODBUS_ERR_OUTPUT,
	MODBUS_ERR_ASCII_NO_HEAD,
	MODBUS_ERR_ASCII_NO_TAIL,
	MODBUS_ERR_ASCII_INVALID_FORMAT,
	MODBUS_ERR_ASCII_LRC,
	MODBUS_ERR_INVALID_INPUT_DATA,
	MODBUS_ERR_RECV_TIMEOUT,
	MODBUS_ERR_DATA_LEN,
	MODBUS_ERR_NOT_IMPLEMENTED

}modbus_err_t;


typedef enum
{
	MODBUS_ASCII,
	MODBUS_RTU
}modbus_type_t;

typedef struct
{
	hm_serial_t *rs485;
	modbus_type_t type;
	void *priv;

}handler_modbus_t;

typedef struct
{
	uint8_t *data;
	size_t data_len;
	size_t data_size;
}modbus_frame_t;


typedef struct
{
	void (*init)(void);
	modbus_err_t (*frame_to_data)(modbus_frame_t *frame ,uint8_t *data, uint8_t *date_len);
	modbus_err_t (*data_to_frame)(uint8_t *data, uint8_t date_len, modbus_frame_t *frame);
}modbus_driver_t;



typedef struct
{
	void (*init)(hm_serial_t *handler, int uart_num, int baud_rate, const char pattern_chr, size_t buf_size);
	int (*send)(hm_serial_t *handler, const uint8_t* data, size_t data_len);
	int (*recive)(hm_serial_t *handler, uint8_t* data, size_t data_len, uint32_t time_out);
	modbus_err_t (*wait_for_data_recv)(hm_serial_t *handler, uint32_t time_out);
	void (*wait_send_done)(hm_serial_t *handler, uint32_t time_out);
	void (*set_rts)(hm_serial_t *handler, int level);
	void (*flush)(hm_serial_t *handler);
}serial_driver_t;


hm_serial_t *serial_create(int uart_num, int baud_rate, const char pattern_chr, size_t buf_size, serial_driver_t *serial_driver);

handler_modbus_t *modbus_create(hm_serial_t *rs485, modbus_driver_t *modbus_driver);
void modbus_task(void *parm);
modbus_err_t modbus_transaction(handler_modbus_t *handler, uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out, uint8_t *data_out_len, uint32_t timeout);

#endif /* HM_MAIN_MODBUS_MODBUS_H_ */

