/*
 * PLLDriver.h
 *
 *  Created on: May 29, 2023
 *      Author: anarrietam
 */

#ifndef PLLDRIVER_H_
#define PLLDRIVER_H_

#define PLL_100MHz_Frequency  2
#define PLL_80MHz_Frequency   1
#define PLL_16MHz_Frequency   0

// Se crea una variable de typedef struct que nos ayudara en la configuracion del timer

typedef struct
{
	uint8_t PLL_Frequency; // se elige la frecuencia del PLL entre 80 y 100 MHz

}PLL_Handler;

void configPll(PLL_Handler *ptrPLLHandler);
uint8_t getConfigPLL(void);
void configHSI(void);

#endif /* PLLDRIVER_H_ */
