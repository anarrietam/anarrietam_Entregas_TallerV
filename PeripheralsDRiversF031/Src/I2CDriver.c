/*
 * I2CDriver.c
 *
 *  Created on: Jun 15, 2023
 *      Author: andres
 */

#include <stdint.h>
#include <I2CDriver.h>

/*
 * Recordar que se se deben configurar los pines para los pines para el I2C (SDA y SCL),
 * para lo cual se necesita el modulo GPIO y los pines configurados en el modo Alternate
 * Function.
 *
 * Además, estos pines deben ser configurados como salidas open-drain y con las
 * resistencias en modo pull-up..
 */

void i2c_config(I2C_Handler_t *ptrHandlerI2C){

	// Activar la señal de reloj para I2C1
	RCC->CR |= RCC_APB1ENR_I2C1EN;

	//Reiniciamos el sistema
	ptrHandlerI2C->ptrI2Cx->CR1 &= ~I2C_CR1_PE_Msk;

	// hay que esperar 3 ciclos de reloj por lo cual, el manuel de referencia nos indica que
	// escribimos PE en 0, revisamos que este en 0 y lo volvemos a poner en 1
	__NOP();
	__NOP();
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_PE_Msk;

	/* 4. Configuramos el modo I2C en el que el sistema funciona
	 * En esta configuración se incluye también la configuración del reloj
	 * y el tiempo maximo para el cambio de la señal (T-Rise).
	 * Todo comienza con los registros en 0
	 */




}
