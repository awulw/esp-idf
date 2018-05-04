/*
 * driver_rtu.c
 *
 *  Created on: Apr 13, 2018
 *      Author: adam
 */

#include "modbus.h"

static modbus_err_t rtu_data_to_frame(uint8_t *data, uint8_t date_len, modbus_frame_t *frame)
		{
			return MODBUS_ERR_NOT_IMPLEMENTED;
		}
static modbus_err_t rtu_frame_to_data(modbus_frame_t *frame, uint8_t *data, uint8_t *date_len)
		{
			return MODBUS_ERR_NOT_IMPLEMENTED;
		}

modbus_driver_t rtu_driver =
		{
				.frame_to_data = rtu_frame_to_data,
				.data_to_frame = rtu_data_to_frame
		};
