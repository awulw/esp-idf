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
#include "modbus.h"

hm_serial_t *serial_create(int uart_num, int baud_rate, const char *pattern_chr, size_t buf_size);
serial_driver_t *serial_get_driver(hm_serial_t *handler);


#endif /* HM_MAIN_MODBUS_SERIAL_H_ */
