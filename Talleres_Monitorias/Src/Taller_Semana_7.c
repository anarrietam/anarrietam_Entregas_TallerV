/*
 * Taller_Semana_7.c
 *
 *  Created on: Mar 23, 2023
 *      Author: anarrietam
 */


#include <stdint.h>

int main (void){

	uint32_t dato = 0xABCD;
	uint16_t dato_16 = (uint16_t) dato; //Cast
	uint32_t *ptrDato = &dato;           // ir a la posicion de memoria de la variable dato
	uint32_t dirDacto = (uint32_t) ptrDato;  // para obtenmer el numero de la direccion de memoria
	(void) dato_16;
	(void) dirDacto;

	*ptrDato = 0x1234;
	ptrDato ++;
	*ptrDato = 100;

	//Areglos

	uint32_t miPrimerArreglo[] = {12 ,15, 0xABD, 0b111110}



	while(1){

	}
}
