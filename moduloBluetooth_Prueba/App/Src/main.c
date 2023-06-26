/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : moduloBluetooth_Prueba
 * Direccion del primer modulo Bluetooth = DDR:0022:09:010CE6;
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h>

#include <stm32f4xx.h>

#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"


// Handler necesarios para el blinky
BasicTimer_Handler_t handlerTIM2       = {0};
GPIO_Handler_t       handlerLedBlinky  = {0};

// Handler necesarios para el USART1
USART_Handler_t handlerUSART1Comm = {0};
GPIO_Handler_t  handlerUSARTPINTX1 = {0};
GPIO_Handler_t  handlerUSARTPINRX1 = {0};



// Variables necesarias para el corrrecto desarrollo de la tarea
char bufferData[64]        = {0} ;                            //  Mensaje a imprimir cuando se inicie el codigo
char rxChar                = 0 ;
char rxChar6               = 0 ;
char bufferReception[64]   = {0} ;
uint16_t counterReception  = 0 ;
uint16_t counterReception6 = 0 ;
uint8_t stringComplete     = 0 ;

//Cabecera de las funciones utilizadas en el desarrollo de la tarea
void initialConfig (void);
void cmdIngresoConsola (void);
void parseCommand (char *buffer);


int main(void){

	initialConfig();

    while(1){
    	}
}

void initialConfig (void){


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
	handlerTIM2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_1ms;
	handlerTIM2.TIMx_Config.TIMx_period				= 250;
	BasicTimer_Config(&handlerTIM2);

	// Configuracion del USART1
	handlerUSART1Comm.ptrUSARTx                       = USART1;
	handlerUSART1Comm.USART_Config.USART_baudrate     = USART_BAUDRATE_9600;
	handlerUSART1Comm.USART_Config.USART_datasize     = USART_DATASIZE_8BIT;
	handlerUSART1Comm.USART_Config.USART_parity       = USART_PARITY_NONE;
	handlerUSART1Comm.USART_Config.USART_mode         = USART_MODE_RXTX;
	handlerUSART1Comm.USART_Config.USART_stopbits     = USART_STOPBIT_1;
	handlerUSART1Comm.USART_Config.USART_enable_IntRx = USART_RX_INTERRUPT_ENABLE;
	USART_Config(&handlerUSART1Comm);

	// Configuracion de los pines RX y TX del USART1
	handlerUSARTPINTX1.pGPIOx                            = GPIOA;
	handlerUSARTPINTX1.GPIO_PinConfig.GPIO_PinNumber     = PIN_9;
	handlerUSARTPINTX1.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_ALTFN;
	handlerUSARTPINTX1.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINTX1);

	handlerUSARTPINRX1.pGPIOx                            = GPIOA;
	handlerUSARTPINRX1.GPIO_PinConfig.GPIO_PinNumber     = PIN_10;
	handlerUSARTPINRX1.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_ALTFN;
	handlerUSARTPINRX1.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINRX1);



}



void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
	writeChar(&handlerUSART1Comm, 'a');
}

void usart1Rx_CallBack (void){
	rxChar = getRxData();
}

void usart6Rx_CallBack (void){
	rxChar6 = getRxData();
}

