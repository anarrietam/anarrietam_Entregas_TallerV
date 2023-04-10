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


// Creamos los handler ( Timers y GPIOs)  que vamos a utilizar para esta tarea

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

//Pines encoder
GPIO_Handler_t handlerDT = {};        // Pin DT del Encoder
GPIO_Handler_t handlerSW = {};        // Pin SW del Encoder
GPIO_Handler_t handlerCLK = {};       // Pin CLK del Encoder

//Pines transistores
GPIO_Handler_t handlerNum1 ={};
GPIO_Handler_t handlerNum2 ={};

// Creamos 2 EXTI, con los cuales vamos a trabajar, uno para el giro del encoder y saber si aumentamos o disminuimos
// los valores de los contadores, y el otro nos servira para leer cuando se presiona el boton del encoder
EXTI_Config_t extiCount = {0};
EXTI_Config_t extiButton = {0};

//Cabecera de las funciones creadas para esta tarea
void initConfig (void);
void visualDisplayCounter(void);
void visualDisplayCulebrita(void);

// Creamos variables a utilizar en la tarea
uint8_t countContador = 0;
uint8_t auxDisplay = 0;
int8_t countCulebrita = 0;
uint8_t banderaEncoder = 0;
uint8_t valorDT = 0;

/*MAIN DEL PROGRAMA*/
int main (void){
	initConfig();
	while(1){
		//Se le agrega un condicional para que dependiendo del estado de la banderaEncoder, la cual cambia al oprimir el boton dele ncoder
		//esta muestra cualquiera de los dos lados ( ya sea el modo contador o culebrita )
		if (banderaEncoder){
			visualDisplayCulebrita();
		}else{
			visualDisplayCounter();
		}
	}
	return 0;
}

// En esta funcion configuramos todos los pines necesarios para el correcto desarollo de la solucion a la tarea, además,
//se configuran los dos EXTIs que se van a utilizar para el aumento del conteo t y el cambio de modo, y por ultimo, se configura
// el TIM2 el cual se va a encargar de blinky del Led de estado
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

	// el handlerNum1 se encargara de las decenas
	handlerNum1.pGPIOx                                 = GPIOA;
	handlerNum1.GPIO_PinConfig.GPIO_PinNumber          = PIN_6;
	handlerNum1.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_OUT;
	handlerNum1.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_PUSHPULL;
	handlerNum1.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_NOTHING;
	handlerNum1.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerNum1);
	GPIO_WritePin(&handlerNum1, SET);

	//el handlerNum2 se encarga de las unidades
	handlerNum2.pGPIOx                                 = GPIOA;
	handlerNum2.GPIO_PinConfig.GPIO_PinNumber          = PIN_7;
	handlerNum2.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_OUT;
	handlerNum2.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_PUSHPULL;
	handlerNum2.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_NOTHING;
	handlerNum2.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerNum2);
	GPIO_WritePin(&handlerNum2, RESET);



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

	// Escribimos la configuracion para los pines ST, DW y CLK del encoder, para así poder leerlos
	// y con los datos leidos mandar una interrupcion

	handlerCLK.pGPIOx                                  = GPIOC;
	handlerCLK.GPIO_PinConfig.GPIO_PinNumber           = PIN_1;
	handlerCLK.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerCLK.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerCLK.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerCLK);

	handlerDT.pGPIOx                                  = GPIOC;
	handlerDT.GPIO_PinConfig.GPIO_PinNumber           = PIN_2;
	handlerDT.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerDT.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerDT.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerDT);

	handlerSW.pGPIOx                                  = GPIOC;
	handlerSW.GPIO_PinConfig.GPIO_PinNumber           = PIN_3;
	handlerSW.GPIO_PinConfig.GPIO_PinMode             = GPIO_MODE_IN;
	handlerSW.GPIO_PinConfig.GPIO_PinOPType           = GPIO_OTYPE_PUSHPULL;
	handlerSW.GPIO_PinConfig.GPIO_PinPuPdControl      = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerSW);

	//Se configura el extiCount el cual se va a encargar de leer el giro del encoder, y así
	// disminuira o aumentara ya sea el contador o la posicion de la culebrita
	extiCount.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	extiCount.pGPIOHandler = &handlerDT;
	extInt_Config(&extiCount);

	//Se configura el extiCount el cual se va a encargar de leer el giro del encoder, y así
	// disminuira o aumentara ya sea el contador o la posicion de la culebrita
	extiButton.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	extiButton.pGPIOHandler = &handlerSW;
	extInt_Config(&extiButton);

}

