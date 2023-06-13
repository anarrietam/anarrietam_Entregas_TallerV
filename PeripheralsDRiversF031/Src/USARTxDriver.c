/*
 * USARTxDriver.c
 *
 *  Created on: Jun 5, 2023
 *      Author: andres
 */

#include <stm32f0xx.h>
#include "USARTxDriver.h"

/**
 * Configurando el puerto Serial...
 * Recordar que siempre se debe comenzar con activar la señal de reloj
 * del periferico que se está utilizando.
 */

uint8_t auxRxData = 0;


void USART_Config(USART_Handler_t *ptrUsartHandler){

	// Desactivamos las interrupciones globales mientras configuramos el sistema
	__disable_irq();

	/* 1. Activamos la señal de reloj que viene desde el BUS al que pertenece el periferico */
    /* 1.1 Configuramos el USART1 */
	RCC->APB2ENR |= RCC_APB2RSTR_USART1RST;


	/* 2. Configuramos el tamaño del dato, la paridad y los bit de parada */
	/* En el CR1 estan parity (PCE y PS) y tamaño del dato (M) */
	/* Mientras que en CR2 estan los stopbit (STOP)*/
	/* Configuracion del Baudrate (registro BRR) */
	/* Configuramos el modo: only TX, only RX, o RXTX */
	/* Por ultimo activamos el modulo USART cuando todo esta correctamente configurado */

	// 2.1 Comienzo por limpiar los registros, para cargar la configuración desde cero
	USART1->CR1 = 0;
	USART1->CR2 = 0;

	// 2.2 Configuracion del Parity:
	// Verificamos si el parity esta activado o no
    // Tenga cuidado, el parity hace parte del tamaño de los datos...
	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE){
		USART1->CR1 |= USART_CR1_PCE;

		// Verificamos si se ha seleccionado ODD or EVEN
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN){
			// Es even, entonces cargamos la configuracion adecuada
			USART1->CR1 &= ~(USART_CR1_PS);

		}else{
			// Si es "else" significa que la paridad seleccionada es ODD, y cargamos esta configuracion
			USART1->CR1 |= USART_CR1_PS;
		}
	}else{
		// Si llegamos aca, es porque no deseamos tener el parity-check
		USART1->CR1 &= ~(USART_CR1_PCE);
	}

	// 2.3 Configurpamos el tamaño del dato
	if (ptrUsartHandler->USART_Config.USART_datasize == USART_DATASIZE_8BIT){
		USART1->CR1 &= ~(USART_CR1_M);
	} else{
		USART1->CR1 |= USART_CR1_M;
	}

	// 2.4 Configuramos los stop bits (SFR USART_CR2)
	switch(ptrUsartHandler->USART_Config.USART_stopbits){
	case USART_STOPBIT_1: {
		// Debemoscargar el valor 0b00 en los dos bits de STOP
		USART1->CR2 &= ~(USART_CR2_STOP);
		break;
	}
	case USART_STOPBIT_0_5: {
		// Debemoscargar el valor 0b01 en los dos bits de STOP
		USART1->CR2 |= USART_CR2_STOP_0;
		break;
	}
	case USART_STOPBIT_2: {
		// Debemoscargar el valor 0b10 en los dos bits de STOP
		USART1->CR2 |= USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_1_5: {
		// Debemoscargar el valor 0b11 en los dos bits de STOP
		USART1->CR2 |= USART_CR2_STOP;
		break;
	}
	default: {
		// En el casopor defecto seleccionamos 1 bit de parada
		USART1->CR2 &= ~(USART_CR2_STOP);
		break;
	}
	}

	// configuramos un oversampling de 16
	USART1->CR1 &= ~(USART_CR1_OVER8);

	// 2.5 Configuracion del Baudrate (SFR USART_BRR)
	if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600){
		USART1->BRR = 80000/96;
	}

	else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200) {
		USART1->BRR = 80000/192;
	}

	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
		USART1->BRR = 80000/1152;
	}

	// 2.6 Configuramos el modo: TX only, RX only, RXTX, disable
	switch(ptrUsartHandler->USART_Config.USART_mode){
	case USART_MODE_TX:
	{
		// Activamos la parte del sistema encargada de enviar
		USART1->CR1 |= USART_CR1_TE;
		USART1->CR1 &= ~(USART_CR1_RE);
		break;
	}
	case USART_MODE_RX:
	{
		// Activamos la parte del sistema encargada de recibir
		USART1->CR1 |= USART_CR1_RE;
		USART1->CR1 &= ~(USART_CR1_TE);
		break;
	}
	case USART_MODE_RXTX:
	{
		// Activamos ambas partes, tanto transmision como recepcion
		USART1->CR1 |= USART_CR1_TE;
		USART1->CR1 |= USART_CR1_RE;
		break;
	}
	case USART_MODE_DISABLE:
	{
		// Desactivamos ambos canales
		USART1->CR1 &= ~(USART_CR1_RE);
		USART1->CR1 &= ~(USART_CR1_TE);
		USART1->CR1 &= ~USART_CR1_UE;
		break;
	}

	default:
	{
		// Actuando por defecto, desactivamos ambos canales
		USART1->CR1 &= ~(USART_CR1_RE);
		USART1->CR1 &= ~(USART_CR1_TE);
		USART1->CR1 &= ~USART_CR1_UE;
		break;
	}
	}


	// verificamos la activacion de la interrupcion Rx
	if (ptrUsartHandler->USART_Config.USART_enable_IntRx == USART_RX_INTERRUPT_ENABLE) {
		USART1->CR1 |= USART_CR1_RXNEIE;
		__NVIC_EnableIRQ(USART1_IRQn);
	} else {
		USART1->CR1 &= ~USART_CR1_RXNEIE;
	}




		// 2.8 Activamos el modulo serial.
	if (ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE) {
		USART1->CR1 |= USART_CR1_UE;
	}

	//ACTIVAMOS INTERRUPCIONES GLOBALES
	__enable_irq();
}

/* funcion para escribir un solo char */
int writeChar(USART_Handler_t *ptrUsartHandler, char dataToSend ){
	while( !(USART1->ISR & USART_ISR_TC)){
		__NOP();
	}
	USART1->TDR  = dataToSend;
	return dataToSend;
}

// FUncion para mandar un mensaje ( varios char)
void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSend ){
	while( !(USART1->ISR & USART_ISR_TC)){
			__NOP();
		}
	uint8_t count = 0;
	while(*(msgToSend+count) != 0){
		writeChar(ptrUsartHandler, *(msgToSend+count));
		count++;
	}

}

uint8_t getRxData(void){
	return auxRxData;
}

/* Handler de la interrupcion del USART
 * Acà deben estar todas las interrupciones asociadas
 */
void USART1_IRQHandler (void){
	//Evaluamos si la interrupcion se dio por RX
	if ( (USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){
		auxRxData = (uint8_t) USART1->RDR;
		usart1Rx_CallBack();
	}

}

// Callbacks de las interrupciones en RX
__attribute__((weak)) void usart1Rx_CallBack (void){
	__NOP();
}



