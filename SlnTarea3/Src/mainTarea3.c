/*
 * mainTarea3.c
 *
 *  Created on: Apr 4, 2023
 *      Author: anarrietam
 */


/*
 * SOLUCION TAREA 3. TALLER V - 2023 - 1
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "ExtiDriver.h"


// Creamos los hadler ( Timers y GPIOs)  que vamos a utilizar para esta tarea

BasicTimer_Handler_t handlerTIM2 = {};
BasicTimer_Handler_t handlerTIM3 = {};
GPIO_Handler_t handlerLedBlinky = {};
GPIO_Handler_t handlerLedA = {};
GPIO_Handler_t handlerLedB = {};
GPIO_Handler_t handlerLedC = {};
GPIO_Handler_t handlerLedD = {};
GPIO_Handler_t handlerLedE = {};
GPIO_Handler_t handlerLedF = {};
GPIO_Handler_t handlerLedG = {};
GPIO_Handler_t handlerDT = {};        // Pin DT del Encoder
GPIO_Handler_t handlerSW = {};        // Pin SW del Encoder
GPIO_Handler_t handlerCLK = {};       // Pin CLK del Encoder
GPIO_Handler_t handlerNum1 ={};
GPIO_Handler_t handlerNum2 ={};

//Cabecera de las funciones creadas para esta tarea
void initConfig (void);
void numCero (void);
void numUno (void);
void numDos (void);
void numTres (void);
void numCuatro (void);
void numCinco (void);
void numSeis (void);
void numSiete (void);
void numOcho (void);
void numNueve (void);

// Creamos variables a utilizar en la tarea
uint8_t countNum1  = 0;
uint8_t countNum2  = 0;
uint8_t auxDisplay = 0;


// En esta funcion configuramos todos los pines necesarios para el correcto desarollo de la solucion a la tarea, además,
//se configuran los timers a usar
void initConfig (void){

	///Configuramos el pin A5 el cual se encargara del Led de estado

	handlerLedBlinky.pGPIOx                             = GPIOA;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinNumber      = PIN_5;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlinky);
	GPIO_WritePin(&handlerLedBlinky, SET);

	//Configuramos el TIM2, para asì poder lograr el parpadeo en el led de estado
	handlerTIM2.ptrTIMx 							= TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode				= BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_period				= 500;
	handlerTIM2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
	BasicTimer_Config(&handlerTIM2);

	//Configuramos las bases de los transistores para así poder observar los dos digitos,
	// en los GPIO_WritePin se observa que para el num1 se encuentra en set y el num2 en reset
	//para así con ayuda del TIM3 ( Que se programa más adelante ) pueda cambiar de estado entre los pines y
	//observar en cada uno de los displays un numero diferente.
	handlerNum1.pGPIOx                                 = GPIOA;
	handlerNum1.GPIO_PinConfig.GPIO_PinNumber          = PIN_6;
	handlerNum1.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_OUT;
	handlerNum1.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_PUSHPULL;
	handlerNum1.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_NOTHING;
	handlerNum1.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerNum1);
	GPIO_WritePin(&handlerNum1, SET);

	handlerNum2.pGPIOx                                 = GPIOA;
	handlerNum2.GPIO_PinConfig.GPIO_PinNumber          = PIN_7;
	handlerNum2.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_OUT;
	handlerNum2.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_PUSHPULL;
	handlerNum2.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_NOTHING;
	handlerNum2.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerNum2);
	GPIO_WritePin(&handlerNum2, RESET);

	//COnfiguramos el TIM3 para la correcta visualizacion del display
	handlerTIM3.ptrTIMx 							= TIM3;
	handlerTIM3.TIMx_Config.TIMx_mode				= BTIMER_MODE_UP;
	handlerTIM3.TIMx_Config.TIMx_period				= 10;                //Para así tener el efecto de ver los dos numeros de maenra "simultanea"
	handlerTIM3.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
	BasicTimer_Config(&handlerTIM3);


	//Configuramos el pin encargado de mostrar el led a de los 7 segmentos

	handlerLedA.pGPIOx                                = GPIOC;
	handlerLedA.GPIO_PinConfig.GPIO_PinNumber         = PIN_6;
	handlerLedA.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedA.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedA.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedA.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedA);
	GPIO_WritePin(&handlerLedA, SET);

	handlerLedB.pGPIOx                                = GPIOC;
	handlerLedB.GPIO_PinConfig.GPIO_PinNumber         = PIN_5;
	handlerLedB.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedB.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedB.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedB.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedB);
	GPIO_WritePin(&handlerLedB, SET);

	handlerLedC.pGPIOx                                = GPIOB;
	handlerLedC.GPIO_PinConfig.GPIO_PinNumber         = PIN_14;
	handlerLedC.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedC.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedC.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedC.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedC);
	GPIO_WritePin(&handlerLedC, SET);

	handlerLedD.pGPIOx                                = GPIOB;
	handlerLedD.GPIO_PinConfig.GPIO_PinNumber         = PIN_2;
	handlerLedD.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedD.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedD.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedD.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedD);
	GPIO_WritePin(&handlerLedD, SET);

	handlerLedE.pGPIOx                                = GPIOB;
	handlerLedE.GPIO_PinConfig.GPIO_PinNumber         = PIN_1;
	handlerLedE.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedE.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedE.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedE.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedE);
	GPIO_WritePin(&handlerLedE, SET);


	handlerLedF.pGPIOx                                = GPIOC;
	handlerLedF.GPIO_PinConfig.GPIO_PinNumber         = PIN_8;
	handlerLedF.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedF.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedF.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedF.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedF);
	GPIO_WritePin(&handlerLedF, SET);

	handlerLedG.pGPIOx                                = GPIOB;
	handlerLedG.GPIO_PinConfig.GPIO_PinNumber         = PIN_15;
	handlerLedG.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerLedG.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerLedG.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerLedG.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedG);
	GPIO_WritePin(&handlerLedG, SET);

}

// Creamos 10 funciones las cuales se encargaran de mostrar cada uno de los numero ( de 0 a 9) en el display
void numCero (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, RESET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, SET);
}

void numUno (void){
	GPIO_WritePin(&handlerLedA, SET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, SET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, SET);
	GPIO_WritePin(&handlerLedG, SET);
}

void numDos (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, SET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, RESET);
	GPIO_WritePin(&handlerLedF, SET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numTres (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, SET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numCuatro (void){
	GPIO_WritePin(&handlerLedA, SET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, SET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numCinco (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, SET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numSeis (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, SET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, RESET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numSiete (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, SET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, SET);
	GPIO_WritePin(&handlerLedG, SET);
}

void numOcho (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, RESET);
	GPIO_WritePin(&handlerLedE, RESET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, RESET);
}

void numNueve (void){
	GPIO_WritePin(&handlerLedA, RESET);
	GPIO_WritePin(&handlerLedB, RESET);
	GPIO_WritePin(&handlerLedC, RESET);
	GPIO_WritePin(&handlerLedD, SET);
	GPIO_WritePin(&handlerLedE, SET);
	GPIO_WritePin(&handlerLedF, RESET);
	GPIO_WritePin(&handlerLedG, RESET);
}


// Sobreescribimos la funcion BasicTimer2_Callback, para asi poder lograr el Blinky del led de estado
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
}

// El timer TIM3 es el que se encarga de poder visualizar el dislay de manera correcta, por eso debemos cambiar el estado de los dos pines
// configurados para los transistores rapidamente,  así cuando uno este encendido el otro esta apagado, por eso en la funcion BasicTimer3_Callback
// se cambian con la ayuda de la funcion GPIOxTooglePin los estados de los pines handlerNum1 y handlerNum2, y asì se visualiza correctamente los numero en el display
void BasicTimer3_Callback(void){

	GPIOxTooglePin(&handlerNum1);
	/*uint8_t estadoNum1 = 0;
	estadoNum1 = GPIO_ReadPin(&handlerNum1);
	if (estadoNum1 == 0){
		numSeis();
	}
	else if (estadoNum1 ==1 ){
		numUno();
	}*/
	GPIOxTooglePin(&handlerNum2);
}

