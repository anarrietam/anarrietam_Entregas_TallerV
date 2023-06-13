/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Solucion Parcial
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <stm32f4xx.h>

#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"
#include "I2CDriver.h"
#include "PLLDriver.h"
#include "RTCDriver.h"
#include "PwmDriver.h"
#include "AdcDriver.h"

#include "arm_math.h"

// Handler necesarios para el blinky
BasicTimer_Handler_t handlerTIM2 = {0};
GPIO_Handler_t handlerLedBlinky  = {0};

// Handler necesarios para el USART
USART_Handler_t USART1Comm       = {0};
GPIO_Handler_t handlerUSARTPINTX = {0};
GPIO_Handler_t handlerUSARTPINRX = {0};

// Handler para el desarrollo del I2C Acelerometro
I2C_Handler_t handlerAcelerometro       = {0};
GPIO_Handler_t handlerI2C_SDA           = {0};
GPIO_Handler_t handlerI2C_SCL           = {0};
BasicTimer_Handler_t handlerTimMuestreo = {0};

// Handler para la activacion del PLL
PLL_Handler PLL_Handler100MHz  = {0};

//Handler para el pin MCO1
GPIO_Handler_t MC01_Handler = {0};

//Handler para la fecha y hora con el RTC
RTC_Handler_t handlerFecha_Hora = {0};

//Handlers para la coversion ADC
PWM_Handler_t handlerPWMADC = {0};
ADC_Config_t handlerADC     = {0};

// Variables necesarias para el corrrecto desarrollo de la tarea
char rxChar                     = 0;                               // Variable para ingresar un char por consola
char bufferData[]               = "Menu de comandos: \nIngresando por consola el comando -help se puede ver la lista de todos los comandos\n";    //  Mensaje a imprimir cuando se inicie el codigo
char bufferReception[64]        = {0};                             // String donde se guardara los mensajes que el usuario mande por consola
uint8_t i2cBuffer               = {0};                             // Variable donde se guardan los datos del acelerometro
uint16_t countMuestreo          = 0;                               // Contador que nos ayudara con el muestreo
uint8_t startTimer              = 0;
uint8_t counterReception        = 0;                               // Contador que lleva la cuenta del numero del char ingresados por consola del usuario
bool stringComplete             = 0;
float dataChannel1[258]         = {0};                             // Guardan los datas del canal 1 del ADC
float dataChannel2[258]         = {0};                             // Guardan los datos del canal 2 del ADC
uint8_t channelADC              = 0;
uint16_t countADC               = 0;
uint8_t flagADC                 = 0;
float convFreq                  = 0;
uint8_t flagReadData            = 0;
uint8_t flagSaveData            = 0;
uint8_t ejeZ_Low                = 0;
uint8_t ejeZ_High               = 0;
int16_t ejeZ                    = 0;
int16_t countAcelerometro       = 0;
float32_t dataAcelerometro[513] = {0};

// Variables que nos ayudan con la utilizacion de los comandos
char cmdo[64]             = {0};
unsigned int primerParam  = 0;
unsigned int segundoParam = 0;

// Variables para el uso de las librerias CMSIS para la FFT
arm_status iniciarFFT  = ARM_MATH_ARGUMENT_ERROR;
uint16_t tamañoArreglo = 512;
arm_rfft_fast_instance_f32 config_Rfft_fast_f32;
float32_t dataFourier[513] = {0};


// Macros utilizadas para  configurar el acelerometro
#define ACCEL_ADDRESS          	 0x1D
#define ACCEL_DATAX0			 50
#define ACCEL_DATAX1             51
#define ACCEL_DATAY0             52
#define ACCEL_DATAY1             53
#define ACCEL_DATAZ0             54
#define ACCEL_DATAZ1             55

#define POWER_CTL                45
#define DEVID                	 0

#define BW_RATE					 44	// Valor para cambiar el muestreo del acelerometro
#define NewFRequency             0xE // Valor de la nueva frecuencia para el muestreo

