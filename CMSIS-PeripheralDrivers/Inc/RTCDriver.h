/*
 * RCTDriver.h
 *
 *  Created on: Jun 7, 2023
 *      Author: anarrietam
 */

#ifndef RTCDRIVER_H_
#define RTCDRIVER_H_

#include "stm32f4xx.h"

#define MODE_AM	    0
#define MODE_PM	    1
#define MODE_24HOUR 2
#define MODE_AMPM   3


typedef struct
{
	uint8_t		RTC_Hora;
	uint8_t		RTC_Minutos;
	uint8_t		RTC_Segundos;
	uint8_t		RTC_Dias;
	uint8_t		RTC_Meses;
	uint16_t	RTC_Años;
	uint8_t		RTC_format;
	uint8_t     RTC_AMPM;
}RTC_Handler_t;


void configRCT(RTC_Handler_t *ptrRTCHandler);
uint8_t* read_time(void);
uint8_t* read_date(void);


#endif /* RTCDRIVER_H_ */
