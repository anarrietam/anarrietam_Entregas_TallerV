/*
 * Taller8_template.c
 *
 *  Created on: 28/03/2023
 *      Author: alvelezp - jutoroa
 */

// Taller 6 - Paso por valor y paso por referencia

#include <stdint.h>

// Creacion de una funcion que duplique el valor de un numero

// 1.0 Paso por valor Básico

void duplicarNumero(uint8_t numero);

// 1.1 Paso por referencia

void duplicarNumeroRef(uint8_t *numero);

// 1.2 Paso por valor reasignando variables.

uint8_t duplicarNumeroReturn(uint8_t numero);

// 1.3 Arreglos por Referencia

void abonarDeudas(uint16_t misDeudas[], uint8_t cantidadDeudas);

// ***** // SOLUCION EJERCICIO // ***** //

void stringCaseConverter(uint8_t *string);

int main(void){

	uint8_t variable = 10;
	variable = duplicarNumeroReturn(variable);

	uint16_t deudas[] = {15000, 10000, 5000};

	abonarDeudas(deudas, 3);

	uint8_t string[] = "HoLa";

	stringCaseConverter(string);




	// 1.5 EJERCICIO:

	// Crear una función llamada stringCaseConverter que no retorne ningún
	// valor, y reciba una string.

	// Esta función deberá cambiar las minúsculas por mayúsculas y viceversa
	// del string original.

}

void duplicarNumero(uint8_t numero){

	numero = numero *2;

}

uint8_t duplicarNumeroReturn(uint8_t numero){

	numero = numero*2;
	return numero;
}

void duplicarNumeroRef(uint8_t *numero){

	*numero *= 2;
}

void abonarDeudas(uint16_t misDeudas[], uint8_t cantidadDeudas){

	for (uint8_t i=0 ; i<cantidadDeudas; i++){

		misDeudas[i] = misDeudas[i]/2;

	}

}

void stringCaseConverter(uint8_t *string){

	uint8_t count = 0;

	while (*(string+count) != 0){

		if (*(string+count) >= 97 && *(string+count)<=122){

			*(string+count) = *(string+count)-32;

		}
		else if(*(string+count) >= 65 && *(string+count)<=90){
			*(string+count) = *(string+count)+32;
		}
		count ++;
	}
}
