/*
 * GPIOxMain.c
 *
 *****************************
 *  Created on: Mar 13, 2023
 *      Author: anarrietam
 *****************************
 */


// LIBRERIAS

#include <stdint.h>
#include <stm32f411xx_hal.h>
#include <GPIOxDriver.h>

/***************************
 * Desarrollo de la tarea
 *
 * PUNTO 1:
 * a) El error en la funcion GPIO_ReadPin se encontraba en que al cargar el valor del registro IDR en la variable PinValue,
 * este nos mostraba todos los valores que tenia este registro màs alla del que necesitabamos, es decir, el pin que se habia elegido para trabajar
 *
 * b) Esto se podria solucionar con ayuda de una mascara en la variable PinValue, puesto que el registro IDR es solo de lectura y no se puede escribir
 * sobre este; la mascara limpiaria todo el registro, dejando solo el bit que nos interesa.
 *
 * c) La solucion se puede observar en el archivo GPIOxDriver.c
 *
 ***************************/


int main(void){

/**********************************
 * Codigo para los puntos 1 y 2
 ************************************/

	//Definimos el handler para el PIN que deseamos configurar
	GPIO_Handler_t handlerUserLedPin ={0};

	//Deseamos trabajar con el puerto GPIOA
	handlerUserLedPin.pGPIOx                             = GPIOA               ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinNumber      = PIN_5               ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT      ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING  ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_MEDIUM  ;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0                 ; // Ninguna funcion

	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handlerUserLedPin);

	//Hacemos que el PIN A5 quede encendido
	GPIO_WritePin(&handlerUserLedPin, RESET);

	GPIO_ReadPin(&handlerUserLedPin);   // Prueba del punto 1

	GPIOxTooglePin(&handlerUserLedPin); // Prueba del punto 2

/*************************
 * PUNTO 3
 **************************/

