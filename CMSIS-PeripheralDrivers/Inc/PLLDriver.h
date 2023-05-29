/*
 * PLLDriver.h
 *
 *  Created on: May 29, 2023
 *      Author: andres
 */

#ifndef PLLDRIVER_H_
#define PLLDRIVER_H_

#define PLL_80MHz_Frequency   1
#define PLL_16MHz_Frequency   0


void configPll(void);
uint8_t getConfigPLL(void);

#endif /* PLLDRIVER_H_ */
