/*
 * Taller_Semana_5.c
 *
 *  Created on: Mar 9, 2023
 *      Author: anarrietam
 */

// LIBRERIAS :

#include <stdint.h>     // Permite trabajar con enteros
#include <stdbool.h>    // Para variables booleanas
#include <math.h>       // Funciones matematicas

// VARIABLES Y CONSTANTES:

#define CONSTANTE 100
#define UNSIGNED  0
#define SIGNED    1

bool variableBooleana = true;
uint8_t parametro1    = 100 ;
int16_t parametro2    = -173;

//HEADERS
void clearGlobal(void);
uint8_t getMaxChart(void);
uint16_t getMaxValue (uint16_t x, uint16_t y, uint16_t z);
uint64_t getMaxValueUS (uint8_t nBit, uint8_t signo );

int main (void){

	clearGlobal();
	parametro1 = getMaxChart();
	uint16_t parametro3 = getMaxValue(100, 100, 50);
	uint64_t parametro4 = getMaxValueUS(64, UNSIGNED);

}

void clearGlobal(void){
	variableBooleana = false;
	parametro1 = 0b01;
	parametro2 = 0;
}

uint8_t getMaxChart(void){

	uint8_t maxChar = pow(2,8)-1;
	return maxChar;

}

uint16_t getMaxValue (uint16_t x, uint16_t y, uint16_t z){

	if ((x>=y) && (x>=z)){
		return x;
	}else if ((y>=x) && (y>=z)){
		return y;
	}else{
		return z;
	}
}

uint64_t getMaxValueUS (uint8_t nBit, uint8_t signo ){

	uint64_t maxVar;

	if (nBit == 8 || nBit == 16 || nBit == 32 || nBit == 64 ){
		if (signo == SIGNED){
			maxVar = pow(2,nBit)/2-1;
		}else if (signo == UNSIGNED){
			maxVar = pow(2,nBit)-1;
		}else{
			maxVar = 0;
		}
	}else {
		maxVar = 0;
	}
	return maxVar;
}
