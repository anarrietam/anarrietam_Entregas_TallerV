/*
 * ExtiDriver.c
 *
 *  Created on: Jun 2, 2023
 *      Author: andres
 */

#include "ExtiDriver.h"
#include "GPIOxDriver.h"

void extInt_Config(EXTI_Config_t *extiConfig){

	/* 1.0 Se carga la configuración, que debe ser el PINx como entrada "simple" */
	GPIO_Config(extiConfig->pGPIOHandler);

	// Activamos el accesos al SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

	switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {

	case 0: {
			/* SYSCFG_EXTICR1 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI0_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_0, ó GPIOB_0, ó GPIOC_0, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PF);

			} else {
				__NOP();
			}

			break;
		}


		/* Configurando para el todos los pines GPIOX_1*/
		case 1: {
			/* SYSCFG_EXTICR1 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI1_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_1, ó GPIOB_1, ó GPIOC_1, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_2*/
		case 2: {
			/* SYSCFG_EXTICR1 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI2_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_2, ó GPIOB_2, ó GPIOC_2, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_3*/
		case 3: {
			/* SYSCFG_EXTICR1 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI3_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_3, ó GPIOB_3, ó GPIOC_3, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PC);

			}  else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PF);

			}else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_4*/
		case 4: {
			/* SYSCFG_EXTICR2 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI4_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_4, ó GPIOB_4, ó GPIOC_4, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PC);

			}  else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PF);

			}  else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_5*/
		case 5: {
			/* SYSCFG_EXTICR2 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI5_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_5, ó GPIOB_5, ó GPIOC_5, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PC);

			}  else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_6*/
		case 6: {
			/* SYSCFG_EXTICR2 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI6_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_6, ó GPIOB_6, ó GPIOC_6, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_7*/
		case 7: {
			/* SYSCFG_EXTICR2 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI7_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_7, ó GPIOB_7, ó GPIOC_7, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PC);

			}else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_8*/
		case 8: {
			/* SYSCFG_EXTICR3 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI8_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_8, ó GPIOB_8, ó GPIOC_8, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_9*/
		case 9: {
			/* SYSCFG_EXTICR3 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI9_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_9, ó GPIOB_9, ó GPIOC_9, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_10*/
		case 10: {
			/* SYSCFG_EXTICR3 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI10_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_10, ó GPIOB_10, ó GPIOC_10, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_11*/
		case 11: {
			/* SYSCFG_EXTICR3 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI11_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_11, ó GPIOB_11, ó GPIOC_11, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_12*/
		case 12: {
			/* SYSCFG_EXTICR4 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI12_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_12, ó GPIOB_12, ó GPIOC_12, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_13*/
		case 13: {
			/* SYSCFG_EXTICR4 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI13_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_13, ó GPIOB_13, ó GPIOC_13, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_14*/
		case 14: {
			/* SYSCFG_EXTICR4 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI14_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_14, ó GPIOB_14, ó GPIOC_14, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PC);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PF);

			} else {
				__NOP();
			}

			break;
		}

		/* Configurando para el todos los pines GPIOX_15 */
		case 15: {
			/* SYSCFG_EXTICR4 */
			// Limpiamos primero la posición que deseamos configurar
			SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI15_Pos);

			// Ahora seleccionamos el valor a cargar en la posición, segun sea la selección
			// del puerto que vamos a utilizar: GPIOA_0, ó GPIOB_0, ó GPIOC_0, etc
			if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PA);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PB);

			} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PC);

			}else if (extiConfig->pGPIOHandler->pGPIOx == GPIOF) {
				SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PF);

			} else {
				__NOP();
			}
			break;
		}

	default: {
			__NOP();
			break;
		}
		}
	/* 4.0 Seleccionamos el tipo de flanco */
		if(extiConfig->edgeType == EXTERNAL_INTERRUPT_FALLING_EDGE){
			/* Falling Trigger selection register*/
			switch(extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber){
			case 0:{
				EXTI->RTSR &= ~EXTI_RTSR_RT0; // Ponemos el rising en reset value (0),ya que no será usado.
				EXTI->FTSR &= ~EXTI_FTSR_FT0; // Limpiamos el registro para el bit FT necesitado.
				EXTI->FTSR |= EXTI_FTSR_FT0; // Dejamos el bit necesitado del FT activado.
				break;
			}
			case 1:{
				EXTI->RTSR &= ~EXTI_RTSR_RT1; // Ponemos el rising en reset value (0),ya que no será usado.
				EXTI->FTSR &= ~EXTI_FTSR_FT1; // Limpiamos el registro para el bit FT necesitado.
				EXTI->FTSR |= EXTI_FTSR_FT1; // Dejamos el bit necesitado del FT activado.
				break;
			}
			case 2:{
				EXTI->RTSR &= ~EXTI_RTSR_RT2; // Ponemos el rising en reset value (0),ya que no será usado.
				EXTI->FTSR &= ~EXTI_FTSR_FT2; // Limpiamos el registro para el bit FT necesitado.
				EXTI->FTSR |= EXTI_FTSR_FT2; // Dejamos el bit necesitado del FT activado.
				break;
			}
			case 3:{
				EXTI->RTSR &= ~EXTI_RTSR_RT3;
				EXTI->FTSR &= ~EXTI_FTSR_FT3;
				EXTI->FTSR |= EXTI_FTSR_FT3;
				break;
			}
			case 4:{
				EXTI->RTSR &= ~EXTI_RTSR_RT4;
				EXTI->FTSR &= ~EXTI_FTSR_FT4;
				EXTI->FTSR |= EXTI_FTSR_FT4;
				break;
			}
			case 5:{
				EXTI->RTSR &= ~EXTI_RTSR_RT5;
				EXTI->FTSR &= ~EXTI_FTSR_FT5;
				EXTI->FTSR |= EXTI_FTSR_FT5;
				break;
			}
			case 6:{
				EXTI->RTSR &= ~EXTI_RTSR_RT6;
				EXTI->FTSR &= ~EXTI_FTSR_FT6;
				EXTI->FTSR |= EXTI_FTSR_FT6;
				break;
			}
			case 7:{
				EXTI->RTSR &= ~EXTI_RTSR_RT7;
				EXTI->FTSR &= ~EXTI_FTSR_FT7;
				EXTI->FTSR |= EXTI_FTSR_FT7;
				break;
			}
			case 8:{
				EXTI->RTSR &= ~EXTI_RTSR_RT8;
				EXTI->FTSR &= ~EXTI_FTSR_FT8;
				EXTI->FTSR |= EXTI_FTSR_FT8;
				break;
			}
			case 9:{
				EXTI->RTSR &= ~EXTI_RTSR_RT9;
				EXTI->FTSR &= ~EXTI_FTSR_FT9;
				EXTI->FTSR |= EXTI_FTSR_FT9;
				break;
			}
			case 10:{
				EXTI->RTSR &= ~EXTI_RTSR_RT10;
				EXTI->FTSR &= ~EXTI_FTSR_FT10;
				EXTI->FTSR |= EXTI_FTSR_FT10;
				break;
			}
			case 11:{
				EXTI->RTSR &= ~EXTI_RTSR_RT11;
				EXTI->FTSR &= ~EXTI_FTSR_FT11;
				EXTI->FTSR |= EXTI_FTSR_FT11;
				break;
			}
			case 12:{
				EXTI->RTSR &= ~EXTI_RTSR_RT12;
				EXTI->FTSR &= ~EXTI_FTSR_FT12;
				EXTI->FTSR |= EXTI_FTSR_FT12;
				break;
			}
			case 13:{
				EXTI->RTSR &= ~EXTI_RTSR_RT13;
				EXTI->FTSR &= ~EXTI_FTSR_FT13;
				EXTI->FTSR |= EXTI_FTSR_FT13;
				break;
			}
			case 14:{
				EXTI->RTSR &= ~EXTI_RTSR_RT14;
				EXTI->FTSR &= ~EXTI_FTSR_FT14;
				EXTI->FTSR |= EXTI_FTSR_FT14;
				break;
			}
			case 15:{
				EXTI->RTSR &= ~EXTI_RTSR_RT15;
				EXTI->FTSR &= ~EXTI_FTSR_FT15;
				EXTI->FTSR |= EXTI_FTSR_FT15;
				break;
			}
			default: {
				__NOP();
				break;
			}
			}
		}
		else{
			/* Rising Trigger selection register*/
			switch(extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber){
			case 0:{
				EXTI->FTSR &= ~EXTI_FTSR_FT0; // Ponemos el falling en reset value (0),ya que no será usado.
				EXTI->RTSR &= ~EXTI_RTSR_RT0; // Limpiamos el registro para el bit TR necesitado.
				EXTI->RTSR |= EXTI_RTSR_RT0; // Dejamos el bit necesitado del TR activado.
				break;
			}
			case 1:{
				EXTI->FTSR &= ~EXTI_FTSR_FT1;
				EXTI->RTSR &= ~EXTI_RTSR_RT1;
				EXTI->RTSR |= EXTI_RTSR_RT1;
				break;
			}
			case 2:{
				EXTI->FTSR &= ~EXTI_FTSR_FT2;
				EXTI->RTSR &= ~EXTI_RTSR_RT2;
				EXTI->RTSR |= EXTI_RTSR_RT2;
				break;
			}
			case 3:{
				EXTI->FTSR &= ~EXTI_FTSR_FT3;
				EXTI->RTSR &= ~EXTI_RTSR_RT3;
				EXTI->RTSR |= EXTI_RTSR_RT3;
				break;
			}
			case 4:{
				EXTI->FTSR &= ~EXTI_FTSR_FT4;
				EXTI->RTSR &= ~EXTI_RTSR_RT4;
				EXTI->RTSR |= EXTI_RTSR_RT4;
				break;
			}
			case 5:{
				EXTI->FTSR &= ~EXTI_FTSR_FT5;
				EXTI->RTSR &= ~EXTI_RTSR_RT5;
				EXTI->RTSR |= EXTI_RTSR_RT5;
				break;
			}
			case 6:{
				EXTI->FTSR &= ~EXTI_FTSR_FT6;
				EXTI->RTSR &= ~EXTI_RTSR_RT6;
				EXTI->RTSR |= EXTI_RTSR_RT6;
				break;
			}
			case 7:{
				EXTI->FTSR &= ~EXTI_FTSR_FT7;
				EXTI->RTSR &= ~EXTI_RTSR_RT7;
				EXTI->RTSR |= EXTI_RTSR_RT7;
				break;
			}
			case 8:{
				EXTI->FTSR &= ~EXTI_FTSR_FT8;
				EXTI->RTSR &= ~EXTI_RTSR_RT8;
				EXTI->RTSR |= EXTI_RTSR_RT8;
				break;
			}
			case 9:{
				EXTI->FTSR &= ~EXTI_FTSR_FT9;
				EXTI->RTSR &= ~EXTI_RTSR_RT9;
				EXTI->RTSR |= EXTI_RTSR_RT9;
				break;
			}
			case 10:{
				EXTI->FTSR &= ~EXTI_FTSR_TR10;
				EXTI->RTSR &= ~EXTI_RTSR_TR10;
				EXTI->RTSR |= EXTI_RTSR_TR10;
				break;
			}
			case 11:{
				EXTI->FTSR &= ~EXTI_FTSR_FT11;
				EXTI->RTSR &= ~EXTI_RTSR_RT11;
				EXTI->RTSR |= EXTI_RTSR_RT11;
				break;
			}
			case 12:{
				EXTI->FTSR &= ~EXTI_FTSR_FT12;
				EXTI->RTSR &= ~EXTI_RTSR_RT12;
				EXTI->RTSR |= EXTI_RTSR_RT12;
				break;
			}
			case 13:{
				EXTI->FTSR &= ~EXTI_FTSR_FT13;
				EXTI->RTSR &= ~EXTI_RTSR_RT13;
				EXTI->RTSR |= EXTI_RTSR_RT13;
				break;
			}
			case 14:{
				EXTI->FTSR &= ~EXTI_FTSR_FT14;
				EXTI->RTSR &= ~EXTI_RTSR_RT14;
				EXTI->RTSR |= EXTI_RTSR_RT14;
				break;
			}
			case 15:{
				EXTI->FTSR &= ~EXTI_FTSR_FT15;
				EXTI->RTSR &= ~EXTI_RTSR_RT15;
				EXTI->RTSR |= EXTI_RTSR_RT15;
				break;
			}
			default: {
				__NOP();
				break;
			}
			}
		}

		/* 5.0 Desactivo primero las interrupciones globales */
		__disable_irq();

		/* 6.0 Activamos la interrupción del canal que estamos configurando */
		// Interrupt Mask register
		switch(extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber){
		case 0: {
			EXTI->IMR &= ~EXTI_IMR_IM0;
			EXTI->IMR |= EXTI_IMR_IM0;
			break;
		}
		case 1: {
			EXTI->IMR &= ~EXTI_IMR_IM1;
			EXTI->IMR |= EXTI_IMR_IM1;
			break;
		}
		case 2: {
			EXTI->IMR &= ~EXTI_IMR_IM2;
			EXTI->IMR |= EXTI_IMR_IM2;
			break;
		}
		case 3: {
			EXTI->IMR &= ~EXTI_IMR_IM3;
			EXTI->IMR |= EXTI_IMR_IM3;
			break;
		}
		case 4: {
			EXTI->IMR &= ~EXTI_IMR_IM4;
			EXTI->IMR |= EXTI_IMR_IM4;
			break;
		}
		case 5: {
			EXTI->IMR &= ~EXTI_IMR_IM5;
			EXTI->IMR |= EXTI_IMR_IM5;
			break;
		}
		case 6: {
			EXTI->IMR &= ~EXTI_IMR_IM6;
			EXTI->IMR |= EXTI_IMR_IM6;
			break;
		}
		case 7: {
			EXTI->IMR &= ~EXTI_IMR_IM7;
			EXTI->IMR |= EXTI_IMR_IM7;
			break;
		}
		case 8: {
			EXTI->IMR &= ~EXTI_IMR_IM8;
			EXTI->IMR |= EXTI_IMR_IM8;
			break;
		}
		case 9: {
			EXTI->IMR &= ~EXTI_IMR_IM9;
			EXTI->IMR |= EXTI_IMR_IM9;
			break;
		}
		case 10: {
			EXTI->IMR &= ~EXTI_IMR_IM10;
			EXTI->IMR |= EXTI_IMR_IM10;
			break;
		}
		case 11: {
			EXTI->IMR &= ~EXTI_IMR_IM11;
			EXTI->IMR |= EXTI_IMR_IM11;
			break;
		}
		case 12: {
			EXTI->IMR &= ~EXTI_IMR_IM12;
			EXTI->IMR |= EXTI_IMR_IM12;
			break;
		}
		case 13: {
			EXTI->IMR &= ~EXTI_IMR_IM13;
			EXTI->IMR |= EXTI_IMR_IM13;
			break;
		}
		case 14: {
			EXTI->IMR &= ~EXTI_IMR_IM14;
			EXTI->IMR |= EXTI_IMR_IM14;
			break;
		}
		case 15: {
			EXTI->IMR &= ~EXTI_IMR_IM15;
			EXTI->IMR |= EXTI_IMR_IM15;
			break;
		}
		default: {
			__NOP();
			break;
		}
		}

		/* 6.1 Matriculamos la interrupción en el NVIC para el canal correspondiente,
		 * donde el canal 0 corresponde al EXTI_0, canal 1 al EXTI_1, etc.
		 *
		 * NOTA: Observar que algunos canales EXTI comparten un mismo vector de interrupción
		 * */
		switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {
		case 0: {
			__NVIC_EnableIRQ(EXTI0_IRQn);
			break;
		}

		case 1: {
			__NVIC_EnableIRQ(EXTI1_IRQn);
			break;
		}

		case 2: {
			__NVIC_EnableIRQ(EXTI2_IRQn);
			break;
		}

		case 3: {
			__NVIC_EnableIRQ(EXTI3_IRQn);
			break;
		}

		case 4: {
			__NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		}

		case 5: {
			__NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}

		case 6: {
			__NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}

		case 7: {
			__NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}

		case 8: {
			__NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}

		case 9: {
			__NVIC_EnableIRQ(EXTI9_5_IRQn);
			break;
		}

		case 10: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		case 11: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		case 12: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		case 13: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		case 14: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		case 15: {
			__NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
		}

		default: {
			break;
		}

		}

		/* 7.0 Volvemos a activar las interrupciones globales */
		__enable_irq();
	}


	/**/
	__attribute__ ((weak)) void callback_extInt0(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt1(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt2(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt3(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt4(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt5(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt6(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt7(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt8(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt9(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt10(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt11(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt12(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt13(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt14(void){
		__NOP();
	}
	__attribute__ ((weak)) void callback_extInt15(void){
		__NOP();
	}

	/* ISR de la interrupción canal 0*/
	void EXTI0_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_0 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR0){
			EXTI->PR |= EXTI_PR_PR0; // Bajamos la bandera correspondiente
			callback_extInt0(); // llamamos al callback
		}
	}
	void EXTI1_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_1 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR1){
			EXTI->PR |= EXTI_PR_PR1;
			callback_extInt1();
		}
	}
	void EXTI2_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_2 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR2){
			EXTI->PR |= EXTI_PR_PR2;
			callback_extInt2();
		}
	}
	void EXTI3_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_3 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR3){
			EXTI->PR |= EXTI_PR_PR3;
			callback_extInt3();
		}
	}
	void EXTI4_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_4 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR4){
			EXTI->PR |= EXTI_PR_PR4;
			callback_extInt4();
		}
	}
	/* ISR de la interrupción canales 9_5
	 * Observe que debe agregar totos los posibles casos, los cuales
	 * son identificados por un bloque if() y el analisis de la bandera
	 * (pending register -> EXTI_PR)
	 */
	void EXTI9_5_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_5 del GPIO_X
		if(EXTI->PR & EXTI_PR_PR5){
			EXTI->PR |= EXTI_PR_PR5; // Bajamos la bandera correspondiente
			callback_extInt5(); // llamamos al callback
		}
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_6 del GPIO_X
		else if(EXTI->PR & EXTI_PR_PR6){
			EXTI->PR |= EXTI_PR_PR6;
			callback_extInt6();
		}
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_7 del GPIO_X
		else if(EXTI->PR & EXTI_PR_PR7){
			EXTI->PR |= EXTI_PR_PR7;
			callback_extInt7();
		}
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_8 del GPIO_X
		else if(EXTI->PR & EXTI_PR_PR8){
			EXTI->PR |= EXTI_PR_PR8;
			callback_extInt8();
		}
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_9 del GPIO_X
		else if(EXTI->PR & EXTI_PR_PR9){
			EXTI->PR |= EXTI_PR_PR9;
			callback_extInt9();
		}
	}
	/* ISR de la interrupción canales 15_10
	 */
	void EXTI15_10_IRQHandler(void){
		// Evaluamos si la interrupción que se lanzo corresponde al PIN_Y del GPIO_X
		if(EXTI->PR & EXTI_PR_PR10){
			EXTI->PR |= EXTI_PR_PR10; // Bajamos la bandera correspondiente
			callback_extInt10(); // Llamamos al callback
		}
	    else if(EXTI->PR & EXTI_PR_PR11){
	    	EXTI->PR |= EXTI_PR_PR11;
	    	callback_extInt11();
	    }
	    else if(EXTI->PR & EXTI_PR_PR12){
	    	EXTI->PR |= EXTI_PR_PR12;
	    	callback_extInt12();
	    }
	    else if(EXTI->PR & EXTI_PR_PR13){
	    	EXTI->PR |= EXTI_PR_PR13;
	    	callback_extInt13();
	    }
	    else if(EXTI->PR & EXTI_PR_PR14){
	    	EXTI->PR |= EXTI_PR_PR14;
	    	callback_extInt14();
	    }
	    else if(EXTI->PR & EXTI_PR_PR15){
	    	EXTI->PR |= EXTI_PR_PR15;
	    	callback_extInt15();
	    }
	}




}
