/*
 * USARTxDriver.c
 *
 *  Created on: Apr 6, 2022
 *      Author: namontoy
 */

#include <stm32f4xx.h>
#include "USARTxDriver.h"
#include "PLLDriver.h"
#include "string.h"

/**
 * Configurando el puerto Serial...
 * Recordar que siempre se debe comenzar con activar la señal de reloj
 * del periferico que se está utilizando.
 */

uint8_t auxRxData = 0;
uint8_t dataDR = 0;
char *arregloDt = 0;
uint8_t Tx_index = 0;
uint8_t auxFn = 0;

void USART_Config(USART_Handler_t *ptrUsartHandler) {

	// Desactivamos las interrupciones globales mientras configuramos el sistema
	__disable_irq();

	/* 1. Activamos la señal de reloj que viene desde el BUS al que pertenece el periferico */
	/* Lo debemos hacer para cada uno de las pisbles opciones que tengamos (USART1, USART2, USART6) */
	/* 1.1 Configuramos el USART1 */
	if (ptrUsartHandler->ptrUSARTx == USART1) {
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	} else if (ptrUsartHandler->ptrUSARTx == USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	} else if (ptrUsartHandler->ptrUSARTx == USART6) {
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	}

	/* 2. Configuramos el tamaño del dato, la paridad y los bit de parada */
	/* En el CR1 estan parity (PCE y PS) y tamaño del dato (M) */
	/* Mientras que en CR2 estan los stopbit (STOP)*/
	/* Configuracion del Baudrate (registro BRR) */
	/* Configuramos el modo: only TX, only RX, o RXTX */
	/* Por ultimo activamos el modulo USART cuando todo esta correctamente configurado */

	// 2.1 Comienzo por limpiar los registros, para cargar la configuración desde cero
	ptrUsartHandler->ptrUSARTx->CR1 = 0;
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	// 2.2 Configuracion del Parity:
	// Verificamos si el parity esta activado o no
	// Tenga cuidado, el parity hace parte del tamaño de los datos...
	if (ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE) {
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PCE;

		// Verificamos si se ha seleccionado ODD or EVEN
		if (ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN) {
			// Es even, entonces cargamos la configuracion adecuada
			ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_PS);

		} else {
			// Si es "else" significa que la paridad seleccionada es ODD, y cargamos esta configuracion
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PS;
		}
	} else {
		// Si llegamos aca, es porque no deseamos tener el parity-check
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_PCE);
	}

	// 2.3 Configurpamos el tamaño del dato
	if (ptrUsartHandler->USART_Config.USART_datasize == USART_DATASIZE_8BIT) {
		if (ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_NONE) {
			// Deseamos trabajar con datos de 8 bits
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_M;
		} else {
			// Si la partidad esta activada debemos agregar un bir adicional
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_M;
		}
	} else {
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_M;
	}

	// 2.4 Configuramos los stop bits (SFR USART_CR2)
	switch (ptrUsartHandler->USART_Config.USART_stopbits) {
	case USART_STOPBIT_1: {
		// Debemoscargar el valor 0b00 en los dos bits de STOP
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(USART_CR2_STOP);
		break;
	}
	case USART_STOPBIT_0_5: {
		// Debemoscargar el valor 0b01 en los dos bits de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_0;
		break;
	}
	case USART_STOPBIT_2: {
		// Debemoscargar el valor 0b10 en los dos bits de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_1_5: {
		// Debemoscargar el valor 0b11 en los dos bits de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP;
		break;
	}
	default: {
		// En el casopor defecto seleccionamos 1 bit de parada
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(USART_CR2_STOP);
		break;
	}
	}

	// Revisamos si el PLL esta activo
	if (getConfigPLL() == PLL_80MHz_Frequency) {
		// 2.5 Configuracion del Baudrate (SFR USART_BRR) ( con 80MHz debido a que el PLL esta activo)
		if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600) {
			// El valor a cargar es 520.8333 -> Mantiza = 520,fraction = 0.8333
			// Mantiza = 520 = 0x208, fraction = 80 * 0.8333 = 67 = 43
			// la fraction al ser muy grande se le suma un carry a la mantiza, es decir
			// la matiza seria 212 y la fraction seria 3
			// Configurando el Baudrate generator para una velocidad de 9600bps
			ptrUsartHandler->ptrUSARTx->BRR = 0x20c3;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate
				== USART_BAUDRATE_19200) {
			// El valor a cargar es 260.4166 -> Mantiza = 260,fraction = 0.4166
			// Mantiza = 260 = 0x104, fraction = 80 * 0.4166 = 33 = 21
			// la fraction al ser muy grande se le suma un carry a la mantiza, es decir
			// la matiza seria 262 y la fraction seria 1
			ptrUsartHandler->ptrUSARTx->BRR = 0x1061;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate
				== USART_BAUDRATE_115200) {
			// El valor a cargar es 43.4027 -> Mantiza = 43,fraction = 0.4027
			// Mantiza = 43 = 0x2b, fraction = 80 * 0.4027 = 32 = 20
			// la fraction al ser muy grande se le suma un carry a la mantiza, es decir
			// la matiza seria 45 y la fraction seria 0
			ptrUsartHandler->ptrUSARTx->BRR = 0x02d0;
		}

	} else {
		// 2.5 Configuracion del Baudrate (SFR USART_BRR)
		// Ver tabla de valores (Tabla 73), Frec = 16MHz, overr = 0;
		if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600) {
			// El valor a cargar es 104.1875 -> Mantiza = 104,fraction = 0.1875
			// Mantiza = 104 = 0x68, fraction = 16 * 0.1875 = 3
			// Valor a cargar 0x0683
			// Configurando el Baudrate generator para una velocidad de 9600bps
			ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate
				== USART_BAUDRATE_19200) {
			// El valor a cargar es 52.0625 -> Mantiza = 52,fraction = 0.0625
			// Mantiza = 52 = 0x34, fraction = 16 * 0.1875 = 1
			// valor a cargar 0x0341
			ptrUsartHandler->ptrUSARTx->BRR = 0x0341;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate
				== USART_BAUDRATE_115200) {
			// El valor a cargar es 8,6875 -> Mantiza = 8,fraction = 0.6875
			// Mantiza = 8 = 0x08, fraction = 16 * 0.6875 = 11
			// valor a cargar 0x008B
			ptrUsartHandler->ptrUSARTx->BRR = 0x008B;
		}
	}

	// 2.6 Configuramos el modo: TX only, RX only, RXTX, disable
	switch (ptrUsartHandler->USART_Config.USART_mode) {
	case USART_MODE_TX: {
		// Activamos la parte del sistema encargada de enviar
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_RE);
		break;
	}
	case USART_MODE_RX: {
		// Activamos la parte del sistema encargada de recibir
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_TE);
		break;
	}
	case USART_MODE_RXTX: {
		// Activamos ambas partes, tanto transmision como recepcion
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;
		break;
	}
	case USART_MODE_DISABLE: {
		// Desactivamos ambos canales
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_RE);
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_TE);
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		break;
	}

	default: {
		// Actuando por defecto, desactivamos ambos canales
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_RE);
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(USART_CR1_TE);
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		break;
	}
	}

	// verificamos la activacion de la interrupcion Rx
	if (ptrUsartHandler->ptrUSARTx == USART1) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_RX_INTERRUPT_ENABLE) {
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;
			__NVIC_EnableIRQ(USART1_IRQn);
		} else {
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
		}
	}

	/* 1.2 Configuramos USART2 */
	else if (ptrUsartHandler->ptrUSARTx == USART2) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_RX_INTERRUPT_ENABLE) {
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;
			__NVIC_EnableIRQ(USART2_IRQn);
		} else {
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
		}
	}

	/* 1.3 Configuramos USART6 */
	else if (ptrUsartHandler->ptrUSARTx == USART6) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_RX_INTERRUPT_ENABLE) {
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;
			__NVIC_EnableIRQ(USART6_IRQn);
		} else {
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
		}
	}

	// verificamos la activacion de la interrupcion Tx
	if (ptrUsartHandler->ptrUSARTx == USART1) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_TX_INTERRUPT_ENABLE) {
			__NVIC_EnableIRQ(USART1_IRQn);
		}
	}

	/* 1.2 Configuramos USART2 */
	else if (ptrUsartHandler->ptrUSARTx == USART2) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_TX_INTERRUPT_ENABLE) {
			__NVIC_EnableIRQ(USART2_IRQn);
		}
	}

	/* 1.3 Configuramos USART6 */
	else if (ptrUsartHandler->ptrUSARTx == USART6) {
		if (ptrUsartHandler->USART_Config.USART_enable_IntRx
				== USART_TX_INTERRUPT_ENABLE) {
			__NVIC_EnableIRQ(USART6_IRQn);
		}
	}

	// 2.7 Activamos el modulo serial.
	if (ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE) {
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_UE;
	}

	//ACTIVAMOS INTERRUPCIONES GLOBALES
	__enable_irq();
}

