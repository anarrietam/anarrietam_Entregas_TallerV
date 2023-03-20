/*
 * GPIOxDriver.h
 *
 *  Created on: Mar 9, 2023
 *      Author: anarrietam
 */

/****
 * Este programa se encarga de las definiciones realacionadass con el debido
 * manejo y contro de los perifericos GPIOx (Funciones del Controlador
 *****/

#ifndef INC_GPIOXDRIVER_H_
#define INC_GPIOXDRIVER_H_

//Incluyendo este archivo estamos incluyendo tambien el correspondiente al GPIOx
#include "stm32f411xx_hal.h"

typedef struct
{

	uint8_t GPIO_PinNumber            ;  //Pin con el que deseamos trabajar
	uint8_t GPIO_PinMode              ;  //Modo de la configuracion: entrada, salida, analogo, F.Alternativa
	uint8_t GPIO_PinSpeed             ;  //Velocidad de la respuesta del PIN (SOLO DIGITAL)
	uint8_t GPIO_PinPuPdControl       ;  //Seleccionamos si deseamos una salida Pull-Up, Pull-Down o "libre"
	uint8_t GPIO_PinOPType            ;  //Trabaja de la mano con el anterior, salecciona salid PUPD o OPenDrain
	uint8_t GPIO_PinAltFunMode        ;  //Selecciona cual es la funcion alternativa que se esta configurando

}GPIO_PinConfig_t;

/*Esta estructura contiene dos elementos:
 *   -La direccion del puerto que se esta utilizando ( la referencia al puerto)
 *   -La configuracion especifica del PIN que se esta utilizando
 */

typedef struct
{
	GPIOx_RegDef_t     *pGPIOx          ; /*!<Direccion del al que el pin corresponde >*/
	GPIO_PinConfig_t   GPIO_PinConfig   ; /*!<Configuracion del pin>*/

}GPIO_Handler_t;

/* definicion de las cabeceras de las funciones GPIOxDriver */
void GPIO_Config      (GPIO_Handler_t *pGPIOHandler);
void GPIO_WritePin    (GPIO_Handler_t *pPinHandler, uint8_t newState);
uint32_t GPIO_ReadPin (GPIO_Handler_t *pPinHandler);
void GPIOxTooglePin   (GPIO_Handler_t *pPinHandler);

#endif /* GPIOXDRIVER_H_ */
