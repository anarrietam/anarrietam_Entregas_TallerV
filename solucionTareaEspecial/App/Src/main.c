/**
 ******************************************************************************
 * @file           : main.c
 * @author         : anarrietam
 * @brief          : Solucion tarea "Especial"
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
#include "I2CDriver.h"
#include "PLLDriver.h"
#include "LCDDriver.h"


// Handler necesarios para el blinky
BasicTimer_Handler_t handlerTIM2 = {0};
GPIO_Handler_t handlerLedBlinky  = {0};

// Handler necesarios para el USART
USART_Handler_t USART1Comm       = {0};
GPIO_Handler_t handlerUSARTPINTX = {0};
GPIO_Handler_t handlerUSARTPINRX = {0};

// Handler para el desarrollo del I2C Acelerometro
I2C_Handler_t handlerAcelerometro = {0};
GPIO_Handler_t handlerI2C_SDA     = {0};
GPIO_Handler_t handlerI2C_SCL     = {0};

//Handler para el Timer encargado del muestreo
BasicTimer_Handler_t handlerTimerMuestreo = {0};

// Handlers para la solucion de los PWM
PWM_Handler_t handlerPWMEjeX         = {0};
PWM_Handler_t handlerPWMEjeY         = {0};
PWM_Handler_t handlerPWMEjeZ         = {0};
GPIO_Handler_t handlerPWMCH1         = {0};
GPIO_Handler_t handlerPWMCH2         = {0};
GPIO_Handler_t handlerPWMCH3         = {0};

// Handlers para el desarrollo del I2C LCD
BasicTimer_Handler_t handlerTimerUpdateLCD = {0};
GPIO_Handler_t handlerI2C_SDA_LCD          = {0};
GPIO_Handler_t handlerI2C_SCL_LCD          = {0};
I2C_Handler_t handlerLCD                   = {0};

// Variables necesarias para el corrrecto desarrollo de la tarea
char rxChar            = 0;                               // Variable para ingresar un char por consola
char bufferData[64]    = "Accel ADXL345 testing...\n";      //  Mensaje a imprimir cuando se inicie el codigo
uint8_t i2cBuffer      = {0};                             // Variable donde se guardan los datos del acelerometro
uint16_t countMuestreo = 0;                               // Contador que nos ayudara con el muestreo
uint8_t startTimer     = 0;
uint16_t duttyValueX   = 30000;                           // Valor del dutty cicle para el PWM relacionado con el eje X del acelerometro
uint16_t duttyValueY   = 30000;                           // Valor del dutty cicle para el PWM relacionado con el eje y del acelerometro
uint16_t duttyValueZ   = 30000;                           // Valor del dutty cicle para el PWM relacionado con el eje z del acelerometro
uint8_t countLastLine  = 0;                              // Contador que se encarga de "refrescar" la ultima linea de la LCD


// Se crean unas matrices y variables que ayudaran con el correcto funcionamiento del muestreo
float dataEjeX[2000] = {0};
float dataEjeY[2000] = {0};
float dataEjeZ[2000] = {0};
uint8_t AccelX_low   = 0;
uint8_t AccelX_high  = 0;
int16_t AccelX       = 0;
uint8_t AccelY_low   = 0;
uint8_t AccelY_high  = 0;
int16_t AccelY       = 0;
uint8_t AccelZ_low   = 0;
uint8_t AccelZ_high  = 0;
int16_t AccelZ       = 0;


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
void configAcelerometro(void);                                // Esta funcion se encarga de configurar el acelerometro, tomar los datos y mandar ciertos mensjaes por consola
void generacionPWM(void);                                     // Se encarga de cambiar los duttys cicles de 3 PWM dependiendo de los valores arrojados por el acelerometro
void encenderLCD(void);                                       // Son los mensajes que se pueden observar sobre la LCD que no se van a " refrescar" con los datos del acelerometro
void datosAcelerometroLCD(void);                              //  En esta funcion se programa la LCD para ir mostrando los datos tomados por el acelerometro


int main(void){

	// Se activa el PLL configurado en 80 MHz
	configPll();
	initialConfig();

	encenderLCD();

	// Se cambia la velocidad de muestreo del acelerometro
	i2c_writeSingleRegister(&handlerAcelerometro, BW_RATE , NewFRequency);

	// Se imprime el mensaje de inicio
	writeMsg(&USART1Comm, bufferData);

    while(1){
    	configAcelerometro();
    	generacionPWM();
    	datosAcelerometroLCD();
    	}
}

void initialConfig (void){

	// Activamos el FPU
	SCB->CPACR |= (0xF << 20);
	config_SysTick_ms(2);

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
	handlerTIM2.TIMx_Config.TIMx_speed				= BTIMER_SPEED_80MHz_100us;
	// Al trabjar con 100 us, para que el blinky sea de 250 ms, el periodo debe ser de 2500
	handlerTIM2.TIMx_Config.TIMx_period				= 2500;
	BasicTimer_Config(&handlerTIM2);

	// Configuracion del USART1
	USART1Comm.ptrUSARTx = USART1;
	USART1Comm.USART_Config.USART_baudrate     = USART_BAUDRATE_115200;
	USART1Comm.USART_Config.USART_datasize     = USART_DATASIZE_8BIT;
	USART1Comm.USART_Config.USART_parity       = USART_PARITY_NONE;
	USART1Comm.USART_Config.USART_mode         = USART_MODE_RXTX;
	USART1Comm.USART_Config.USART_stopbits     = USART_STOPBIT_1;
	USART1Comm.USART_Config.USART_enable_IntRx = USART_RX_INTERRUPT_ENABLE;
	USART1Comm.USART_Config.USART_enable_IntTx = USART_TX_INTERRUPT_ENABLE;
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
	handlerAcelerometro.mainClock         = MAIN_CLOCK_80_MHz_FOR_I2C;
	handlerAcelerometro.maxI2C_FM         = I2C_MAX_RISE_TIME_FM_80MHz;
	handlerAcelerometro.modeI2C_FM        = I2C_MODE_FM_SPEED_400KHz_80MHz;
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

	//Timer asociado al muestreo
	handlerTimerMuestreo.ptrTIMx                 = TIM4;
	handlerTimerMuestreo.TIMx_Config.TIMx_mode   = BTIMER_MODE_UP;
	handlerTimerMuestreo.TIMx_Config.TIMx_speed  = BTIMER_SPEED_80MHz_100us;
	handlerTimerMuestreo.TIMx_Config.TIMx_period = 10;
	BasicTimer_Config(&handlerTimerMuestreo);

	//Pines encargados de la salida de los PWM
	handlerPWMCH1.pGPIOx                             = GPIOC;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinNumber      = PIN_6;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerPWMCH1.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	GPIO_Config(&handlerPWMCH1);

	handlerPWMCH2.pGPIOx                             = GPIOC;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinNumber      = PIN_7;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerPWMCH2.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	GPIO_Config(&handlerPWMCH2);

	handlerPWMCH3.pGPIOx                             = GPIOC;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinNumber      = PIN_8;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerPWMCH3.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	GPIO_Config(&handlerPWMCH3);

	// Handlers PWM
	handlerPWMEjeX.ptrTIMx           = TIM3;
	handlerPWMEjeX.config.channel    = PWM_CHANNEL_1;
	handlerPWMEjeX.config.duttyCicle = duttyValueX;
	handlerPWMEjeX.config.periodo    = 60000;
	handlerPWMEjeX.config.prescaler  = 300;
	pwm_Config(&handlerPWMEjeX);
	enableOutput(&handlerPWMEjeX);
	startPwmSignal(&handlerPWMEjeX);


	handlerPWMEjeY.ptrTIMx           = TIM3;
	handlerPWMEjeY.config.channel    = PWM_CHANNEL_2;
	handlerPWMEjeY.config.duttyCicle = duttyValueY;
	handlerPWMEjeY.config.periodo    = 60000;
	handlerPWMEjeY.config.prescaler  = 300;
	pwm_Config(&handlerPWMEjeY);
	enableOutput(&handlerPWMEjeY);
	startPwmSignal(&handlerPWMEjeY);

	handlerPWMEjeZ.ptrTIMx           = TIM3;
	handlerPWMEjeZ.config.channel    = PWM_CHANNEL_3;
	handlerPWMEjeZ.config.duttyCicle = duttyValueZ;
	handlerPWMEjeZ.config.periodo    = 60000;
	handlerPWMEjeZ.config.prescaler  = 300;
	pwm_Config(&handlerPWMEjeZ);
	enableOutput(&handlerPWMEjeZ);
	startPwmSignal(&handlerPWMEjeZ);

	// COnfiguracion I2C LCD

	handlerI2C_SCL_LCD.pGPIOx        					  = GPIOB;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinNumber 	  = PIN_10;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinMode  	  = GPIO_MODE_ALTFN;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinOPType	  = GPIO_OTYPE_OPENDRAIN;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerI2C_SCL_LCD.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	GPIO_Config(&handlerI2C_SCL_LCD);

	handlerI2C_SDA_LCD.pGPIOx        					  = GPIOB;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinNumber 	  = PIN_3;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinMode  	  = GPIO_MODE_ALTFN;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinOPType	  = GPIO_OTYPE_OPENDRAIN;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEED_FAST;
	handlerI2C_SDA_LCD.GPIO_PinConfig.GPIO_PinAltFunMode  = AF9;
	GPIO_Config(&handlerI2C_SDA_LCD);

	handlerTimerUpdateLCD.ptrTIMx                          = TIM5;
	handlerTimerUpdateLCD.TIMx_Config.TIMx_mode            = BTIMER_MODE_UP;
	handlerTimerUpdateLCD.TIMx_Config.TIMx_speed           = BTIMER_SPEED_80MHz_100us;
	handlerTimerUpdateLCD.TIMx_Config.TIMx_period          = 10000;
	handlerTimerUpdateLCD.TIMx_Config.TIMx_interruptEnable = BTIMER_INTERRUPT_ENABLE;
	BasicTimer_Config(&handlerTimerUpdateLCD);

	handlerLCD.ptrI2Cx      = I2C2;
	handlerLCD.modeI2C      = I2C_MODE_FM;
	handlerLCD.slaveAddress = 0x24;                              // Direccion de la LCD, para comunicarse por I2C.
	handlerLCD.mainClock    = MAIN_CLOCK_80_MHz_FOR_I2C;
	handlerLCD.maxI2C_FM    = I2C_MAX_RISE_TIME_FM_80MHz;
	handlerLCD.modeI2C_FM   = I2C_MODE_FM_SPEED_400KHz_80MHz;
	i2c_config(&handlerLCD);
	LCD_Init(&handlerLCD);

}

void configAcelerometro(void){

	//se inicia el acelerometro para la toma de datos
	i2c_writeSingleRegister(&handlerAcelerometro, POWER_CTL , 0x2D);

	// Se tomara 2000 datos para cada eje con ayuda un cintador manjeado con un timer ( TIM4)
	if (countMuestreo <= 2001){
		AccelX_low =  i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX0);
		AccelX_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX1);
		AccelX = AccelX_high << 8 | AccelX_low;
		AccelY_low = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAY0);
		AccelY_high = i2c_readSingleRegister(&handlerAcelerometro,ACCEL_DATAY1);
		AccelZ = AccelY_high << 8 | AccelY_low;
		AccelZ_low = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ0);
		AccelZ_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ1);
		AccelZ = AccelZ_high << 8 | AccelZ_low;

		// Lee eje por eje, y se va guardando en una matriz para cada eje, además se hace una conversion para que quede en m/s2
		dataEjeX[countMuestreo] = (AccelX/256.f)*9.78;
		dataEjeY[countMuestreo] = (AccelY/256.f)*9.78;
		dataEjeZ[countMuestreo] = (AccelZ/232.f)*9.78;

	}

	// Se crean una serie de acciones ordenadas con ayuda de los condicionales,
	//para asì poder tener varios mensajes que se imprimen por consola
	if(rxChar != '\0'){

		if(rxChar == 'w'){

			i2cBuffer = i2c_readSingleRegister(&handlerAcelerometro, DEVID);
			sprintf(bufferData, "Direccion Acelerometro  = 0x%x \n", ACCEL_ADDRESS);
			writeMsg(&USART1Comm, bufferData);
			rxChar = '\0';
		}
		else if (rxChar == 'f'){

			sprintf(bufferData, "Frecuencia de muestreo = 1600 KHz \n");
			writeMsg(&USART1Comm, bufferData);
			rxChar = '\0';
		}
		else if (rxChar == 'r'){
			sprintf(bufferData, "POWER_CTL reset \n");
			writeMsg(&USART1Comm, bufferData);

			i2c_writeSingleRegister(&handlerAcelerometro, POWER_CTL , 0x2D);
			rxChar = '\0';
		}
		else if (rxChar == 'x'){
			sprintf(bufferData, "Axis X data (r) \n");
			writeMsg(&USART1Comm, bufferData);

			AccelX_low =  i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX0);
			AccelX_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX1);
			AccelX = AccelX_high << 8 | AccelX_low;
			sprintf(bufferData, "AccelX = %.2f \n", (AccelX/256.f)*9.78);
			writeMsg(&USART1Comm, bufferData);
			rxChar = '\0';
		}
		else if(rxChar == 'y'){
			sprintf(bufferData, "Axis Y data (r)\n");
			writeMsg(&USART1Comm, bufferData);
			AccelY_low = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAY0);
			AccelY_high = i2c_readSingleRegister(&handlerAcelerometro,ACCEL_DATAY1);
			AccelY = AccelY_high << 8 | AccelY_low;
			sprintf(bufferData, "AccelY = %.2f \n", (AccelY/256.f)*9.78);
			writeMsg(&USART1Comm, bufferData);
			rxChar = '\0';
		}
		else if(rxChar == 'z'){
			sprintf(bufferData, "Axis Z data (r)\n");
			writeMsg(&USART1Comm, bufferData);

			AccelZ_low = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ0);
			AccelZ_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ1);
			AccelZ = AccelZ_high << 8 | AccelZ_low;
			sprintf(bufferData, "AccelZ = %.2f \n", (AccelZ/215.f)*9.78);
			writeMsg(&USART1Comm, bufferData);
			rxChar = '\0';
		}
		// Este es el caso donde se imprimen 6000 datos ( 2000 datos por eje )
		else if (rxChar == 'm'){
			writeMsg(&USART1Comm, "Muestreo del acelerometro en todos los ejes \n" );
			// para esto con ayuda de un for recorremos el arreglo para cada uno de los ejes, y con ayuda del sprintf, le damos el
			//formato deseado antes de imprimir
			for (int i=0;i<2000;i++){
				sprintf(bufferData, " x = %.2f m/s²; y = %.2f m/s²; z = %.2f m/s²; # %d \n",  dataEjeX[i],dataEjeY[i],dataEjeZ[i],i);
				writeMsg(&USART1Comm, bufferData);
			}
		} else{
			rxChar = '\0';
		}
	}
}

void generacionPWM(void){

	// Se utiliza un for para recorrer los arreglos con las medidas por eje dadas por el acelerometro
	for ( uint16_t x; x < 2000; x++){

		// Estas medidas se dividen en 5 rangos, los cuales tienen un valor correspondiente del duttycicle
		if (dataEjeX[x] <= -7.5){
			updateDuttyCycle(&handlerPWMEjeX, 3000);
		}else if (dataEjeX[x] > -7.5 && dataEjeX[x] <=  -2.5){
			updateDuttyCycle(&handlerPWMEjeX, 15000);
		}else if (dataEjeX[x] > -2.5 && dataEjeX[x] <= 2.5){
			updateDuttyCycle(&handlerPWMEjeX, 30000);
		}else if (dataEjeX[x] > 2.5 && dataEjeX[x] <= 7.5){
			updateDuttyCycle(&handlerPWMEjeX, 45000);
		}else{
			updateDuttyCycle(&handlerPWMEjeX, 56000);
		}
		if (dataEjeY[x] <= -7.5){
			updateDuttyCycle(&handlerPWMEjeY, 3000);
		}else if (dataEjeY[x] > -7.5 && dataEjeY[x] <=  -2.5){
			updateDuttyCycle(&handlerPWMEjeY, 15000);
		}else if (dataEjeY[x] > -2.5 && dataEjeY[x] <= 2.5){
			updateDuttyCycle(&handlerPWMEjeY, 30000);
		}else if (dataEjeY[x] > 2.5 && dataEjeY[x] <= 7.5){
			updateDuttyCycle(&handlerPWMEjeY, 45000);
		}else{
			updateDuttyCycle(&handlerPWMEjeY, 56000);
		}
		if (dataEjeZ[x] <= -7.5){
			updateDuttyCycle(&handlerPWMEjeZ, 3000);
		}else if (dataEjeZ[x] > -7.5 && dataEjeZ[x] <=  -2.5){
			updateDuttyCycle(&handlerPWMEjeZ, 15000);
		}else if (dataEjeZ[x] > -2.5 && dataEjeZ[x] <= 2.5){
			updateDuttyCycle(&handlerPWMEjeZ, 30000);
		}else if (dataEjeZ[x] > 2.5 && dataEjeZ[x] <= 7.5){
			updateDuttyCycle(&handlerPWMEjeZ, 45000);
		}else{
			updateDuttyCycle(&handlerPWMEjeZ, 56000);
		}

	}

}

void encenderLCD(void){

	// Despues de prender la LCD en el initialConfig, mandamos a mostrar los datos que no se van a refrescar cuando
	// vayan variando las medidas del acelerometro

	LCD_setCursor(&handlerLCD, 0, 0);
	LCD_sendSTR(&handlerLCD, "Eje X = ");
	LCD_setCursor(&handlerLCD, 0, 15);
	LCD_sendSTR(&handlerLCD, "m/s2");
	LCD_setCursor(&handlerLCD, 1, 0);
	LCD_sendSTR(&handlerLCD, "Eje Y = ");
	LCD_setCursor(&handlerLCD, 1, 15);
	LCD_sendSTR(&handlerLCD, "m/s2");
	LCD_setCursor(&handlerLCD, 2, 0);
	LCD_sendSTR(&handlerLCD, "Eje Z = ");
	LCD_setCursor(&handlerLCD, 2, 15);
	LCD_sendSTR(&handlerLCD, "m/s2");
	// Este mensaje va a iniciar la ultima linea pero se va a "referscar" con ayuda de un timer cada 10s
	LCD_setCursor(&handlerLCD, 3, 0);
	LCD_sendSTR(&handlerLCD, "Sens(Axis)= 0.04m/s2"); // Dato sacado del datasheet

}

void datosAcelerometroLCD(void){

	// Los datos en la LCD se actualizaran cada que la variable startTimer sea igual 1,
	// el timer que cambia esta variable lo hace cada 1s, ademàs de esto al final de condicional para
	//la muestra de los datos se tiene una negacion de la variable, es decir de hace 0 al actuzalizar los datos,
	// luego al pasar el tiempo del timer este se vuelve a hacer 1, haciendo que se vuelvan a actualizar los datos

	if(startTimer == 1){
		AccelX_low =  i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX0);
		AccelX_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAX1);
		AccelX = (AccelX_high << 8 | AccelX_low);
		float AcelereacionX= (AccelX/ 256.f) * 9.78;
		sprintf(bufferData, "%.2f ", AcelereacionX);
		LCD_setCursor(&handlerLCD, 0, 9);
		LCD_sendSTR(&handlerLCD, bufferData);

		AccelY_low =  i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAY0);
		AccelY_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAY1);
		AccelY = (AccelY_high << 8 | AccelY_low);
		float AceleracionY= (AccelY/ 256.f) * 9.78;
		sprintf(bufferData, "%.2f ", AceleracionY);
		LCD_setCursor(&handlerLCD, 1, 9);
		LCD_sendSTR(&handlerLCD, bufferData);

		AccelZ_low =  i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ0);
		AccelZ_high = i2c_readSingleRegister(&handlerAcelerometro, ACCEL_DATAZ1);
		AccelZ = (AccelZ_high << 8 | AccelZ_low);
		float AceleracionZ = (AccelZ/ 256.f) * 9.78;
		sprintf(bufferData, "%.2f ", AceleracionZ);
		LCD_setCursor(&handlerLCD, 2, 9);
		LCD_sendSTR(&handlerLCD, bufferData);
		LCD_setCursor(&handlerLCD, 3, 0);

		startTimer =! startTimer;
	}

	// se va a mostrar en la ultima linea un mensaje que dice la sensibilidad del acelerometro y la frecuencia de muestreo,
	// el cual se va a "refrescar " cada 10s
	if (countLastLine == 10){
		LCD_setCursor(&handlerLCD, 3, 0);
		LCD_sendSTR(&handlerLCD, "Sens(Axis)= 0.04m/s2"); // Dato sacado del datasheet
	} else if (countLastLine == 20){
		LCD_setCursor(&handlerLCD, 3, 0);
		LCD_sendSTR(&handlerLCD, "Frecuencia = 1600mKHz");
		countLastLine = 0;
	}

}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLedBlinky);
}

void BasicTimer4_Callback(void){

	// Aumentamos el contador para el muestreo del acelerometro con este timer,
	// además se coloca un condicional para que no sobrepase el valor de 2000
	countMuestreo ++;
	if(countMuestreo == 2000){
		rxChar='\0';
		countMuestreo = 0;

	}
}

void BasicTimer5_Callback(void){

	// Este timer al tener un periodo de 10000 a 100us, se demora 1s en completar esta accion, lo cual cambiaria
	// la variable startTimer, cada que este tiempo se cumpla, y aumenta de a uno la variable counLastLine
	startTimer =! startTimer;
	countLastLine ++;
}

void usart1Rx_CallBack (void){
	rxChar = getRxData();
}

