/*
 * modbus_conf.h
 *
 *  Created on: May 10, 2018
 *      Author: adam
 */

#ifndef HM_MAIN_MODBUS_MODBUS_CONF_H_
#define HM_MAIN_MODBUS_MODBUS_CONF_H_

//#define MODBUS_RTU
//#define MODBUS_ASCI



#if defined(MODBUS_RTU) && defined(MODBUS_ASCI)
#error "Can`t set driver for both ASCI and RTU"
#endif


#endif /* HM_MAIN_MODBUS_MODBUS_CONF_H_ */
