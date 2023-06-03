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
#include <stm32f0xx.h>
/* Valores estandares para las configuraciones */
/* 8.4.1 GPIOx_MODER (Dos bit por cada PIN)*/
#define GPIO_MODE_IN              0
#define GPIO_MODE_OUT             1
#define GPIO_MODE_ALTFN           2
#define GPIO_MODE_ANALOG          3

/* 8.4.2 GPIOx_OTYPER (Un bit por cada PIN)*/
#define GPIO_OTYPE_PUSHPULL       0
#define GPIO_OTYPE_OPENDRAIN      1

/* 8.4.3 GPIOx_OSPEEDR (Dos bit por cada PIN)*/
#define GPIO_OSPEED_LOW           0
#define GPIO_OSPEED_MEDIUM        1
#define GPIO_OSPEED_HIGH          3

/* 8.4.4 GPIOx_PUPDR (Dos bit por cada PIN)*/
#define GPIO_PUPDR_NOTHING        0
#define GPIO_PUPDR_PULLUP         1
#define GPIO_PUPDR_PULLDOWN       2
#define GPIO_PUPDR_RESERVED       3

/* 8.4.5 GPIOx_IDR (Un bit por cada PIN) - Lee el estado de un pin*/
/* 8.4.6 GPIOx_oDR (Un bit por cada PIN) - Escribe el estado de un pin*/

/* Definicion de los nombres de los pines */
#define PIN_0                     0
#define PIN_1                     1
#define PIN_2                     2
#define PIN_3                     3
#define PIN_4                     4
#define PIN_5                     5
#define PIN_6                     6
#define PIN_7                     7
#define PIN_8                     8
#define PIN_9                     9
#define PIN_10                    10
#define PIN_11                    11
#define PIN_12                    12
#define PIN_13                    13
#define PIN_14                    14
#define PIN_15                    15

/* Definicion de las funciones alternativas */
#define AF0                       0b0000
#define AF1                       0b0001
#define AF2                       0b0010
#define AF3                       0b0011
#define AF4                       0b0100
#define AF5                       0b0101
#define AF6                       0b0110
#define AF7                       0b0111


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
	GPIO_TypeDef     *pGPIOx          ; /*!<Direccion del al que el pin corresponde >*/
	GPIO_PinConfig_t   GPIO_PinConfig   ; /*!<Configuracion del pin>*/

}GPIO_Handler_t;

/* definicion de las cabeceras de las funciones GPIOxDriver */
void GPIO_Config      (GPIO_Handler_t *pGPIOHandler);
void GPIO_WritePin    (GPIO_Handler_t *pPinHandler, uint8_t newState);
uint32_t GPIO_ReadPin (GPIO_Handler_t *pPinHandler);
void GPIOxTooglePin (GPIO_Handler_t *pPinHandler);

#endif /* GPIOXDRIVER_H_ */
