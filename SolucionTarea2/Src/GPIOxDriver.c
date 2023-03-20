/*
 * GPIOxDriver.c
 *
 *  Created on: Mar 9, 2023
 *      Author: anarrietam
 */

/******
 * En este archivo se escribe el control para que sea lo mas generico
 * posble, de fomra que independiente del puerto GPIO y el PIN seleccionado,
 * el programa se ejecute y configure todo correctamente
 ******/

#include "GPIOxDriver.h"

/**
 * Para cualquier periférico, hay varios pasos que siempre se deben seguir en un
 * orden estricto para poder que el sistema permita configurar el periférico X.
 * Lo primero y más importante es activar la señal del reloj principal hacia ese
 * elemento específico (relacionado con el priférico RCC), a esto lo llamaremos
 * simplemente "activar el periférico o activar la señal de reloj del periférico)
 */
void GPIO_Config (GPIO_Handler_t *pGPIOHandler){

	// Variable para hacer todo paso a paso
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	// 1) Activar el periférico
	//Verificamos para GPIOA
	if(pGPIOHandler->pGPIOx == GPIOA){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOA
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOA_EN);
	}
	//Verificamos para GPIOB
	else if(pGPIOHandler->pGPIOx == GPIOB){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOB
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOB_EN);
	}
	//Verificamos para GPIOC
	else if(pGPIOHandler->pGPIOx == GPIOC){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOC
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOC_EN);
	}
	//Verificamos para GPIOD
	else if(pGPIOHandler->pGPIOx == GPIOD){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOD
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOD_EN);
	}
	//Verificamos para GPIOE
	else if(pGPIOHandler->pGPIOx == GPIOE){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOE
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOE_EN);
	}
	//Verificamos para GPIOH
	else if(pGPIOHandler->pGPIOx == GPIOH){
		// Escribimos 1 (SET) en la posición correspondiente al GPIOH
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOH_EN);
	}

	//Después de activado, podemos comenzar a configurar.

	// 2) Configurando el registro GPIOx_MODER
	// Acá estamos leyendo la config, moviendo "PinNumber" veces hacia la izquierda ese valor (shift left)
	// y todo eso lo cargamos en la variable auxConfig
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Antes de cargar el nuevo valor, limpiamos los bits específicos de ese registro (debemos escribir 0b00)
	// para lo cual aplicamos una máscara y una operación bitwise AND
	pGPIOHandler->pGPIOx->MODER &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Cargamos a auxConfig en el registro MODER
	pGPIOHandler->pGPIOx->MODER |= auxConfig;

	// 3) Configurando el registro GPIOx_OTYPER
	// De nuevo, leemos y movemos el valor un número "PinNumber" de veces
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Limpiamos antes de cargar
	pGPIOHandler->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Cargamos el resultado sobre el registro adecuado
	pGPIOHandler->pGPIOx->OTYPER |= auxConfig;

	// 4) Configurando ahora la velocidad
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Limpiando la posición antes de cargar la nueva configuración
	pGPIOHandler->pGPIOx->OSPEEDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Cargamos el resultado sobre el registro adecuado
	pGPIOHandler->pGPIOx->OSPEEDR |= auxConfig;

	// 5) Configurando si se desea pull-up, pull-down o flotante.
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Limpiando la posición antes de cargar la nueva configuración
	pGPIOHandler->pGPIOx->PUPDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Cargamos el resultado sobre el registro adecuado
	pGPIOHandler->pGPIOx->PUPDR |= auxConfig;

	// Esta es la parte para la configuración de las funciones alternativas... se verá luego
	if(pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		// Seleccionamos primero si se debe utilizar el registro bajoo (AFRL) o el alto (AFRH)
		if(pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber < 8){
			// Estamos en el registro AFRL, que controla los pines del PIN_0 al PIN_7
			auxPosition = 4 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

			// Limpiamos primero la posición del registro que deseamos escribir a continuación
			pGPIOHandler->pGPIOx->AFRL &= ~(0b1111 << auxPosition);

			// Y escribimos el valor configurado en la posición seleccionada
			pGPIOHandler->pGPIOx->AFRL |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}
		else{
			// Estamos en el registro AFRH, que controla los pines del PIN_8 al PIN_15
			auxPosition = 4 * (pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber -8);

			// Limpiamos primero la posición del registro que deseamos escribir a continuación
			pGPIOHandler->pGPIOx->AFRH &= ~(0b1111 << auxPosition);

			// Y escribimos el valor configurado en la posición seleccionada
			pGPIOHandler->pGPIOx->AFRH |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}
	}

} // Fin del GPIO_config

/**
 * Función utilizada para cambiar de estado el pin entregado en el handler, asignado
 * el valor entregado en la variable newState
 */
void GPIO_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){
	// Limpiamos la posición que deseamos
	//pGPIOHandler->pGPIOx->ODR &= ~(set << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	if(newState == SET){
		// Trabajando con la parte baja del registro
		pPinHandler->pGPIOx->BSRR |= (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	}
	else{
		// Trabajando con la parte alta del registro
		pPinHandler->pGPIOx->BSRR |= (SET << (pPinHandler->GPIO_PinConfig.GPIO_PinNumber + 16));
	}
}

/**
 * Función para leer el estado de un pin especifico.
 */

uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler){

	//creamos una variable auxiliar la cual luego retornamos
	uint32_t pinValue = 0;

	// Cargamos el valor del registro IDR, desplazando a derecha tantas veces como la ubicacion del pin especifico
	pinValue = (pPinHandler->pGPIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber);

	pinValue &= 0b1 ; // Se crea la mascara que es la solucion al problema que tiene esta funcion ( GPIO_ReadPin)
	return pinValue;

}

/****************
 * Solucion Punto 2 de la tarea
 *****************/

// Definimos una funcion Toogle que cambia el estado de un pin, esta será una funcion vacia que
// acepta como parametro una variable tipo GPIO_Handler_t
void GPIOxTooglePin(GPIO_Handler_t *pPinHandler) {

	// Para cambiar el estado del pin lo haremos en el registro ODR, el cual nos permite leerlo y escribir sobre él,
	// con ayuda de la operacion XOR bitwise haremos el cambio del valor del bit, el cual hara que cambie el estado del pin

	//La mascara que se utilizara será 0b1 que al usarla con la operacion XOR, hara que si el bit esta en 1, al operar con este cambiara a 0,
	//y cuando el bit este en 0, al operar se cambiara a 1; cambiando asi de estado.

	pPinHandler->pGPIOx->ODR ^= (0b1 << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);   //Aca se aplica la mascara al bit que se desea cambiar, rodandola hacia la izquierda
																					   // hasta la posisicion deseada ( "PinNumber" nos da la cantidad de veces que nos desplazamos)

}
