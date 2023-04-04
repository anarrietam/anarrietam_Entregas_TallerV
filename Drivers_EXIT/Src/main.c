/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Timers
 ******************************************************************************
 *
 ******************************************************************************
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include "GPIOxDriver.h"
#include <BasicTimer.h>

// Definición de elementos
GPIO_Handler_t handlerLed2 ={0};	//PA5
BasicTimer_Handler_t handlerTimer2 = {0};  // TIM2
GPIO_Handler_t handlerUserButton =  {0};
uint16_t counterExti13 = 0;

void BasicTimerX_Callback(void);
void EXTI15_10_IRQHandler(void);
void callback_exti13(void);

/**
 * Función principal del programa.
 */
int main(void){

	// Configurando el LED_2 -> PA5
	handlerLed2.pGPIOx 									= GPIOA;
	handlerLed2.GPIO_PinConfig.GPIO_PinNumber			= PIN_5;
	handlerLed2.GPIO_PinConfig.GPIO_PinMode				= GPIO_MODE_OUT;
	handlerLed2.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLed2.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_NOTHING;
	handlerLed2.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;

	// Seleccionar el timer que se quiere usar
	handlerTimer2.ptrTIMx 								= TIM2;
	// Configuración con la cual se maneja el timer
	handlerTimer2.TIMx_Config.TIMx_mode					= BTIMER_MODE_UP;
	handlerTimer2.TIMx_Config.TIMx_period				= 200;
	handlerTimer2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
	//handlerTimer2.TIMx_Config.TIMx_interruptEnable      = BTIMER_INTERRUP_ENABLE;


	// Cargamos la configuración del pinA5
	GPIO_Config(&handlerLed2);
	GPIO_WritePin(&handlerLed2, SET);

	// Cargamos la configuración del timer
	BasicTimer_Config(&handlerTimer2);

	//Configuramos el PIN C13 como entrada
	handlerUserButton.pGPIOx                            = GPIOC;
	handlerUserButton.GPIO_PinConfig.GPIO_PinNumber     = 13;
	handlerUserButton.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_OUT;
	handlerUserButton.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_PUPDR_NOTHING;

	//Cargamos la configuracion del UserButton
	GPIO_Config(&handlerUserButton);

	// Activamos la señal de reloj para el SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Configurar el multiplexor 13, asignado para el puerto GPIOC
	SYSCFG->EXTICR[3] &= -(0xF << SYSCFG_EXTICR4_EXTI13_Pos);
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	//Configurar el EXTI
	EXTI->FTSR = 0;
	EXTI->RTSR = 0;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->IMR = 0;
	EXTI->IMR |= EXTI_IMR_IM13;

	//DESACTIVAR LAS INTERRUPCCIONES GLOBALES
	__disable_irq();

	// Incluir la interrupcion en el NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	//Activamos las interrupciones globales
	__enable_irq();



    /* Loop forever */
	while(1){


	}
}

void EXTI15_10_IRQHandler(void){
	// Verificamos la interrupcion
	if((EXTI->PR & EXTI_PR_PR13) != 0){
		//Bajamos la bandera
		EXTI->PR |= EXTI_PR_PR13;
		callback_exti13();
	}
}

void callback_exti13(void){
	counterExti13 ++;
}

void BasicTimerX_Callback(void){
	GPIOxTooglePin(&handlerLed2);
}
