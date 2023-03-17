/*
 * Taller_Semana_6.c
 *
 *  Created on: Mar 16, 2023
 *      Author: anarrietam
 */


#include <stdint.h>

#define SUMA    '+'
#define RESTA   '-'
#define MULTIPLICACION '*'
#define DIVISION '/'
#define FACTORIAL '!'
#define FIBONACCI 'F'


// HEADERS DE FUNCIONES

uint16_t Operaciones(uint8_t tipoOperacion, uint16_t numero1,uint16_t numero2 );
uint32_t Operaciones1(uint8_t Operacion, uint16_t numero);


int main (void){

	uint32_t salida = Operaciones1(FACTORIAL, 6);
	salida = Operaciones1(FIBONACCI, 6);

	while (1){

	}

}


uint16_t Operaciones(uint8_t tipoOperacion, uint16_t numero1,uint16_t numero2 ){

	switch(tipoOperacion){

	case SUMA:{
		return numero1 + numero2;
		break;
	}
	case RESTA:{
		return numero1 - numero2;
		break;
	}
	case MULTIPLICACION:{
		return numero1*numero2;
		break;
	}
	case DIVISION:{
			return numero1/numero2;
			break;
		}
	default:{
		return 0;
		break;
	}
	}
}

uint32_t Operaciones1(uint8_t Operacion, uint16_t numero){

	uint16_t cont = 1;
	uint32_t resultado1 = 1;
	uint16_t varAux1 = 1;
	uint16_t varAux2 = 1;

	switch(Operacion){
	case FACTORIAL:{
		while(cont <= numero){
			resultado1 *=cont ;
			cont++;
		}
		return resultado1;
		break;
	}
	case FIBONACCI:{
		while(cont < numero){
			resultado1 = varAux1 + varAux2;
			varAux1 = varAux2;
			varAux2 = resultado1;
			cont ++;
		}
		return resultado1;
		break;
	}
	default:{
		return 0;
		break;
	}
	}
}
