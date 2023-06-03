/*
 * BasicTimer.c
 *
 *  Created on: Jun 1, 2023
 *      Author: anarrietam
 */

#include "BasicTimer.h"

// Este microcontrolado posee 6 timers, TIM1, TIM2, TIM3, TIM14, TIM16, TIM17
/* Variable que guarda la referencia del periférico que se esta utilizando*/
TIM_TypeDef	*ptrTimerUsed;

void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler){

	// Desactivamos las interrupciones
	__disable_irq();

    // Guardamos la referencia del timer usado
	ptrTimerUsed = ptrBTimerHandler->ptrTIMx;

	// Activamos la señal de reloj del periferico necesario
	if(ptrBTimerHandler->ptrTIMx  == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}else if( ptrBTimerHandler->ptrTIMx  == TIM2){
		RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;
	}else if (ptrBTimerHandler->ptrTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}else if (ptrBTimerHandler->ptrTIMx == TIM14){
		RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	}else if (ptrBTimerHandler->ptrTIMx == TIM16){
		RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	}else if (ptrBTimerHandler->ptrTIMx == TIM17){
		RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	}

	// 2. Configuramos el Pre-scaler
	ptrBTimerHandler->ptrTIMx->PSC = ptrBTimerHandler->TIMx_Config.TIMx_speed;

	/* 3. Configuramos la dirección del counter (up/down)*/
	if(ptrBTimerHandler->TIMx_Config.TIMx_mode == BTIMER_MODE_UP){

		/* 3a. Estamos en UP_Mode, el limite se carga en ARR y se comienza en 0 */
		// Configurar el registro que nos controla el modo up or down
		ptrBTimerHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

		/* 3b. Configuramos el Auto-reload. Este es el "limite" hasta donde el CNT va a contar */
		ptrBTimerHandler->ptrTIMx->ARR = ptrBTimerHandler->TIMx_Config.TIMx_period - 1;

		/* 3c. Reiniciamos el registro counter*/
		ptrBTimerHandler->ptrTIMx->CNT = 0;

	}else{	//		RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
		/* 3a. Estamos en DOWN_Mode, el limite se carga en ARR (0) y se comienza en un valor alto
		 * Trabaja contando en direccion descendente*/
		ptrBTimerHandler->ptrTIMx->CR1 |= TIM_CR1_DIR;

		/* 3b. Configuramos el Auto-reload. Este es el "limite" hasta donde el CNT va a contar
		 * En modo descendente, con numero positivos, cual es el minimo valor al que ARR puede llegar*/
		ptrBTimerHandler->ptrTIMx->ARR = 0;

		/* 3c. Reiniciamos el registro counter
		 * Este es el valor con el que el counter comienza */
		ptrBTimerHandler->ptrTIMx->CNT = ptrBTimerHandler->TIMx_Config.TIMx_period - 1;
	}
	/* 4. Activamos el Timer (el CNT debe comenzar a contar*/
	ptrBTimerHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;

	/* 5. Activamos la interrupción debida al Timerx Utilizado
	 * Modificar el registro encargado de activar la interrupcion generada por el TIMx*/
	ptrBTimerHandler->ptrTIMx->DIER |= TIM_DIER_UIE;

	/* 6. Activamos el canal del sistema NVIC para que lea la interrupción*/
	if(ptrBTimerHandler->ptrTIMx == TIM1){
		// Activando en NVIC para la interrupción del TIM1
		NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

	} else if(ptrBTimerHandler->ptrTIMx == TIM2){
		NVIC_EnableIRQ(TIM2_IRQn);

	} else if(ptrBTimerHandler->ptrTIMx == TIM3){
		NVIC_EnableIRQ(TIM3_IRQn);

	}else if(ptrBTimerHandler->ptrTIMx == TIM14){
		NVIC_EnableIRQ(TIM14_IRQn);

	}else if(ptrBTimerHandler->ptrTIMx == TIM16){
		NVIC_EnableIRQ(TIM16_IRQn);

	}else if(ptrBTimerHandler->ptrTIMx == TIM17){
		NVIC_EnableIRQ(TIM17_IRQn);

	}else{
		__NOP();
	}

	/* 7. Volvemos a activar las interrupciones del sistema */
	__enable_irq();

}

__attribute__((weak)) void BasicTimer1_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void BasicTimer2_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void BasicTimer3_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void BasicTimer14_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void BasicTimer16_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}

__attribute__((weak)) void BasicTimer17_Callback(void){
	  /* NOTE : This function should not be modified, when the callback is needed,
	            the BasicTimerX_Callback could be implemented in the main file
	   */
	__NOP();
}
/* Estas son las funciones a las que apunta el sistema en el vector de interrupciones.
 * Se debe utilizar usando exactamente el mismo nombre definido en el vector de interrupciones,
 * Al hacerlo correctamente, el sistema apunta a esta función y cuando la interrupción se lanza
 * el sistema inmediatamente salta a este lugar en la memoria*/
void TIM1_BRK_UP_TRG_COM_IRQ (void){
	TIM2->SR &= ~TIM_SR_UIF;
	BasicTimer1_Callback();

}

void TIM2_IRQHandler(void){
	/* Limpiamos la bandera que indica que la interrupción se ha generado */
	TIM2->SR &= ~TIM_SR_UIF;

	/* LLamamos a la función que se debe encargar de hacer algo con esta interrupción*/
	BasicTimer2_Callback();

}

void TIM3_IRQHandler(void){
	/* Limpiamos la bandera que indica que la interrupción se ha generado */
	TIM3->SR &= ~TIM_SR_UIF;

	/* LLamamos a la función que se debe encargar de hacer algo con esta interrupción*/
	BasicTimer3_Callback();

}

void TIM14_IRQHandler(void){
	/* Limpiamos la bandera que indica que la interrupción se ha generado */
	TIM14->SR &= ~TIM_SR_UIF;

	/* LLamamos a la función que se debe encargar de hacer algo con esta interrupción*/
	BasicTimer14_Callback();

}

void TIM16_IRQHandler(void){
	/* Limpiamos la bandera que indica que la interrupción se ha generado */
	TIM16->SR &= ~TIM_SR_UIF;

	/* LLamamos a la función que se debe encargar de hacer algo con esta interrupción*/
	BasicTimer16_Callback();

}

void TIM17_IRQHandler(void){
	/* Limpiamos la bandera que indica que la interrupción se ha generado */
	TIM17->SR &= ~TIM_SR_UIF;

	/* LLamamos a la función que se debe encargar de hacer algo con esta interrupción*/
	BasicTimer17_Callback();

}