// Hacemos la visualizacion de los numero en el display, con ayuda de tres variables, 2 contadores uno para la
void visualDisplay(void){

	//Hacemos un ciclo while para que cuando el contador del segundo numero sea menor a 9, el mueste el numero que tenga el contador
	while (countNum2 < 9){

		// creamos un if para asi poder ver que digito es el que se va a visualizar y con que numero

		// si el pin del primer digito esta apagado, se va a prender el segundo digito con el valor en el cual este el contador countNum2
		if (GPIO_ReadPin(&handlerNum1) == 0 ){
			auxDisplay = countNum2;

		//En cambio, si el pin del segundo numero esta apagado, se encendera el primer numero con el valor en el cual este el contador countNum1
		}else if (GPIO_ReadPin(&handlerNum2) == 0 ){
			auxDisplay = countNum1;
		}

		switch(auxDisplay){
		case 0:{
			numCero();
			break;
		}
		case 1:{
			numUno();
			break;
			}
		case 2:{
			numDos();
			break;
			}
		case 3:{
			numTres();
			break;
			}
		case 4:{
			numCuatro();
			break;
			}
		case 5:{
			numCinco();
			break;
			}
		case 6:{
			numSeis();
			break;
			}
		case 7:{
			numSiete();
			break;
			}
		case 8:{
			numOcho();
			break;
			}
		case 9:{
			numNueve();
			break;
			}
		}

		// Ponemos la condicion de cuando el contador countNum2 sea igual  o mayor a 9, este se reinicia a 0 y de paso, le suma una unidad al countNum1
		// es decir, cada que llegamos a 9 unidades, se suma un 1 en las decenas
		if (countNum2 >= 9){
			countNum1++;
			countNum2 = 0;

		//	cuando los dos contadores llegan a ser 9, para evitar un overflow al seguir girando el encoder a la direccion de aumento, hacemos que las variables se queden en 9
		}else if (countNum1 >= 9 && countNum2 >= 9){
			countNum1 = 9;
			countNum2 = 9;
			//	cuando los dos contadores llegan a ser 0, para evitar un overflow al seguir girando el encoder a la direccion a la que disminuye el contadot,
			//hacemos que las variables se queden en 0
		}else if (countNum1 == 0 && countNum2 < 0){
			countNum1 = 0;
			countNum2 = 0;
		}

	}
}


int main (void){
	initConfig();
	visualDisplay();
	while(1){

	}

	return 0;
}