/* funcion para escribir un solo char */
int writeChar(USART_Handler_t *ptrUsartHandler, char dataToSend) {
	while (!(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TXE)) {
		__NOP();
	}
	ptrUsartHandler->ptrUSARTx->DR = dataToSend;
	return dataToSend;
}

// FUncion para mandar un mensaje ( varios char)
void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSend) {
	while (!(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TXE)) {
		__NOP();
	}
	uint8_t count = 0;
	while (*(msgToSend + count) != 0) {
		writeChar(ptrUsartHandler, *(msgToSend + count));
		count++;
	}

}

uint8_t getRxData(void) {
	return auxRxData;
}

/* Handler de la interrupcion del USART
 * Acà deben estar todas las interrupciones asociadas
 */
void USART1_IRQHandler(void) {
	//Evaluamos si la interrupcion se dio por RX
	if ( USART1->SR & USART_SR_RXNE) {

		auxRxData = (uint8_t) USART1->DR;
		usart1Rx_CallBack();

	} else if ( USART1->SR & USART_SR_TXE) {

		if (auxFn == 0) {

			USART1->DR = dataDR;
			USART1->CR1 &= ~USART_CR1_TXEIE;

		} else if (auxFn == 1) {

			if (*(arregloDt + Tx_index) != '\0') {
				dataDR = *(arregloDt + Tx_index);
				USART1->DR = dataDR;
				Tx_index++;
				USART1->CR1 |= USART_CR1_TXEIE;
			} else if (USART1->SR & USART_SR_TC_Pos) {
				USART1->CR1 &= ~USART_CR1_TXEIE;
				//yaestoylibre
			}

		}
	}
}

