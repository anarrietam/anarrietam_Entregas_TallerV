/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Proyecto base
 ******************************************************************************
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f411xx_hal.h"
/**
 * Funcion principal del programa
 */

int main(void){

	// Configuacion inicial del MCU
	RCC->AHB1ENR &= -(1 << 0) ;      //Borrar posicion cero del registro AHB1ENR
	RCC->AHB1ENR |=   1 << 0  ;      // Activamos la señal de reloj del GPIOA

	//Configuracion de MODER
	GPIOA->MODER  &= -(0b11 << 10) ; // Limpinado la posicion correspondiente al Pin5
	GPIOA->MODER  |= (0b01 << 10)  ; // Configuranddo el pin5 como salida general

	//Configuracion OTYPER
	GPIOA->OTYPER &= -(0b1 << 5)   ; // configurando como push pull

	//Configuracion OSPEED
	GPIOA->OSPEEDR &= -(0b11 << 10); // Limpiar la configuracion para los pines 10 y 11
	GPIOA->OSPEEDR |= (0b01 << 10) ; // Configurando velocidad para fast

	GPIOA->ODR &= -(0b1 << 5);       //Limpiar posicion 5 -LED apagado
	GPIOA->ODR |= (0b1 << 5);        //EScribiendo 1 en posicion 5 -LED encendido


    /* Loop forever */
	while (1){

	}
	return 0;
}
