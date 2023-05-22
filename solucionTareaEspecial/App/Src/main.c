/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : SOlucion tarea "ESpecial
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <stm32f4xx.h>

#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
#include "SysTickDriver.h"
#include "PwmDriver.h"


// Handler necesarios para el blinky
GPIO_Handler_t handlerLedBlinky = {0};
BasicTimer_Handler_t handlerTIM2 = {0};


//Cabecera de las funciones utilizadas en el desarrollo de la tarea
void initialConfig (void);


int main(void){

	initialConfig();
    while(1){
    }
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
}


void initialConfig (void){

	// Activamos el FPU
	//SCB->CPACR |= (0xF << 20);

	///Configuramos el pin A5 el cual se encargara del Led de estado

	handlerLedBlinky.pGPIOx                             = GPIOA;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinNumber      = PIN_5;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlinky);
	GPIO_WritePin(&handlerLedBlinky, SET);

	//Configuramos el TIM2, para asì poder lograr el parpadeo en el led de estado
	handlerTIM2.ptrTIMx 							= TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode				= BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_period				= 250;
	handlerTIM2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
	BasicTimer_Config(&handlerTIM2);

}