//void USART2_IRQHandler (void){
//	//Evaluamos si la interrupcion se dio por RX
//	if ( USART2->SR & USART_SR_RXNE){
//		auxRxData = (uint8_t) USART2->DR;
//		usart2Rx_CallBack();
//	}
//	else if( USART2->SR & USART_SR_TXE_Msk) {
//		USART2->DR = dataDR;
//		Tx_index = 1;
//		USART2->DR = *(arregloDt+Tx_index);
//		Tx_index ++;
//		if (*(arregloDt+Tx_index) == 0){
//			USART2->CR1 &= ~USART_CR1_TXEIE;
//		}
//
//	}
//}

//void USART6_IRQHandler (void){
//	//Evaluamos si la interrupcion se dio por RX
//	if ( USART6->SR & USART_SR_RXNE){
//		auxRxData = (uint8_t) USART6->DR;
//		usart6Rx_CallBack();
//	}
//	else if( USART6->SR & USART_SR_TXE_Msk) {
//
//		USART6->DR = dataDR;
//		Tx_index = 1;
//		USART6->DR = *(arregloDt+Tx_index);
//		Tx_index ++;
//		if (*(arregloDt+Tx_index) == 0){
//			USART6->CR1 &= ~USART_CR1_TXEIE;
//		}
//	}
//
//}

int writeIntChar(USART_Handler_t *ptrUsartHandler, char dataToSend) {

	auxFn = 0;
	dataDR = dataToSend;
	ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TXEIE;

	return dataToSend;
}

void writeIntMsg(USART_Handler_t *ptrUsartHandler, char *msgToSend) {
		//entronuevodato
	auxFn = 1;
	arregloDt = msgToSend;
	ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TXEIE;

}

__attribute__((weak)) void usart1Rx_CallBack(void) {
	__NOP();
}

__attribute__((weak)) void usart2Rx_CallBack(void) {
	__NOP();
}

__attribute__((weak)) void usart6Rx_CallBack(void) {
	__NOP();
}


//get siestaocupadito