//Cabecera de las funciones utilizadas en el desarrollo de la tarea
void initialConfig (void);
void cmdIngresoConsola (void);
void parseCommand (char *bufferRecibido);
void leerAcelerometro (void);

int main(void){

	initialConfig();

	// Se imprime el mensaje de inicio
	writeMsg(&USART1Comm, bufferData);

	// Se reinicia el acelerometro para así poder comenzar una buena toma de datos
	i2c_writeSingleRegister(&handlerAcelerometro, POWER_CTL , 0x08);

	//El acelerometro toma muestras por defecto a 100Hz, pero toca subirle la frecuancia, por lo cual al ponemos a
	i2c_writeSingleRegister(&handlerAcelerometro, BW_RATE , 0xE);


    while(1){
    	cmdIngresoConsola();
    	leerAcelerometro();
    	}
}

void initialConfig (void){

	// Activamos el FPU
	SCB->CPACR |= (0xF << 20);

	// Para utilizar el pim H1 como el necargado del LED de estado debemos apagar el HSE
	RCC->CR &= ~(RCC_CR_HSEON);

	// Configuramos el HSI
	configHSI();

	// Se configura el PLL en 100 MHz
	PLL_Handler100MHz.PLL_Frequency  = PLL_100MHz_Frequency;
	configPll(&PLL_Handler100MHz);


	///Configuramos el pin H1 el cual se encargara del Led de estado

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
	handlerTIM2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_100MHz_100us;
	// Al trabjar con 100 us, para que el blinky sea de 250 ms, el periodo debe ser de 2500
	handlerTIM2.TIMx_Config.TIMx_period				= 2500;
	BasicTimer_Config(&handlerTIM2);

	// Configuracion del USART1
	USART1Comm.ptrUSARTx                       = USART1;
	USART1Comm.USART_Config.USART_baudrate     = USART_BAUDRATE_115200;
	USART1Comm.USART_Config.USART_datasize     = USART_DATASIZE_8BIT;
	USART1Comm.USART_Config.USART_parity       = USART_PARITY_NONE;
	USART1Comm.USART_Config.USART_mode         = USART_MODE_RXTX;
	USART1Comm.USART_Config.USART_stopbits     = USART_STOPBIT_1;
	USART1Comm.USART_Config.USART_enable_IntRx = USART_RX_INTERRUPT_ENABLE;
	USART_Config(&USART1Comm);

	// COnfiguracion de los pines RX y TX del USART1
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

	// Configuracion I2C Acelerometro
	handlerAcelerometro.ptrI2Cx           = I2C1;
	handlerAcelerometro.modeI2C           = I2C_MODE_FM;
	handlerAcelerometro.slaveAddress      = ACCEL_ADDRESS;
	handlerAcelerometro.mainClock         = MAIN_CLOCK_100_MHz_FOR_I2C;
	handlerAcelerometro.maxI2C_FM         = I2C_MAX_RISE_TIME_FM_100MHz;
	handlerAcelerometro.modeI2C_FM        = I2C_MODE_FM_SPEED_400KHz_100MHz;
	i2c_config(&handlerAcelerometro);

	//COnfiguracion de los pines SDA Y SCL para el Acelerometro
	handlerI2C_SCL.pGPIOx                             = GPIOB ;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinNumber      = PIN_8 ;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerI2C_SCL.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	GPIO_Config(&handlerI2C_SCL);

	handlerI2C_SDA.pGPIOx                             = GPIOB ;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinNumber      = PIN_9 ;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerI2C_SDA.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	GPIO_Config(&handlerI2C_SDA);

	// Se configura un Timer para el muestreo
	handlerTimMuestreo.ptrTIMx                          = TIM5;
	handlerTimMuestreo.TIMx_Config.TIMx_mode            = BTIMER_MODE_UP;
	handlerTimMuestreo.TIMx_Config.TIMx_speed           = BTIMER_SPEED_100MHz_100us;
	handlerTimMuestreo.TIMx_Config.TIMx_period          = 50;
	handlerTimMuestreo.TIMx_Config.TIMx_interruptEnable = BTIMER_INTERRUPT_ENABLE;
	BasicTimer_Config(&handlerTimMuestreo);

	// Configuracion del pin MC01
	MC01_Handler.pGPIOx                               = GPIOA;
	MC01_Handler.GPIO_PinConfig.GPIO_PinNumber        = PIN_8;
	MC01_Handler.GPIO_PinConfig.GPIO_PinMode          = GPIO_MODE_ALTFN;
	MC01_Handler.GPIO_PinConfig.GPIO_PinOPType        = GPIO_OTYPE_PUSHPULL;
	MC01_Handler.GPIO_PinConfig.GPIO_PinPuPdControl   = GPIO_PUPDR_NOTHING;
	MC01_Handler.GPIO_PinConfig.GPIO_PinSpeed         = GPIO_OSPEED_FAST;
	MC01_Handler.GPIO_PinConfig.GPIO_PinAltFunMode    = AF0;
	GPIO_Config(&MC01_Handler);

	// Configuramos el PWM
	handlerPWMADC.ptrTIMx                             = TIM3;
	handlerPWMADC.config.channel                      = PWM_CHANNEL_1;
	handlerPWMADC.config.duttyCicle                   = 10;
	handlerPWMADC.config.periodo                      = 100;
	handlerPWMADC.config.prescaler                    = 100;
	pwm_Config(&handlerPWMADC);
	enableOutput(&handlerPWMADC);
	startPwmSignal(&handlerPWMADC);

	// Se configura el handler del RTC
	handlerFecha_Hora.RTC_format                     = MODE_24HOUR;
	handlerFecha_Hora.RTC_Segundos                   = 0;
	handlerFecha_Hora.RTC_Minutos                    = 0;
	handlerFecha_Hora.RTC_Hora                       = 0;
	handlerFecha_Hora.RTC_Dias                       = 1;
	handlerFecha_Hora.RTC_Meses                      = 1;
	handlerFecha_Hora.RTC_Años                       = 2023;
	configRCT(&handlerFecha_Hora);

	// Se configura el ADC
	handlerADC.mode                                = ADC_MULTI_CHANNEL;
	handlerADC.dataAlignment                       = ADC_ALIGNMENT_RIGHT;
	handlerADC.samplingPeriod                      = ADC_SAMPLING_PERIOD_112_CYCLES;
	handlerADC.resolution                          = ADC_RESOLUTION_12_BIT;
	handlerADC.channel                             = ADC_CHANNEL_0;
	handlerADC.channel2                            = ADC_CHANNEL_1;
	adc_Config(&handlerADC);


}

