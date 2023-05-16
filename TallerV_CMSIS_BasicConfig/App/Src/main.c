/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Solucion Basica de un proyecto con librerias externas
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <stm32f4xx.h>

#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
#include "SysTickDriver.h"

#define HSI_CLOCK_CONFIGURED 0  // 16MHz
#define HSE_CLOCK_CONFIGURED 1
#define PLL_CLOCK_CONFIGURED 2

GPIO_Handler_t handlerLedBlinky = {0};
BasicTimer_Handler_t handlerTIM2 = {0};

// Prueba del USART
GPIO_Handler_t handlerUSARTPINTX = {0};
GPIO_Handler_t handlerUSARTPINRX = {0};
USART_Handler_t USART2Comm = {0};

uint8_t sendMSG = 0;
char mensaje[] = "HOLA\n";
uint8_t newChar = 0;



int main(void){


	// Activamos el FPU
	//SCB->CPACR |= (0xF << 20);

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

	handlerUSARTPINTX.pGPIOx  = GPIOA;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINTX);

	handlerUSARTPINRX.pGPIOx  = GPIOA;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinNumber= PIN_3;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINRX);

	USART2Comm.ptrUSARTx = USART2;
	USART2Comm.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	USART2Comm.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	USART2Comm.USART_Config.USART_parity = USART_PARITY_NONE;
	USART2Comm.USART_Config.USART_mode = USART_MODE_RXTX;
	USART2Comm.USART_Config.USART_stopbits = USART_STOPBIT_1;
	USART2Comm.USART_Config.USART_enable_IntRx = USART_RX_INTERRUPT_ENABLE;
	USART2Comm.USART_Config.USART_enable_IntTx = USART_TX_INTERRUPT_DISABLE;

	USART_Config(&USART2Comm);



    while(1){

    	if (sendMSG == 5){
    		sendMSG = 0;
    		writeMsg(&USART2Comm, mensaje);
    	}

    }

}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
	sendMSG++;

}

void usart2Rx_CallBack (void){
	newChar = getRxData();
	writeChar(&USART2Comm, newChar);
}



