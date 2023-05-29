/*
 * PLLDriver.c
 *
 *  Created on: May 20, 2023
 *      Author: anarrietam
 */

#include <stm32f4xx.h>
#include "PLLDriver.h"

uint8_t confPLL = 0;

/* Configuramos el PLL */
void configPll(void){

	// Seleccionamos el HSI clock como PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);

	// Limpiamos el registro
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);

	// Ponemos un 8 en el PLLM como divisor, ya que este al dividir al
	// valor de PLL input ( 16 MHz), no da un valor de VCO input recomendado ( 2 MHz)
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLM_3);


	// Ponemos PLLN en 80MHz, como tenemo un valor de VCO Input en 2 Mhz, configuramos el PLLN
	// para que sea 80 MHz ( dentro del rango recomendado), haciendo que el valor VCO Ouput sea de 160 MHz

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);  // Limpiamos el registro
	RCC->PLLCFGR |= (80 << RCC_PLLCFGR_PLLN_Pos);


	//Se configura PLLP en 2, para que ya al hacer la ultima operacion la cual nos da el valor de
	// PLL output clock frequency este sea el esperado ( 80MHz )
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);

	/* Cambiamos la latencia  en FLASH_ACR para que el micro espere un poco para entrar la información en los registros */
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);  //Limpiamos el registro
	/* 3WS de latencia */
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

	//Seleccionamos la señal PLL

	RCC->CFGR |= ( 3 << RCC_CFGR_MCO1_Pos);

	// Encender el PLL
	RCC->CR |= RCC_CR_PLLON;

	// Esperamos que el PLL se cierre (estabilizacion)
	while(!(RCC->CR & RCC_CR_PLLRDY)){
		__NOP();
	}

	//Cambiamos el CPU Clock source cambiando los SW bits (System clock switch)
	RCC->CFGR &= ~(RCC_CFGR_SW);//LImpiamos el registro
	RCC->CFGR |= (RCC_CFGR_SW_1);

	// El bus APB1 al no poder superar los 50 MHz, hay quee aplicarles un prescaler
	RCC->CFGR &= ~RCC_CFGR_PPRE1; //LImpiamos el registro
	RCC->CFGR |= RCC_CFGR_PPRE1_2; // Dividimos entre 2 para si trabjar en este bus con una frecuencia de 40MHz


}

uint8_t getConfigPLL(void){

	if (RCC->CR & RCC_CR_PLLON){
		confPLL = PLL_80MHz_Frequency;
	} else {
		confPLL = PLL_16MHz_Frequency;
	}

	return confPLL;
}

