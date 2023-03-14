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

/******
 * Para cualquier Periferico, hay ciertos paso para que el sistema deje configurarlo
 * Lo primero y màs importante es activar la señal de reloj principal hacia ese elemento
 * especifico (relacionado con el periferico RCC)
 ******/
void GPIO_Config   (GPIO_Handler_t *pGPIOHandler){

	//Variable para hacer todo paso a paso
	uint32_t  auxConfig   = 0;
	uint32_t  auxPosition = 0;

	// 1) Activar el Perifierico
	// Verificamos para GPIOA
	if(pGPIOHandler->pGPIOx == GPIOA){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOA
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOA_EN);
	}

	// Verificamos para GPIOB
	else if(pGPIOHandler->pGPIOx == GPIOB){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOB
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOB_EN);
		}

	// Verificamos para GPIOC
	else if(pGPIOHandler->pGPIOx == GPIOC){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOC
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOC_EN);
			}

	// Verificamos para GPIOD
	else if(pGPIOHandler->pGPIOx == GPIOD){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOD
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOD_EN);
			}

	// Verificamos para GPIOE
	else if(pGPIOHandler->pGPIOx == GPIOE){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOC
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOE_EN);
			}
	// Verificamos para GPIOH
	else if(pGPIOHandler->pGPIOx == GPIOH){
		//EScribimos 1 (SET) en la posicion correspondiente al GPIOH
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOH_EN);
			}

	//Despues de activado, podemos empezar a configurar
;

	// 2) Configurando el registro GPIOx_MODER
	/*Acà estamos leyendo la config, moviendo "PinNumber" veces a la izquierda ese valor
	 * y se carga en la variable auxConfig
	 */

	auxConfig = ( pGPIOHandler->GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Antes de crear el nuevo valor, limpiamos los bits  especificos de ese regsitro
	// para lo cual aplicamos una mascara y una operacion bitwise AND

	pGPIOHandler->pGPIOx->MODER &= -(0b11 << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//cargamos a auxconfig en el registro MODER

	pGPIOHandler->pGPIOx->MODER |= auxConfig;

	// 3) configuramos el registro GPIOx_OTYPER
	//De nuevo, leemos y movemos el valor un numero "PINUMBER" de veces

	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinDPType << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// Limpiamos antes de cargar

	pGPIOHandler->pGPIOx->OTYPER &= -(SET << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// cargamos el resultado sobre el registro adecuado

	pGPIOHandler->pGPIOx->OTYPER |= auxConfig;

	// 4) COnfiguramos ahora la velocidad

	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Limpiando la configuracion antes de caegar la nueva configuracion

	pGPIOHandler->pGPIOx->OSPEEDR &= -(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//cargamos el resultado sobre el registro adecudado

	pGPIOHandler->pGPIOx->OSPEEDR |= auxConfig;

	// 5) configurando si se desea pull up, down o flotante

	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl << 2* pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Limpiando la configuracion antes de cargar la nueva configuracion

	pGPIOHandler->pGPIOx->PUPDR &= -(0b11 << 2* pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	// cargamos el resultado sobre el registro adecuado

	pGPIOHandler->pGPIOx->PUPDR |= auxConfig;

	// Configuracion fns alternativas
	if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		//Seleccionamos primero si se debe seleccionar el registro bajo (AFRL) o alto (AFRH)
		if(pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber < 8){

			//Estamos en el registro AFRL, que controlaes los pines del 0:7
			auxPosition = 4*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

			//Limpiamos primero la posicion del registro que deseamos escribir a continuacion
			pGPIOHandler->pGPIOx->AFRL &= -(0b1111 << auxPosition);

			// y escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler->pGPIOx->AFRL |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);

		}
		else{
			//REgistro AFRH, controla los pines 8:15
			auxPosition = 4* (pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber -8);

			//Limpiamos primero la posicion del registro que queremos escribir
			pGPIOHandler->pGPIOx->AFRH &= -(0b1111 << auxPosition);

			// y escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler->pGPIOx->AFRH |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);

		}
	} // FIN DEL GPIO_config
}
/********
 * Funcion utilizada para cambiar de estado el pin entregado en el Handler, asignando
 * el valor entregado en la variable newState
 ********/

void GPIO_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){

	//Limpiamos la posicion que deseamos
	//pPinHandler->pGPIOx->ODR &= -(SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);

	if (newState == SET){
		pPinHandler->pGPIOx->BSRR |= (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	}
	else{
		pPinHandler->pGPIOx->BSRR |= (SET << (pPinHandler->GPIO_PinConfig.GPIO_PinNumber + 16));
	}
}

/********
 * funcion para leer el estado de un pin especifico
 ********/

uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler){

	//creamos una variable auxiliar la cual luego retornamos
	uint32_t pinValue = 0;

	// Cargamos el valor del registro IDR, desplazando a derecha tantas veces como la ubicacion del pin especifico
	pinValue = (pPinHandler->pGPIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber);

	return pinValue;
}
