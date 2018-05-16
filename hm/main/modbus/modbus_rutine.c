/*
 * modbus_rutine.c
 *
 *  Created on: May 14, 2018
 *      Author: adam
 */
#include "modbus_rutine.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "sys/time.h"

typedef void (*func_t)(modbus_master_dev_t *);

typedef struct{
	func_t func;
	uint64_t period;
	uint64_t last_exec;
	uint8_t priority;
}rutine_item_t;

static uint64_t millis() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return (tv.tv_sec *1000  + (tv.tv_usec)/1000);
}

static rutine_item_t rutine_tab[] =
{
		{.func = modbus_devs_poll, .period = 100, .last_exec = 0, .priority = 1},
		{.func = modbus_devs_discovery, .period = 100, .last_exec = 0, .priority = 0},
		{.func = NULL}
};

void (*init)();
void (*discovery)();
void (*reg_update)();

uint8_t is_ready(rutine_item_t *item)
{
	if (millis() - item->last_exec > item->period)
	{
		return 1;
	}
	return 0;
}

func_t shedule(rutine_item_t rutine_tab[])
{
	uint8_t ready_prior_index = 0;
	uint8_t ready_prior_count = 0;

	uint8_t i = 0;
	while (rutine_tab[i].func != NULL)
	{
		if (is_ready(rutine_tab + i))
		{
			if (rutine_tab[ready_prior_index].priority <= rutine_tab[i].priority)
			{
				ready_prior_index = i;
				ready_prior_count++;
			}
		}
		i++;
	}
	if (!ready_prior_count) return NULL;
	rutine_tab[ready_prior_index].last_exec = millis();
	return rutine_tab[ready_prior_index].func;
}

void modbus_rutine(modbus_master_dev_t *master, uint64_t run_time)
{
	func_t func;
	uint64_t time = millis();
	while (millis() - time < run_time)
	{
		func = shedule(rutine_tab);
		if (func != NULL)
		{
			func(master);
		}
	}
}
