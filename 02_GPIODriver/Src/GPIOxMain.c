/*
 * GPIOxMain.c
 *
 *****************************
 *  Created on: Mar 13, 2023
 *      Author: anarrietam
 *****************************
 *
 * Se desea mostar el uso basico de los registros que controlan el micro (SFR)
 *
 * HAL --> Hardaware ABstraction Layer
 *
 * Este programa introduce el periferico màs simple que contiene el micro, que es el
 * encargado de manejar los pines de cada puerto
 *
 * Cada Pinx de cada GPIO puede configurarse como entrada y salida de datos digitales,
 * además se le puede asignar fns especiales que estan ligadas a otro perifercio adicional
 *
 * Debemos definir entonces todos los registros que manejan el periferico GPIOx y luego
 * crear algunas fns para utilizar adecuadamente el equipo
 *
 */

// LIBRERIAS

#include <stdint.h>
#include <stm32f411xx_hal.h>
#include <GPIOxDriver.h>

/*********************************
 * FUNCION PRINCIPAL DEL PROGRAMA
 *********************************/
int main(void){

	//Definimos el handler para el PIN que deseamos configurar
	GPIO_Handler_t handlerUserLedPin ={0};

	//Deseamos trabajar con el puerto GPIOA
	handlerUserLedPin.pGPIOx                             = GPIOA               ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinNumber      = PIN_5               ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT       ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinDPType      = GPIO_OTYPE_PUSHPULL ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING  ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_MEDIUM  ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0                 ; // Ninguna funcion

	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handlerUserLedPin);

	//Hacemos que el PIN A5 quede encendido
	GPIO_WritePin(&handlerUserLedPin, SET);

	//Este es el ciclo principal, donde se ejecuta todo el programa
	while(1){

		NOP();

	}
}
