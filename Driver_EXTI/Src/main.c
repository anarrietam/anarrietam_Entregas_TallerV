/*
 * main.c
 *
 *  Created on: 23/03/2023
 *      Author: anarrietam
 */
#include <stdint.h>
#include <stm32f4xx.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include <ExtiDriver.h>

GPIO_Handler_t handler_Led2 = {0};
GPIO_Handler_t handler_UserButton = {0};
GPIO_Handler_t handlerDT = {0};
GPIO_Handler_t handlerSW = {0};
GPIO_Handler_t handlerCLK = {0};
BasicTimer_Handler_t  HandlerBlinkyTimer = {0};
uint32_t counterExti13 = 0;
EXTI_Config_t exti1 = {0};
EXTI_Config_t extiCount = {0};
uint32_t countNum2 = 0;

void callback_exti13 (void);
void EXTI15_10_IRQHandler(void);
void BasicTimer2_Callback(void);
void init_Hardware(void);

void init_Hardware(void){
	//Se trabajara en el puerto GPIOA
	handler_Led2.pGPIOx                             = GPIOA               ;
	handler_Led2.GPIO_PinConfig.GPIO_PinNumber      = PIN_5               ;
	handler_Led2.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT       ;
	handler_Led2.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL ;
	handler_Led2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING  ;
	handler_Led2.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST 	  ;
	handler_Led2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING   ;

	GPIO_Config(&handler_Led2);
	GPIO_WritePin(&handler_Led2, SET);


	HandlerBlinkyTimer.ptrTIMx = TIM2;
	HandlerBlinkyTimer.TIMx_Config.TIMx_mode  =BTIMER_MODE_UP;
	HandlerBlinkyTimer.TIMx_Config.TIMx_speed = BTIMER_SPEED_1ms;
	HandlerBlinkyTimer.TIMx_Config.TIMx_period = 250;
	HandlerBlinkyTimer.TIMx_Config.TIMx_interruptEnable = BTIMER_INTERRUPT_ENABLE;

	BasicTimer_Config(&HandlerBlinkyTimer);
	handler_UserButton.pGPIOx                             = GPIOC;
	handler_UserButton.GPIO_PinConfig.GPIO_PinNumber      = PIN_13              ;
	handler_UserButton.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN      	 ;
	handler_UserButton.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL ;
	handler_UserButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING  ;

	GPIO_Config(&handler_UserButton);

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


	extiCount.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	extiCount.pGPIOHandler = &handlerDT;
	extInt_Config(&extiCount);



}


void callback_extInt13 (void){
	counterExti13++;
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handler_Led2);
}

void callbackextInt2 (void){
	if (GPIO_ReadPin(&handlerDT)  == 1){
		countNum2 ++;
		counterExti13++;
	}
}

int main (void){
	init_Hardware();
	while (1){
	}
	return 0;
}

