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
#include "PwmDriver.h"
#include "AdcDriver.h"

#define HSI_CLOCK_CONFIGURED 0  // 16MHz
#define HSE_CLOCK_CONFIGURED 1
#define PLL_CLOCK_CONFIGURED 2

GPIO_Handler_t handlerLedBlinky = {0};
BasicTimer_Handler_t handlerTIM2 = {0};

// Prueba del USART
GPIO_Handler_t handlerUSARTPINTX = {0};
GPIO_Handler_t handlerUSARTPINRX = {0};
USART_Handler_t USART2Comm = {0};

// Prueba del PWM
GPIO_Handler_t HandlerPWM = {0};
PWM_Handler_t handlerTIM3PWM = {0};

//PRUEBA DEL DRIVER ADC
ADC_Config_t pruebaADC = {0};
BasicTimer_Handler_t handlerTIM5 = {0};

//Variables para pruebas

uint8_t sendMSG = 0;
char mensaje[64] = "HOLA\n";
char newChar = 0;
uint16_t duttyValue = 1000;
uint8_t teclaA = 0;
uint8_t adcIsComplete = 0;
uint16_t dataADC = 0;


int main(void){


    // Activamos el FPU
	SCB->CPACR |= (0xF << 20);

	RCC->CR &= ~(RCC_CR_HSEON);

	///Configuramos el pin A5 el cual se encargara del Led de estado

	handlerLedBlinky.pGPIOx                             = GPIOH;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinNumber      = PIN_1;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlinky.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlinky);
	GPIO_WritePin(&handlerLedBlinky, SET);

	//Configuramos el TIM2, para asì poder lograr el parpadeo en el led de estado
	handlerTIM2.ptrTIMx 							= TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode				= BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_period				= 250;
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

	USART_Config(&USART2Comm);



	pruebaADC.channel        = ADC_CHANNEL_0;
	pruebaADC.dataAlignment  = ADC_ALIGNMENT_RIGHT;
	pruebaADC.samplingPeriod = ADC_SAMPLING_PERIOD_84_CYCLES;
	pruebaADC.resolution     = ADC_RESOLUTION_12_BIT;
	adc_Config(&pruebaADC);



	writeMsg(&USART2Comm, mensaje);


    while(1){
    	if(newChar != '\0'){
    		if(newChar == 's'){
    			// Comenzar la conversion
    			startSingleADC();
    		} else if (newChar == 'p'){
    			newChar = '\0';
    		}
    		newChar = '\0';
    	}
    	if ( adcIsComplete == 1){

    		sprintf(mensaje, "data: %u\n", dataADC);
    		writeMsg(&USART2Comm, mensaje);
    		adcIsComplete = 0;
 	}
    }
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
	teclaA ++;
}


void usart2Rx_CallBack (void){
	newChar = getRxData();
	writeChar(&USART2Comm, newChar);
}

void adcComplete_Callback (void){
	dataADC = getADC();
	adcIsComplete = 1;
}