void cmdIngresoConsola (void){

	// Por cada char diferente al carcater nulo, estos se agregan a un string
	if ( rxChar != '\0'){
		bufferReception[counterReception] = rxChar;
		counterReception++;

		// revisar si el char ingresado es un @, cuando esto pase decimos que ya el comando esta
		// ingresado por completo
		 if ( rxChar == '\r'){
			 stringComplete =true;

			 // Agregamos un caracter nulo al final del string
			 bufferReception[counterReception] = '\0';

			 // hacemos el contador se vuelva 0
			 counterReception = 0;
		 }
		 rxChar = '\0';
	}

	// Revisamos el estado de la bandera stringComplete, si esta en 1
	// llamamos la funcion parseCommands que se encarga de revisar el string que ingreso el usuario
	// para ver a que comando corresponde
	if(stringComplete){
		parseCommand (bufferReception);
		stringComplete = false;
	}

}

void parseCommand (char *bufferRecibido){

	sscanf(bufferRecibido,"%s %u %u ",cmdo,&primerParam,&segundoParam);

	// Comprobamos el primer comando que de debe ser -help
	if (strcmp(cmdo,"help") == 0){

		// Se imprime la lista de comando para que el usuario pueda elegir entre ellos
		writeMsg(&USART1Comm, "Lista de comandos: \n");
		writeMsg(&USART1Comm, " 1) ClockSel ----> Con este comando podemos elegir que reloj va a ser visuzalido en el pin MC01 \n") ;
		writeMsg(&USART1Comm, "   a) ClockSel 1 ---> Clock HSI \n");
		writeMsg(&USART1Comm, "   b) ClockSel 2 ---> Clock LSE \n");
		writeMsg(&USART1Comm, "   c) ClockSel 3 ---> Clock PLL \n");
		writeMsg(&USART1Comm, " 2) ClockPreScal ----> Con este comando podemos elegir el prescaler configurado en el pin MC01 \n") ;
		writeMsg(&USART1Comm, "   a) ClockPreScal 1 ---> No Prescaler \n");
		writeMsg(&USART1Comm, "   b) ClockPreScal 2 ---> Prescaler dividido por 2 \n");
		writeMsg(&USART1Comm, "   c) ClockPreScal 3 ---> Prescaler dividido por 3 \n");
		writeMsg(&USART1Comm, "   d) ClockPreScal 4 ---> Prescaler dividido por 4 \n");
		writeMsg(&USART1Comm, "   e) ClockPreScal 5 ---> Prescaler dividido por 5 \n");
		writeMsg(&USART1Comm, " 3) setTime A B ---> Este comando se encarga de configurar una hora que elija el usuario, siendo \n");
		writeMsg(&USART1Comm, "                     A las horas y B los minutos \n");
		writeMsg(&USART1Comm, " 4) setDate A B ---> Este comando se encarga de configurar una fecha que elija el horario, siendo \n");
		writeMsg(&USART1Comm, "                     A un numero de dia y B un mes (El año esta configurado como 2023) \n");
		writeMsg(&USART1Comm, " 5) getTime ---> Imprime el valor de tiempo que se encuentre en los registros correspondientes\n");
		writeMsg(&USART1Comm, " 6) getDate ---> Imprime la fecha que se encuentre en los registros correspondientes\n");
		writeMsg(&USART1Comm, " 7) freqMuestreo ---> Seleccionamos la frecuencia deseada para hacer el muestreo\n");
		writeMsg(&USART1Comm, " 8) dataADC ---> Imprime los datos tomados en la conversion Analoga-Digital\n ");
		writeMsg(&USART1Comm, "9)  dataAcelerometro --> Iniciar captura de datos del acelerometro\n");
		writeMsg(&USART1Comm, "10) FFTAcelerometro --> Mostrar la FFT de los datos tomados por el acelerometro \n ");

	} else if(strcmp(cmdo,"ClockSel") == 0){
		if(primerParam == 1){
			// Configuramos el registo encargado del MC01 para que no muestra la señal del HSI
				RCC->CFGR &= ~( RCC_CFGR_MCO1_Msk);
				RCC->CFGR |= (0b00 << RCC_CFGR_MCO1_Pos);
				writeMsg(&USART1Comm, "Se seleeciono el HSI CLock \n");
		}else if(primerParam == 2){
			// Configuramos el registo encargado del MC01 para que no muestra la señal del LSE
				RCC->CFGR &= ~( RCC_CFGR_MCO1_Msk);
				RCC->CFGR |= (0b01 << RCC_CFGR_MCO1_Pos);
				writeMsg(&USART1Comm, "Se seleeciono el LSE CLock \n");
		}else if(primerParam == 3){
			// Configuramos el registo encargado del MC01 para que no muestra la señal del PLL
				RCC->CFGR &= ~( RCC_CFGR_MCO1_Msk);
				RCC->CFGR |= (0b11 << RCC_CFGR_MCO1_Pos);
				writeMsg(&USART1Comm, "Se seleeciono el PLL CLock \n");
		}else{
			writeMsg(&USART1Comm, "Comando Erroneo. Revisar el listado de comandos\n");
		}

	}else if(strcmp(cmdo,"ClockPreScal") == 0){
		if(primerParam == 1){
			// No configuramos ningun valor para el prescaler
			RCC->CFGR &= ~( RCC_CFGR_MCO1PRE_Msk);
			RCC->CFGR |= (0b000<< RCC_CFGR_MCO1PRE_Pos);
			writeMsg(&USART1Comm, "No hay Prescaler \n");
		}else if(primerParam == 2){
			// Configuramos el prescaler en 2
			RCC->CFGR &= ~( RCC_CFGR_MCO1PRE_Msk);
			RCC->CFGR |= (0b100<< RCC_CFGR_MCO1PRE_Pos);
			writeMsg(&USART1Comm, " Prescaler en 2 \n");
		}else if(primerParam == 3){
			// Configuramos el prescaler en 3
			RCC->CFGR &= ~( RCC_CFGR_MCO1PRE_Msk);
			RCC->CFGR |= (0b101<< RCC_CFGR_MCO1PRE_Pos);
			writeMsg(&USART1Comm, " Prescaler en 3 \n");
		}else if(primerParam == 4){
			// Configuramos el prescaler en 4
			RCC->CFGR &= ~( RCC_CFGR_MCO1PRE_Msk);
			RCC->CFGR |= (0b110<< RCC_CFGR_MCO1PRE_Pos);
			writeMsg(&USART1Comm, " Prescaler en 4 \n");
		}else if(primerParam == 5){
			// Configuramos el prescaler en 5
			RCC->CFGR &= ~( RCC_CFGR_MCO1PRE_Msk);
			RCC->CFGR |= (111<< RCC_CFGR_MCO1PRE_Pos);
			writeMsg(&USART1Comm, " Prescaler en 5 \n");
		}else{
			writeMsg(&USART1Comm, "Comando Erroneo. Revisar el listado de comandos\n");
		}
	} else if((strcmp(cmdo,"setTime"))== 0){
		// Revisamos que las horas esten entre 0 y 23
		if ( (0 <= primerParam) && (primerParam <= 23)){
			handlerFecha_Hora.RTC_Hora = primerParam;
					// se revisa que los minutos esten entre 0 y 59
					if ( (0 <= segundoParam) && (segundoParam <= 59)){
						handlerFecha_Hora.RTC_Minutos = segundoParam;
						configRCT(&handlerFecha_Hora);
						writeMsg(&USART1Comm, " Hora actualizada\n");
					} else {
						writeMsg(&USART1Comm, " Comando Erroneo. Los minutos deben estar entre 0 y 59\n");
					}
		} else {
			writeMsg(&USART1Comm, " Comando Erroneo. Las horas deben estar entre 0 y 23\n");
		}

	} else if((strcmp(cmdo,"setDate"))== 0){
		// Revisamos que los dias esten entre 1 y 31
		if ( (1 <= primerParam) && (primerParam <= 31)){
			handlerFecha_Hora.RTC_Dias = primerParam;
					// se revisa que los meses esten entre 1 y 12
					if ( (1 <= segundoParam) && (segundoParam <= 12)){
						handlerFecha_Hora.RTC_Meses = segundoParam;
						configRCT(&handlerFecha_Hora);
						writeMsg(&USART1Comm, " Fecha actualizada\n");
					} else {
						writeMsg(&USART1Comm, " Comando Erroneo. Los meses deben ser entre 1 y 12\n");
					}
		} else {
			writeMsg(&USART1Comm, " Comando Erroneo. Los dias deben estar entre 1 y 31\n");
		}

	}else if((strcmp(cmdo,"getTime"))== 0){
		// Imprimimos la hora
		writeMsg(&USART1Comm, "La hora es: ");
		sprintf(bufferData, " %u%u : %u%u : %u%u \n", read_time()[5],read_time()[4],read_time()[3],read_time()[2],read_time()[1],read_time()[0]);
		writeMsg(&USART1Comm, bufferData);

	}else if((strcmp(cmdo,"getDate"))== 0){
		// Imprimimos la fecha
		writeMsg(&USART1Comm, "La fecha es: ");
		sprintf(bufferData, " %u%u / %u / 20%u%u \n", read_date()[1],read_date()[0],read_date()[2],read_date()[4],read_date()[3]);
		writeMsg(&USART1Comm, bufferData);

	}else if((strcmp(cmdo,"freqMuestreo")) == 0){
		if( (8000 <= primerParam) && (primerParam <= 25000)){
			// Convertimos la velocidad ingresada por el usuario a frecuencia
			convFreq = (1000000/primerParam);

			// Se redondea al proximo entero
			convFreq = round(convFreq);

			// Se actualiza la nueva frecuencia con un duttycicle igual a la mitad de la nueva frecuencia
			updateFrequency(&handlerPWMADC, convFreq);
			updateDuttyCycle(&handlerPWMADC, convFreq/2);
			writeMsg(&USART1Comm, "Velocidad del muestre actualizada\n");

		}else {
			writeMsg(&USART1Comm, "Comando Erroneo. POr favor ingresar una frecuencia entre 8000 y 25000\n");
		}
	}else if ((strcmp(cmdo,"dataADC"))==0){
		writeMsg(&USART1Comm, "Cargando datos de la conversion\n");
		flagADC = 1;
		while(flagADC){
			__NOP();
		}
		// Se imprimen todos los datos del arreglo
		for (uint16_t i = 1; i < 257; i++) {
			sprintf(bufferData, "Channel 1= %.2f V ; Channel 2= %.2f V\n", dataChannel1[i] * (3.3f / 4095.f),dataChannel2[i] * (3.3f / 4095.f));
			writeMsg(&USART1Comm, bufferData);
		}
	} else if((strcmp(cmdo,"dataAcelerometro")) == 0){
		writeMsg(&USART1Comm, "Iniciando toma de datos del Acelerometro:\n");
		flagSaveData = 1;

	}else if((strcmp(cmdo,"FFTAcelerometro")) == 0){
		int i = 0;
		int j = 0;
		writeMsg(&USART1Comm, "Calculando la FFT...\n");
		iniciarFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, tamañoArreglo);
		if(iniciarFFT == ARM_MATH_SUCCESS){
			arm_rfft_fast_f32(&config_Rfft_fast_f32, dataAcelerometro,dataFourier,0);
			arm_abs_f32(dataFourier, dataAcelerometro, tamañoArreglo);
			uint32_t max = 0;
			float maxFFT = dataAcelerometro[i];
			for( i=1; i< tamañoArreglo; i++){
				if(i%2){
					if(dataAcelerometro[i] > maxFFT){
						maxFFT = dataAcelerometro[i];
						max = j;
					}
					sprintf(bufferData," Dato : %#.6f \n", 2*dataAcelerometro[i]);
					writeMsg(&USART1Comm, bufferData);
					j++;
				}
			}
			float freqHz = (max*200)/512.0f;
			sprintf(bufferData, "Frecuencia = %f Hz \n", freqHz);
			writeMsg(&USART1Comm, bufferData);
		}

	}else{
		writeMsg(&USART1Comm, "Comando Erroneo. Revisar el listado de comandos\n");
	}
}

