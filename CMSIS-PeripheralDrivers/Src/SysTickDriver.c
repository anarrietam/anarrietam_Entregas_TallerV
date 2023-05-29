/*
 * SysTickDriver.c
 *
 *  Created on: May 13, 2023
 *      Author: andres
 */

/*
 * Cambio de velocidad del microcontrolador
 */

#include <stm32f4xx.h>
#include "SysTickDriver.h"

int64_t ticks = 0;
int64_t ticks_start = 0;
int64_t ticks_counting = 0;

void config_SysTick_ms(uint8_t systemClock){
	//reiniciamos el valor de la variable que cuenta tiempo
	ticks = 0;
	//Cargamos el valor del limite de incrementos que represetna 1ms
	switch(systemClock){

	//caso para el reloj HSI-> 16MHZ
	case 0:
		SysTick->LOAD = SYSTICK_LOAD_16MHz_1ms;
		break;
	case 1:
		SysTick->LOAD = SYSTICK_LOAD_16MHz_1ms;
		break;
	case 2:
		SysTick->LOAD = SYSTICK_LOAD_80MHz_1ms;
		break;
	case 3:
		SysTick->LOAD = SYSTICK_LOAD_100MHz_1ms;
		break;
	default:
		SysTick->LOAD = SYSTICK_LOAD_80MHz_1ms;
		break;
	}
	//Limpiamos el valor actual del SysTick
	SysTick->VAL = 0;

	//Configuramos el relos interno como el reloj para el timer
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	//Desactivamos la interrupciones globales
	__disable_irq();

	//Matriculamos las interrupciones en el NVIC
	__NVIC_EnableIRQ(SysTick_IRQn);

	//Activamos la interrupcion debiad al conteo a cero del SysTick
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	//Activamos le timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	//Activamos de nuevo las interrupciones globales
	__enable_irq();
}

	uint64_t getTicksMs(void){
		return ticks;
	}

void delay_ms (uint32_t wait_time_ms){
		//Capturamos el primer valor del tiempo para comparar
		ticks_start = getTicksMs();

		//Capturamos el segundo valor del tiempo para comparar
		ticks_counting = getTicksMs();

		//Compara si el valor de counting es menor que el de start +wait
		//Actualiza rl valor counting
		//repite esta operacion hasta que counting sea mayor

		while(ticks_counting < (ticks_start+(uint64_t )wait_time_ms)){
			//Actualiza el valor
			ticks_counting = getTicksMs();
		}

	}
void SysTick_Handler (void){
		//Verificamos si la interrupcion se lanzo
		if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
			//limpiamosla bandera
			SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;

			//Incrementamos en uno el contador
			ticks++;
		}
	}

