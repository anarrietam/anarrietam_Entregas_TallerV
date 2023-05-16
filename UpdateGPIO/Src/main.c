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
GPIO_Handler_t handlerCLK = {0};
GPIO_Handler_t handlerDT ={0};
GPIO_Handler_t handlerSW = {0};

uint8_t counteDT = 0;
uint8_t counterCLK = 0;
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

	handlerCLK.pGPIOx                                  = GPIOC;
	handlerCLK.GPIO_PinConfig.GPIO_PinNumber           = PIN_1;
	handlerCLK.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerCLK.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerCLK.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerCLK);

	handlerDT.pGPIOx                                  = GPIOC;
	handlerDT.GPIO_PinConfig.GPIO_PinNumber           = PIN_2;
	handlerDT.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerDT.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerDT.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerDT);

	handlerSW.pGPIOx                                  = GPIOC;
	handlerSW.GPIO_PinConfig.GPIO_PinNumber           = PIN_3;
	handlerSW.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerSW.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerSW.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerSW);

	GPIO_Config(&handlerLed);
	GPIO_WritePin(&handlerLed, SET);

	if(GPIO_ReadPin(&handlerDT) == 1){
				counteDT++;
			}else if(GPIO_ReadPin(&handlerCLK)==1){
				counterCLK++;
			}

    /* Loop forever */
	while (1){

	}
	return 0;
}
