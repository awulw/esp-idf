/*
 * modbas_ascii.c
 *
 *  Created on: Apr 12, 2018
 *      Author: adam
 */

#define _GNU_SOURCE
#include "modbus.h"
#include <string.h>

#define END_FRAME "\r\n"
#define ASCII_TAIL_LEN 3 //crc + /r/n/r/n


enum
{
	ASCII_OK,
	ASCII_ERR_NO_HEAD,
	ASCII_ERR_NO_TAIL,
	ASCII_ERR_INVALID_FORMAT,
	ASCII_ERR_LRC

}ascii_error;

uint8_t hex2nible(uint8_t c) {
  if(c>='0' && c<='9') return(c - '0');
  if(c>='A' && c<='F') return(c - 'A' + 10);
  return 0xff;
}

uint8_t nible2hex(uint8_t d) {

  if (d<10 )  return('0' + d);
  if (d>=10 && d<=15)  return('A' + (d - 10) );
  return 0;
}

void byte2hex(uint8_t c, uint8_t *h1, uint8_t *h2) {

   *h1 = nible2hex((c & 0xf0) >> 4);
   *h2 = nible2hex( c & 0x0f);
}

uint8_t hex2byte(uint8_t s1, uint8_t s2, uint8_t *byte) {
	uint8_t n1 = hex2nible(s1);
	uint8_t n2 = hex2nible(s2);
	if (n1 > 0x0F) return 0;
	if (n2 > 0x0F) return 0;
	*byte = (n1<<4) + n2;
	return 1;
}

uint8_t lrcgen(uint8_t *fptr, uint8_t nb)                          /*funkcja zwraca jeden bajt*/
{
	uint8_t lrc, sum = 0;
     while (nb--)
        sum += *fptr++;       /*sumowanie kolejnych bajtów bez przeniesień*/
     lrc = ~sum + 1;                                   /*uzupełnienie do 2*/
     return (lrc);
}


static uint8_t ascii_data_to_frame(uint8_t *data, uint8_t data_len, modbus_frame_t *frame)
{
	frame->data_len = -1;
	if (data_len == 0) return -1;
	if (data_len * 2 + 1 + ASCII_TAIL_LEN > frame->data_size) return -1;
	frame->data[0] = ':';

	uint8_t i;
	uint8_t *f_ptr = frame->data + 1;
	for (i= 0; i < data_len; i++)
	{
		byte2hex(data[i], f_ptr, f_ptr + 1);
		f_ptr=f_ptr + 2;
	}
	byte2hex(lrcgen(data, data_len), f_ptr, f_ptr + 1);
	f_ptr +=2;

	*(f_ptr++) = '\r';
	*(f_ptr++) = '\n';
	//*(f_ptr++) = '\r';
	//*(f_ptr++) = '\n';

	frame->data_len = f_ptr - frame->data;
	return 0;
}

static uint8_t ascii_frame_to_data(modbus_frame_t *frame, uint8_t *data, uint8_t *data_len)
{
	uint8_t *f_start_ptr = frame->data;
	uint8_t *f_end_ptr;
	uint8_t *data_ptr = data;
	uint8_t i;

	for (i = 0; i < frame->data_len; i++)
	{
		if (*(f_start_ptr++) == ':') break;
	}

	if (i > frame->data_len - sizeof(END_FRAME)) return ASCII_ERR_NO_HEAD;
	f_end_ptr = memmem(f_start_ptr, frame->data_len, END_FRAME, sizeof(END_FRAME) - 1);
	if (f_end_ptr == NULL) return ASCII_ERR_NO_TAIL;
	*data_len = 0;
	for (uint8_t *ptr = f_start_ptr; ptr < f_end_ptr; ptr += 2)
	{
		if (hex2byte(*ptr, *(ptr + 1), data_ptr++) == 0) return ASCII_ERR_INVALID_FORMAT;
		(*data_len)++;
	}
	(*data_len)--;
	if (lrcgen(data, data_ptr - data -1) != *(data_ptr -1)) return ASCII_ERR_LRC;
	return ASCII_OK;
}

modbus_driver_t ascii_driver =
		{
				.frame_to_data = ascii_frame_to_data,
				.data_to_frame = ascii_data_to_frame
		};
