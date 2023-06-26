/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Solucion Parcial
 *
 * Este proyecto se crea para poder programar un control con un Joystick, el cual
 * se va a encargar de enviar con comunicacion bluetooth las direcciones de movimiento para la
 * araña robot
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h>

#include <stm32f4xx.h>

#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
#include "PwmDriver.h"
#include "AdcDriver.h"
#include "SysTickDriver.h"


// Handler necesarios para el blinky
BasicTimer_Handler_t handlerTIM2       = {0};
GPIO_Handler_t       handlerLedBlinky  = {0};

// Handler necesarios para el USART
USART_Handler_t handlerUSART1Comm = {0};
GPIO_Handler_t  handlerUSARTPINTX = {0};
GPIO_Handler_t  handlerUSARTPINRX = {0};

//Handlers para la coversion ADC y correcto funcionamiento del Joystick
PWM_Handler_t  handlerPWMADC = {0};
ADC_Config_t   handlerADC    = {0};
GPIO_Handler_t handlerSW     = {0};
EXTI_Config_t  handlerExtiSW = {0};


// Variables necesarias para el corrrecto desarrollo de la tarea
char bufferData[64]             = {0};                                 //  Mensaje a imprimir cuando se inicie el codigo
uint16_t dataChannelX[1]        = {0};                             // Guardan los datas del canal 1 del ADC
uint16_t dataChannelY[1]        = {0};                             // Guardan los datos del canal 2 del ADC
uint8_t channelADC              = 0;
uint16_t countADC               = 0;
uint8_t flagADC                 = 0;
uint8_t flagSW                  = 0;
uint8_t countConv               = 0;
uint8_t flagDir                 = 0;

//Cabecera de las funciones utilizadas en el desarrollo de la tarea
void initialConfig (void);
void datosJoystick(void);


int main(void){

	initialConfig();

    while(1){
    	datosJoystick();
    	if (countConv == 10){
    		if(flagDir == 1){
    			writeMsg(&handlerUSART1Comm, "1");
    			flagDir = 0;
    		}else if (flagDir == 2){
    			writeMsg(&handlerUSART1Comm, "2");
    			flagDir = 0;
    		}else if(flagDir == 3){
    			writeMsg(&handlerUSART1Comm, "3");
    			flagDir = 0;
    		}else if(flagDir == 4){
    			writeMsg(&handlerUSART1Comm, "4");
    			flagDir = 0;
    		}
    	}
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
	USART_Config(&handlerUSART1Comm);

	// Configuracion de los pines RX y TX del USART1
	handlerUSARTPINTX.pGPIOx                            = GPIOA;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinNumber     = PIN_9;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_ALTFN;
	handlerUSARTPINTX.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINTX);

	handlerUSARTPINRX.pGPIOx                            = GPIOA;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinNumber     = PIN_10;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_ALTFN;
	handlerUSARTPINRX.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	GPIO_Config(&handlerUSARTPINRX);


	// Configuramos el PWM
	handlerPWMADC.ptrTIMx                             = TIM3;
	handlerPWMADC.config.channel                      = PWM_CHANNEL_1;
	handlerPWMADC.config.duttyCicle                   = 10;
	handlerPWMADC.config.periodo                      = 100;
	handlerPWMADC.config.prescaler                    = 100;
	pwm_Config(&handlerPWMADC);
	enableOutput(&handlerPWMADC);
	startPwmSignal(&handlerPWMADC);


	// Se configura el ADC
	handlerADC.mode                                = ADC_MULTI_CHANNEL;
	handlerADC.dataAlignment                       = ADC_ALIGNMENT_RIGHT;
	handlerADC.samplingPeriod                      = ADC_SAMPLING_PERIOD_112_CYCLES;
	handlerADC.resolution                          = ADC_RESOLUTION_12_BIT;
	handlerADC.channel                             = ADC_CHANNEL_0;
	handlerADC.channel2                            = ADC_CHANNEL_1;
	adc_Config(&handlerADC);

	//Se configura el EXTI que se encargara del SW del Joystick con su respectivo PIN
	handlerSW.pGPIOx                             = GPIOC;
	handlerSW.GPIO_PinConfig.GPIO_PinNumber      = 3;
	handlerSW.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	handlerSW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	GPIO_Config(&handlerSW);

	handlerExtiSW.pGPIOHandler  = &handlerSW;
	handlerExtiSW.edgeType      = EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiSW);

}

void datosJoystick(void) {

	// En esta funcion se revisan los valores de la conversion ADC del joystick, con loc ual se levantaran una bandera con cierto valor
	// dependiendo del rango en el que este
	if (dataChannelY[0] >= 2500 && dataChannelX[0] < 2500
			&& dataChannelX[0] > 1500) {
		flagDir = 1;
	} else if (dataChannelY[0] <= 1500 && dataChannelX[0] < 2500
			&& dataChannelX[0] > 1500) {
		flagDir = 2;
	} else if (dataChannelX[0] <= 1500 && dataChannelY[0] < 2500
			&& dataChannelY[0] > 1500) {
		flagDir = 3;
	} else if (dataChannelX[0] >= 2500 && dataChannelY[0] < 2500
			&& dataChannelY[0] > 1500) {
		flagDir = 4;
	}

}
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
	countConv ++;
	// Activamos la bandera la cual nos va a ayudar a tomar los datos
	// de la conversion ADC
	flagADC  = 1;
	if(countConv > 10){
		countConv = 0;
	}
}

void adcComplete_Callback(void){

	// Esperamos que se active la bandera que nos ayuda para guardar los datos
	if(flagADC){
		// Agregamos los datos a los arreglos correspondientes de cada canal
		if (channelADC==0){
			dataChannelX[0]=getADC();
		}
		else{
			dataChannelY[0]=getADC();
		}

	channelADC++;
	if(countADC>= 1){
		flagADC=0;

		}
	if (channelADC>=2){
		channelADC=0;
	}
	}
}

void callback_extInt3(void){
	// Cambiamos el valor de la bandera al oprimir el boton del JoyStick para si tener dos estados
	// de la araña uno "Apagado" y otro " Encendido "
	flagSW = !flagSW;
	if(flagSW){
		writeMsg(&handlerUSART1Comm, "u");
	}else if (flagSW == 0){
		writeMsg(&handlerUSART1Comm, "d");
	}
}