// Codigo para el punto 3

	// En este punto se utilizaran los pines:PA7, PC8, PC7, PA6, PB8, PC6, PC9

	// Configuramos los pines a utilizar:

	// PIN A 7
	GPIO_Handler_t PINA7 ={0};

	PINA7.pGPIOx                                    =GPIOA              ;  // Se seleccionan el puerto
	PINA7.GPIO_PinConfig.GPIO_PinNumber             =PIN_7              ;  // Elegimos el numero de pin
	PINA7.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINA7.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINA7.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINA7.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINA7.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// PIN C 8
	GPIO_Handler_t PINC8 ={0};

	PINC8.pGPIOx                                    =GPIOC              ;  // Se seleccionan el puerto
	PINC8.GPIO_PinConfig.GPIO_PinNumber             =PIN_8              ;  // Elegimos el numero de pin
	PINC8.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINC8.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINC8.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINC8.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINC8.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// PIN C 7
	GPIO_Handler_t PINC7 ={0};

	PINC7.pGPIOx                                    =GPIOC              ;  // Se seleccionan el puerto
	PINC7.GPIO_PinConfig.GPIO_PinNumber             =PIN_7              ;  // Elegimos el numero de pin
	PINC7.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINC7.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINC7.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINC7.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINC7.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	//PIN A 6
	GPIO_Handler_t PINA6 ={0};

	PINA6.pGPIOx                                    =GPIOA              ;  // Se seleccionan el puerto
	PINA6.GPIO_PinConfig.GPIO_PinNumber             =PIN_6            ;  // Elegimos el numero de pin
	PINA6.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINA6.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINA6.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINA6.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINA6.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// PIN B 8
	GPIO_Handler_t PINB8 ={0};

	PINB8.pGPIOx                                    =GPIOB              ;  // Se seleccionan el puerto
	PINB8.GPIO_PinConfig.GPIO_PinNumber             =PIN_8              ;  // Elegimos el numero de pin
	PINB8.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINB8.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINB8.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINB8.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINB8.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// PIN C 6
	GPIO_Handler_t PINC6 ={0};

	PINC6.pGPIOx                                    =GPIOC              ;  // Se seleccionan el puerto
	PINC6.GPIO_PinConfig.GPIO_PinNumber             =PIN_6              ;  // Elegimos el numero de pin
	PINC6.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINC6.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINC6.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINC6.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINC6.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// PIN C 9
	GPIO_Handler_t PINC9 ={0};

	PINC9.pGPIOx                                    =GPIOC              ;  // Se seleccionan el puerto
	PINC9.GPIO_PinConfig.GPIO_PinNumber             =PIN_9              ;  // Elegimos el numero de pin
	PINC9.GPIO_PinConfig.GPIO_PinMode               =GPIO_MODE_OUT      ;  // Configuramos el pim como una salida
	PINC9.GPIO_PinConfig.GPIO_PinOPType             =GPIO_OTYPE_PUSHPULL;  // Seleccionamos una configuacion Push-Pull
	PINC9.GPIO_PinConfig.GPIO_PinPuPdControl        =GPIO_PUPDR_NOTHING ;  // Sin pull up ni pull down
	PINC9.GPIO_PinConfig.GPIO_PinSpeed              =GPIO_OSPEED_HIGH   ;  // Velcoidad alta
	PINC9.GPIO_PinConfig.GPIO_PinAltFunMode         =AF0                ;  // Ninguna funcion alterna

	// Ahora se configura el USER_BUTTON, el cual gracias a la documentacion , se puede observar que esta conectado al PINC13

	GPIO_Handler_t User_Button = {0};

	User_Button.pGPIOx                             =GPIOC                ;
	User_Button.GPIO_PinConfig.GPIO_PinNumber      =PIN_13               ;
	User_Button.GPIO_PinConfig.GPIO_PinMode        =GPIO_MODE_IN         ;
	User_Button.GPIO_PinConfig.GPIO_PinPuPdControl =GPIO_PUPDR_PULLDOWN  ;
	User_Button.GPIO_PinConfig.GPIO_PinAltFunMode  =AF0                  ;

	// Cargamos la configuracion a cada pin y del boton
	GPIO_Config(&PINA7);
	GPIO_Config(&PINC8);
	GPIO_Config(&PINC7);
	GPIO_Config(&PINC7);
	GPIO_Config(&PINA6);
	GPIO_Config(&PINB8);
	GPIO_Config(&PINC6);
	GPIO_Config(&PINC9);
	GPIO_Config(&User_Button);

	// Creamos un arreglo donde adentro estaran cada uno de los pines con su configuracion
	GPIO_Handler_t pinsConfig[7];

	// Cargamos en cada una de las posiciones una configuracion para cada pin y el User_Button
	pinsConfig[0] = PINA7;
	pinsConfig[1] = PINC8;
	pinsConfig[2] = PINC7;
	pinsConfig[3] = PINA6;
	pinsConfig[4] = PINB8;
	pinsConfig[5] = PINC6;
	pinsConfig[6] = PINC9;
	pinsConfig[7] = User_Button;

	// Ahora creamos una variable contador el cual nos va a ayudar con el conteo de los segundos y la inicializamos en 0 y se crea una variable auxiliar
	uint8_t cont = 1;
	uint8_t aux  = 0;

	// se crea una variable donde se guardara el estado del User Button
	uint8_t estadoBoton = 0;


	//Este es el ciclo principal, donde se ejecuta todo el programa
	while(1){

		// en la variable creada se guarda el estado del User_Button
		estadoBoton = GPIO_ReadPin(&User_Button);

		// Se crea un condicional para cuando el boton no este oprimido
		if (estadoBoton >= 1){

			// Creamos un condicional para que el el programa funciona hasta que llegue a 60, el contador ( que seria cuando ya hallan pasado los 60 segundos )
			if (cont <= 60){

				// Creamos un ciclo con ayuda del for que vaya de 0 a 7, para así recorre todas las configuraciones de los pines
				for (uint8_t i = 0; i <=6 ; i++){

					// en la variable auxiliar se guarda el numero del contado rodado i veces hacia la derecha, para asi tener el 1 o el 0, que se encuentra en ese bit del contador
					aux = cont >> i;

					aux &= 0b1   ; // Aplicamos una mascara para limpiar el resto de bits de la variable, y asi solo obtener el bit que nos interesa

					// Aca al ser un 1 un set y el 0 reset, pasamos pin a pin escribiendo sobre él, el estado que nos indique cada posicion del contador el cual aumenta cada segundo
					GPIO_WritePin(&pinsConfig[i], aux);
				}
				// creamos un ciclo con for, en el cual lo configuramos para obtener un delay de 1 s
						for (uint32_t delay = 0; delay <1300000 ; delay++);
			}
			// Creamos un funcion para que el contador al llegar a 60 o superar este numero, se reinicie a 1
			else if (cont > 60){

						cont = 0;
					}

			cont ++;// Aumentamos el contador en 1
		}
		// Ahora cuando el boton este presionado su estado será de 0
		else if (estadoBoton <= 0){

			// Creamos un if para cuando el contador este entre 60 y 1
			if (cont <= 60 && cont >= 1 ){

				//Se crea un ciclo para recorrer las configuraciones de los pines
				for (uint8_t i = 0; i <=6 ; i++){

					// Guardamos el contador rodados i veces a la derecha
					aux = cont >> i;
					aux &=0b1;                             //Limpiamos el resto de la variable, para solo obtener el bit que necesitamos

					// Escribimos la configuracion en el pin
					GPIO_WritePin(&pinsConfig[i] , aux);
				}
				// creamos un ciclo con for, en el cual lo configuramos para obtener un delay de 1 s
						for (uint32_t delay = 0; delay <1300000 ; delay++);
			// Para comenzar a disminuir el valor del sistema, debemos ir restando el contador de a 1 cada que completemos un ciclo
				cont--;
			}
			// se crea un if para cuandoe l contador ya sea menor a 1
			else if(cont < 1){

				// cuando esta condicion se cumpla, el sistema debe reiniciarse en 60
				//por lo se hace que el contador vuelva a 60
				cont = 60;
			}
		}

	}
}
