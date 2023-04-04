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

GPIO_Handler_t handler_Led2 = {0};
GPIO_Handler_t handler_UserButton = {0};
BasicTimer_Handler_t  HandlerBlinkyTimer = {0};
uint32_t counterExti13 = 0;

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

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR1_EXTI3_Pos);
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->FTSR = 0;
	EXTI->RTSR = 0;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->IMR = 0;
	EXTI->IMR |= EXTI_IMR_IM13;

	__disable_irq();

	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();
}

void EXTI15_10_IRQHandler(void){
	if ((EXTI->PR & EXTI_PR_PR13) !=0){
		EXTI->PR |= EXTI_PR_PR13;
		callback_exti13();
	}
}

void callback_exti13 (void){
	counterExti13++;
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handler_Led2);
}


int main (void){
	init_Hardware();
	while (1){
	}
	return 0;
}

