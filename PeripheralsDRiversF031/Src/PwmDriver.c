/*
 * PwmDriver.c
 *
 *  Created on: Jun 5, 2023
 *      Author: andres
 */

#include "PwmDriver.h"

void pwm_Config(PWM_Handler_t *ptrPwmHandler) {

	// Activamos la señal de reloj del periferico necesario
	if (ptrPwmHandler->ptrTIMx == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	} else if (ptrPwmHandler->ptrTIMx == TIM2) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	} else if (ptrPwmHandler->ptrTIMx == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	} else if (ptrPwmHandler->ptrTIMx == TIM14) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	} else if (ptrPwmHandler->ptrTIMx == TIM16) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	} else if (ptrPwmHandler->ptrTIMx == TIM17) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	}

	/* 1. Cargamos la frecuencia deseada */
	setFrequency(ptrPwmHandler);

	/* 2. Cargamos el valor del dutty-Cycle*/
	setDuttyCycle(ptrPwmHandler);

	/* 2a. Estamos en UP_Mode, el limite se carga en ARR y se comienza en 0 */
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

	// Activamos el auto-reload preload enable
	ptrPwmHandler->ptrTIMx->CR1 |= TIM_CR1_ARPE;

	// Inicializamos los registros
	ptrPwmHandler->ptrTIMx->EGR |= TIM_EGR_UG;

	// Activamos el registro MOE
	ptrPwmHandler->ptrTIMx->BDTR |= TIM_BDTR_MOE;


	/* 3. Configuramos los bits CCxS del registro TIMy_CCMR1, de forma que sea modo salida
	 * (para cada canal hay un conjunto CCxS)
	 *
	 * 4. Además, en el mismo "case" podemos configurar el modo del PWM, su polaridad...
	 *
	 * 5. Y además activamos el preload bit, para que cada vez que exista un update-event
	 * el valor cargado en el CCRx será recargado en el registro "shadow" del PWM */

	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {

		// Se apaga el canal para poder escribir sobre él
		ptrPwmHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC1E);

		// Seleccionamos como salida el canal
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);

		// Configuramos el canal como PWM
		ptrPwmHandler->ptrTIMx->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);

		// Activamos la funcionalidad de pre-load
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1PE;


		break;
	}

	case PWM_CHANNEL_2: {

		// Se apaga el canal para poder escribir sobre él
		ptrPwmHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC2E);

		// Seleccionamos como salida el canal
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~(TIM_CCMR1_CC2S);

		// Configuramos el canal como PWM
		ptrPwmHandler->ptrTIMx->CCMR1 |= (0b110 << TIM_CCMR1_OC2M_Pos);

		// Activamos la funcionalidad de pre-load
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2PE;
		break;
	}

	case PWM_CHANNEL_3: {

		// Se apaga el canal para poder escribir sobre él
		ptrPwmHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC3E);

		// Seleccionamos como salida el canal
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~(TIM_CCMR2_CC3S);

		// Configuramos el canal como PWM
		ptrPwmHandler->ptrTIMx->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos);

		// Activamos la funcionalidad de pre-load
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC3PE;
		break;
	}

	case PWM_CHANNEL_4: {

		// Se apaga el canal para poder escribir sobre él
		ptrPwmHandler->ptrTIMx->CCER &= ~(TIM_CCER_CC4E);

		// Seleccionamos como salida el canal
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~(TIM_CCMR2_CC4S);

		// Configuramos el canal como PWM
		ptrPwmHandler->ptrTIMx->CCMR2 |= (0b110 << TIM_CCMR2_OC4M_Pos);

		// Activamos la funcionalidad de pre-load
		ptrPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC4PE;
		break;
	}

	default: {
		break;
	}


	} // fin del switch-case

//	ptrPwmHandler->ptrTIMx->BDTR |= TIM_BDTR_MOE;

	/* 6. Activamos la salida seleccionada */
	enableOutput(ptrPwmHandler);
}

/* Función para activar el Timer y activar todo el módulo PWM */
void startPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	ptrPwmHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;
}

/* Función para desactivar el Timer y detener todo el módulo PWM*/
void stopPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_CEN;
}

/* Función encargada de activar cada uno de los canales con los que cuenta el TimerX */
void enableOutput(PWM_Handler_t *ptrPwmHandler) {
	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {
		// Activamos la salida del canal 1
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC1E;
		break;
	}

	case PWM_CHANNEL_2: {
		// Activamos la salida del canal 2
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC2E;
		break;
	}

	case PWM_CHANNEL_3: {
		// Activamos la salida del canal 2
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC3E;

		break;
	}

	case PWM_CHANNEL_4: {
		// Activamos la salida del canal 2
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC4E;

		break;
	}

	default: {
		break;
	}
	}
}

/*
 * La frecuencia es definida por el conjunto formado por el preescaler (PSC)
 * y el valor límite al que llega el Timer (ARR), con estos dos se establece
 * la frecuencia.
 * */
void setFrequency(PWM_Handler_t *ptrPwmHandler) {

	// Cargamos el valor del prescaler, nos define la velocidad (en ns) a la cual
	// se incrementa el Timer
	ptrPwmHandler->ptrTIMx->PSC = ptrPwmHandler->config.prescaler - 1;

	// Cargamos el valor del ARR, el cual es el límite de incrementos del Timer
	// antes de hacer un update y reload.
	ptrPwmHandler->ptrTIMx->ARR = ptrPwmHandler->config.periodo;
}

/* Función para actualizar la frecuencia, funciona de la mano con setFrequency */
void updateFrequency(PWM_Handler_t *ptrPwmHandler, uint16_t newFreq) {
	// Actualizamos el registro que manipula el periodo
	ptrPwmHandler->config.periodo = newFreq;

	// Llamamos a la fucnión que cambia la frecuencia
	setFrequency(ptrPwmHandler);
}

/* El valor del dutty debe estar dado en valores de %, entre 0% y 100%*/
void setDuttyCycle(PWM_Handler_t *ptrPwmHandler) {

	// Seleccionamos el canal para configurar su dutty
	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.duttyCicle;

		break;
	}

	case PWM_CHANNEL_2: {
		ptrPwmHandler->ptrTIMx->CCR2 = ptrPwmHandler->config.duttyCicle;
		break;
	}

	case PWM_CHANNEL_3: {
		ptrPwmHandler->ptrTIMx->CCR3 = ptrPwmHandler->config.duttyCicle;
		break;
	}

	case PWM_CHANNEL_4: {
		ptrPwmHandler->ptrTIMx->CCR4 = ptrPwmHandler->config.duttyCicle;
		break;
	}

	default: {
		break;
	}

	}		// fin del switch-case

}

/* Función para actualizar el Dutty, funciona de la mano con setDuttyCycle */
void updateDuttyCycle(PWM_Handler_t *ptrPwmHandler, uint16_t newDutty) {
	// Actualizamos el registro que manipula el dutty
	ptrPwmHandler->config.duttyCicle = newDutty;

	// Llamamos a la fucnión que cambia el dutty y cargamos el nuevo valor
	setDuttyCycle(ptrPwmHandler);
}