// Sobreescribimos la funcion BasicTimer2_Callback, para asi poder lograr el Blinky del led de estado
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
}

// El timer TIM3 es el que se encarga de poder visualizar el display de manera correcta, por eso debemos cambiar el estado de los dos pines
// configurados para los transistores rapidamente,  así cuando uno este encendido el otro esta apagado, por eso en la funcion BasicTimer3_Callback
// se cambian con la ayuda de la funcion GPIOxTooglePin los estados de los pines handlerNum1 y handlerNum2, y asì se visualiza correctamente los numero en el display
void BasicTimer3_Callback(void){

	GPIOxTooglePin(&handlerNum1);
	GPIOxTooglePin(&handlerNum2);
}

// Hacemos la visualizacion de los numeros en el display, con ayuda de dos variables, un contador (countContador)
//y una variable auxiliar que nos ayudara directamente con la visualizacion de los numeros (auxDisplay)
void visualDisplayCounter(void){

		//COnfiguramos el TIM3 para la correcta visualizacion del display
		handlerTIM3.ptrTIMx 							= TIM3;
		handlerTIM3.TIMx_Config.TIMx_mode				= BTIMER_MODE_UP;
		handlerTIM3.TIMx_Config.TIMx_period				= 10;                //Para así tener el efecto de ver los dos numeros de maenra "simultanea"
		handlerTIM3.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
		BasicTimer_Config(&handlerTIM3);

		//Hacemos un ciclo while para que el contador sea menor o igual a 99, se muestren los numeros en el display
		while (countContador <= 99){

			// ponemo un condicional, que lea el estado de banderaEncoder, para que cuando sea uno. el ciclo de esta funcion se rompa y asì se pueda
			// visualizar sin problemas la culebrita
			if(banderaEncoder){
					break;
			}

			// creamos un if para asi poder ver que digito es el que se va a visualizar y con que numero
			// si el pin del primer digito esta apagado, se va a prender el segundo digito con el valor de la operacion modulo entre el contador y 10
			if (GPIO_ReadPin(&handlerNum1) == 0 ){
				auxDisplay = countContador%10;

			//En cambio, si el pin del segundo numero esta apagado, se encendera el primer numero con el valor de la operacion division entre el contador y 10
			}else if (GPIO_ReadPin(&handlerNum2) == 0 ){
				auxDisplay = countContador/10;
			}

			//Creamos un switchcase que funciona con la variable auxiliar (auxDisplay), la cual cambiara con condicional if que hicimos antes, mostrando
			// en cada digito del 7 segemtnos el valor resultado de cada operacion
			switch(auxDisplay){
			case 0:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, RESET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, SET);
				break;
			}
			case 1:{
				GPIO_WritePin(&handlerLedA, SET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, SET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, SET);
				GPIO_WritePin(&handlerLedG, SET);
				break;
				}
			case 2:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, SET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, RESET);
				GPIO_WritePin(&handlerLedF, SET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 3:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, SET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 4:{
				GPIO_WritePin(&handlerLedA, SET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, SET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 5:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, SET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 6:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, SET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, RESET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 7:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, SET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, SET);
				GPIO_WritePin(&handlerLedG, SET);
				break;
				}
			case 8:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, RESET);
				GPIO_WritePin(&handlerLedE, RESET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			case 9:{
				GPIO_WritePin(&handlerLedA, RESET);
				GPIO_WritePin(&handlerLedB, RESET);
				GPIO_WritePin(&handlerLedC, RESET);
				GPIO_WritePin(&handlerLedD, SET);
				GPIO_WritePin(&handlerLedE, SET);
				GPIO_WritePin(&handlerLedF, RESET);
				GPIO_WritePin(&handlerLedG, RESET);
				break;
				}
			}
		}
}