void leerAcelerometro (void){
	if(flagReadData){
		ejeZ_Low = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ0);
		ejeZ_High = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ1);
		ejeZ = ejeZ_High << 8 | ejeZ_Low;
		flagReadData = 0;
		if(flagSaveData){
			dataAcelerometro[countAcelerometro] = ejeZ;
			countAcelerometro++;
			if(countAcelerometro > 513){
				flagSaveData = 0;
				countAcelerometro = 0;
				for (uint16_t i=1; i<513; i++){
					sprintf(bufferData, "Dato = %.2f \n", dataAcelerometro[i]*(9.8f/253.f));
					writeMsg(&USART1Comm, bufferData);
				}
				writeMsg(&USART1Comm, " Todos los datos han sido mostrados \n");
			}
		}
	}
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
}

void usart1Rx_CallBack (void){
	rxChar = getRxData();
}

void adcComplete_Callback(void){

	if(flagADC){
	if (channelADC==0){
		dataChannel1[countADC]=(float)getADC();
	}
	else{
		dataChannel2[countADC]=(float) getADC();
		countADC++;
	}

	channelADC++;
	if(countADC>= 257){
		countADC=0;
		flagADC=0;

		}
	if (channelADC>=2){
		channelADC=0;
	}
}
}

void BasicTimer5_Callback(void){
	flagReadData =1;
}

