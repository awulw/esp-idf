/*
 * modbus_const.h
 *
 *  Created on: May 23, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_CONST_H_
#define HM_MAIN_MODBUS_MODBUS_CONST_H_

#define INDENTITY 0x00
#define READ_N_REG 0x01
#define WRITE_N_REG 0x02
#define READ_REG 0x03
#define WRITE_REG 0x04
#define WRITE_N_REG_IN_EEPPROM 0x05
#define WRITE_AND 0x06
#define WRITE_OR 0x07
#define WRITE_EXOR 0x08
#define RESET 0x30
#define LED_OFF 0x50
#define LED_ON 0x51
#define BULB_OFF 0x52
#define BULB_ON 0x53
#define ALL_OFF 0x54
#define ALL_ON 0x55

#define REG_ANALOG_OUTPUT 1//1..8
#define REG_DIGITAL_OUTPUT 9//9..10
#define REG_DIGITAL_INPUT 11//11..12
#define REG_LIGHT_CONTROL 13
#define REG_COMMAND_CONTROL 14
#define REG_ANALOG_MEM_SPACE 16//16...32


//#define REG_TIMER_MEM_SPACE 48
#define REG_LCD_MEM_SPACE 64
#define REG_LIGHT_DIGITAL_STATE 96//96 97
#define REG_LIGHT_ANALOG_STATE 98//98 99 100 101 102 103 104 105


#define REG_SIZE 255

#define TOKEN_SIZE 4
#define TOKEN_QUANTITY 16
#define TOKEN_BUF_SIZE 4
#define EXTRA_TOKEN 255


#define MAX_DEV 31//tylko do okreslenia adresu przy pierwszym uruchomieniu
#define MAX_ID_SIZE 16
#define MAX_INCOMING_FRAME 22
#define MAX_DATA_LENGHT_IN_TOKEN 16

#define TOKEN 0x0C

#define FIRMWARE_INFO 0x20

enum{
	MODBUS_ADDRESS = 0,
	MODBUS_FUNCTION,
	MODBUS_DATA
};

#endif /* HM_MAIN_MODBUS_MODBUS_CONST_H_ */