//Creamos una funcion que se encargue de la visualizacion de la "culebrita"
void visualDisplayCulebrita(void){

	//creamos un ciclo while para que esta funcion siga funcionado siempre que la variable countCulebrita sea menor a 12
	while(countCulebrita<12){

		if(banderaEncoder == 0){
			break;
		}

		GPIO_WritePin(&handlerNum1, RESET);
		GPIO_WritePin(&handlerNum2, RESET);


		/*creamos un switchcase con la variable countCulebrita, para que asì dependiendo del valro en el cual se encuentre el contador
		 esta sea la posicion que muestre es decir:

		     1era posicion ------>   LedA-Unidades
		     2da posicion  ------>   LedA-Decenas
		     3era posicion ------>   LedF-Decenas
		     4ta posicion  ------>   LedE-Decenas
		     5ta posicion  ------>   LedD-Decenas
		     6ta posicion  ------>   LedE-Unidades
		     7ta posicion  ------>   LedF-Unidades
		     8va posicion  ------>   LedB-Decenas
		     9na posicion  ------>   LedC-Decenas
		     10ma posicion ------>   LedD-Unidades
		     11va posicion ------>   LedC-Unidades
		     12va posicion ------>   LedB-Unidades
		*/
		switch(countCulebrita){
		// En cada uno de los casos, se decide encender los leds y el transistor correspondiente a esa posicion
		// pero ademas, de modo de "limpieza" apagamos el transistor que no se esta utilizando en cada uno de los casos, y apagamos
		// el led que se debe encender en el caso siguiente, y tambien se apaga el del caso anterior
		case 0:{
			// Pines a " Encender "
			GPIO_WritePin(&handlerLedA, RESET); //A
			GPIO_WritePin(&handlerNum2, SET);   //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 1:{
			//Pines a "Encender"
			GPIO_WritePin(&handlerLedA, RESET);  //A
			GPIO_WritePin(&handlerNum1, SET);    //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 2:{
			//Pines a prender
			GPIO_WritePin(&handlerLedF, RESET);  //F
			GPIO_WritePin(&handlerNum1, SET);    //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 3:{
			//Pines a prender
			GPIO_WritePin(&handlerLedE, RESET);  //E
			GPIO_WritePin(&handlerNum1, SET);    //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 4:{
			//Pines a prender
			GPIO_WritePin(&handlerLedD, RESET);  //D
			GPIO_WritePin(&handlerNum1, SET);    //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 5:{
			//Pines a prender
			GPIO_WritePin(&handlerLedE, RESET);  //E
			GPIO_WritePin(&handlerNum2, SET);    //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 6:{
			//Pineas a prender
			GPIO_WritePin(&handlerLedF, RESET);  //F
			GPIO_WritePin(&handlerNum2, SET);    //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 7:{
			//Pines a prender
			GPIO_WritePin(&handlerLedB, RESET);    //B
			GPIO_WritePin(&handlerNum1, SET);      //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 8:{
			//Pines a prender
			GPIO_WritePin(&handlerLedC, RESET);   //C
			GPIO_WritePin(&handlerNum1, SET);     //DEC

			//Pines a apagar
			GPIO_WritePin(&handlerNum2, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 9:{
			//Pines a prender
			GPIO_WritePin(&handlerLedD, RESET);     //D
			GPIO_WritePin(&handlerNum2, SET);       //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 10:{
			//Pines a prender
			GPIO_WritePin(&handlerLedC, RESET);    //C
			GPIO_WritePin(&handlerNum2, SET);      //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedB, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		case 11:{
			//Pines a prender
			GPIO_WritePin(&handlerLedB, RESET);    //B
			GPIO_WritePin(&handlerNum2, SET);      //UNI

			//Pines a apagar
			GPIO_WritePin(&handlerNum1, RESET);
			GPIO_WritePin(&handlerLedA, SET);
			GPIO_WritePin(&handlerLedF, SET);
			GPIO_WritePin(&handlerLedC, SET);
			GPIO_WritePin(&handlerLedD, SET);
			GPIO_WritePin(&handlerLedE, SET);
			GPIO_WritePin(&handlerLedG, SET);
			break;
			}
		}

		//Se crean dos condicionales para cuando el contador ( counterCulebrita ), llegue a su valor "maximo" en cualquiera de las dos direcciones del counter

		// en este hacemos que cuando el contador pase el valor de 11, es decir, llega a su ultimo posicion, este se reinicie y vuelva a comenzar en 0,
		//es decir la primer posicion en el caso que se gire el encoder en la direccion de aumento
		if (countCulebrita > 11){
			countCulebrita = 0;

		// en este, pasa lo contrario, cuando se gira el encoder para disminuir, y el valor del contador llega a cero, es decir a su primera posicion, se hace que la
		//variable se convierta en 11, para asi volver a la ultima posicion, y reiniciar el ciclo desde la ultima posicion
		}else if (countCulebrita < 0){
			countCulebrita = 11;
		}
	}
	}

void callback_extInt3 (void){
	// Hacemos que cada vez que se oprima el boton, la variable banderaEncode se niegue,
	// es decir, por cada interrupcion de boton, este pasaria de ser 1 a 0, o de 0 a 1.
	//Así se puede lograr un cambio en el modo de visualizacion
		banderaEncoder =! banderaEncoder ;
}

void callback_extInt2 (void){
	valorDT = !valorDT;
	// Si la banderaEncoder, llega a estar en 0, es decir no se ha presionado el boton por primera vez
	// hacemos que aumente o disminuta la variable countContador segun la direccion de giro del encoder
	if (banderaEncoder == 0){
		 if (valorDT == 0){
			 // cada que el valor de DT sea 0, y el valor en el pin de CLK sea 1, el cambio serà en sentido CW, por lo cual habra un aumento en el contador
			 if (GPIO_ReadPin(&handlerCLK)){
				 // para este contador, se le pone el limite de que cuando este llegue a 99, deje de aumentar, para así no sobrepsar
				 // la cantidad maxima de dos digitos que debe mostrar el display
				 if(countContador != 99){
					 countContador++;
				 }

			// ahora en el caso contrario , cuando el valor del pin CLK sea 0, el giro será CCW, y el contador disminuira
			 }else {
				 // en el caso del contador del display contador, se le pone un condicional, que para cuando el contador sea diferente a 0si se puda
				 // disminuir, pero cuando este sea cero, por mucho que se gire el encoder no debería pasar nada
				 if (countContador != 0){
				 	countContador--;
			 }
			 }
		 }
	// cuando ya ae ha oprimido el bonton por primera vez, la banderaEncoder tiene un valor de 1
	// es decir que ahora el aumento o disminucion del valor se vera en la variable counterCulebrita
	}else {
		if (valorDT == 0){
			 // cada que el valor de DT sea 0, y el valor en el pin de CLK sea 1, el cambio serà en sentido CW, por lo cual habra un aumento en el contador
			if (GPIO_ReadPin(&handlerCLK)){
				countCulebrita++;

				// ahora en el caso contrario , cuando el valor del pin CLK sea 0, el giro será CCW, y el contador disminuira
			 }else {
				 countCulebrita--;
					}
				}
		}
	}

