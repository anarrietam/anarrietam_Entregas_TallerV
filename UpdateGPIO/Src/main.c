/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Proyecto base
 ******************************************************************************
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include <GPIOxDriver.h>

// Definicion de elementos para trabajar

GPIO_Handler_t handlerLed = {0};

/**
 * Funcion principal del programa
 */

int main(void){

	handlerLed.pGPIOx   = GPIOA;
	handlerLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	handlerLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	GPIO_Config(&handlerLed);
	GPIO_WritePin(&handlerLed, SET);

    /* Loop forever */
	while (1){

		GPIOxTooglePin(&handlerLed);

		for (int i=0; i<2000000 ; i++);

	}
	return 0;
}
