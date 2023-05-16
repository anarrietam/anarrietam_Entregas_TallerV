/*
 * SysTickDriver.h
 *
 *  Created on: May 13, 2023
 *      Author: andres
 */

#ifndef SYSTICKDRIVER_H_
#define SYSTICKDRIVER_H_

#include <stm32f4xx.h>

#define SYSTICK_LOAD_16MHz_1ms      16000
#define SYSTICK_LOAD_100MHz_1ms     100000

void config_SysTick_ms (uint8_t systemClock);
uint64_t getTicks_ms(void);
void delay_ms(uint32_t wait_time_ms);

#endif /* SYSTICKDRIVER_H_ */
