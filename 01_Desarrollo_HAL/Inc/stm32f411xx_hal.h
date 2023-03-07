/**********************************
 * stm32f411xx_hal.h
 *
 *  Created on: Mar 2, 2023
 *      Author: anarrietam
 *
 *  Este archivo contiene la informacion mas basicas del micro:
 *  	- Valores del reloj principal
 *  	-Distribucion basica de la memoria
 *  	-Posiciones de memoria de los perifericos disponibles
 *  	-Incluye los demas registros de los perifericos
 *  	-Definiciones de las constantes mas basicas
 *
 ***********************************/

#ifndef STM32F411XX_HAL_H_
#define STM32F411XX_HAL_H_

#include <stdint.h>
#include <stddef.h>

#define HSI_CLOCK_SPEED          16000000        //Valor para la señal de reloj principal (HSI --> High Speed Internal)
#define HSE_CLOCK_SPEED          4000000         //Valor para la señal de reloj principal (HSE --> HIgh Speed EXternal)

#define NOP()                    asm("NOP")
#define __weak                   __attribute__((weak))

/*
 * Direcciones base de las memorias FLASH y SRAM
 */

#define FLASH_BASE_ADOR          0x08000000U     //Memoria del programa (512KB)
#define SRAM_BASE_ADOR           0x20000000U     //Memoria RAM (128KB)

/*
 * Se agregan la direccion base para cada uno de los perifericos que posee el micro.
 * Teniendo los siguientes buses:
 * 	- APB1
 * 	- APB2
 * 	- AHB1
 * 	- AHB2
 */

#define APB1_BASE_ADOR           0x40000000U
#define APB2_BASE_ADOR           0x40010000U
#define AHB1_BASE_ADOR           0x40020000U
#define AHB2_BASE_ADOR           0x50000000U

/*
 * Se agregan las posiciones de memoria para cada uno de los perifericos de los buses.
 * Solo se hace con los elementos RCC y GPIOx
 */

/*  Posicion de memoria para los periferciso de AHB2*/

#define USB_OTG_FS_BASE_ADOR     (AHB2_BASE_ADOR + 0x0000U)

/*  Posicion de memoria para los periferciso de AHB1*/

#define RCC_BASE_ADOR            (AHB1_BASE_ADOR + 0x3800U)
#define GPIOH_BASE_ADOR          (AHB1_BASE_ADOR + 0x1C00U)
#define GPIOE_BASE_ADOR          (AHB1_BASE_ADOR + 0x1000U)
#define GPIOD_BASE_ADOR          (AHB1_BASE_ADOR + 0x0C00U)
#define GPIOC_BASE_ADOR          (AHB1_BASE_ADOR + 0x0800U)
#define GPIOB_BASE_ADOR          (AHB1_BASE_ADOR + 0x0400U)
#define GPIOA_BASE_ADOR          (AHB1_BASE_ADOR + 0x0000U)

/**
 * Macros Genericos
 */

#define ENABLE                    1
#define DISABLE                   0
#define SET                       ENABLE
#define CLEAR                     DISABLE
#define RESET                     DISABLE
#define GPIO_PIN_SET              SET
#define GPIO_PIN_RESET            RESET
#define FLAG_SET                  SET
#define FLAG_RESET                RESET
#define I2C_WRITE                 0
#define I2C_READ                  1

/*  +++++++++++++++ INICIO DE LA DESCRIPCION DE LOS ELEMENTOS QUE COMPONEN EL PERFIERCO++++++++*/
/*
 * Definicion de la estrucutura de datos que representa cada uno de los registros que compone
 * el perifercio RCC.
 *
 * +NOTA: La posicion de memoria (Offset) debe encajar perfectamente con la posicion de memoria
 * indicada en el datasheet del equipo.
 */

typedef struct
{
		volatile uint32_t CR;                  //Clock control register                    ADDR_OFFSET: 0x00
		volatile uint32_t PLLCFGR;             //PLL Configuration Register                ADDR_OFFSET: 0x04
		volatile uint32_t CFGR;                //Clock Configuration Register              ADDR_OFFSET: 0x08
		volatile uint32_t CIR;                 //Clock Interrupt Register                  ADDR_OFFSET: 0x0C
		volatile uint32_t AHB1RSTR;            //AHB1 Peripheral reset register            ADDR_OFFSET: 0x10
		volatile uint32_t AHB2RSTR;            //AHB2 Peripheral reset register            ADDR_OFFSET: 0x14
		volatile uint32_t reserved0;           //reserved                                  ADDR_OFFSET: 0x18
		volatile uint32_t reserved1;           //reserved                                  ADDR_OFFSET: 0x1C
		volatile uint32_t APB1RSTR;            //APB1 Peripheral reset register            ADDR_OFFSET: 0x20
		volatile uint32_t APB2RSTR;            //APB2 Peripheral reset register            ADDR_OFFSET: 0x24
		volatile uint32_t reserved2;           //reserved                                  ADDR_OFFSET: 0x28
		volatile uint32_t reserved3;           //reserved                                  ADDR_OFFSET: 0X2C
		volatile uint32_t AHB1ENR;             //AHB1 Peripheral Clock Enable Register     ADDR_OFFSET: 0x30
		volatile uint32_t AHB2ENR;             //AHB2 Peripheral Clock Enable Register     ADDR_OFFSET: 0x34
		volatile uint32_t reserved4;           //reserved                                  ADDR_OFFSET: 0x38
		volatile uint32_t reserved5;           //reserved                                  ADDR_OFFSET: 0x3C
		volatile uint32_t APB1ENR;             //APB1 Peripheral Clock Enable Register     ADDR_OFFSET: 0x40
		volatile uint32_t APB2ENR;             //APB2 Peripheral Clock Enable Register     ADDR_OFFSET: 0x44
		volatile uint32_t reserved6;           //reserved                                  ADDR_OFFSET: 0x48
		volatile uint32_t reserved7;           //reserved                                  ADDR_OFFSET: 0x4C
		volatile uint32_t AHB1LPENR;           //AHB1 Clock Enable Low Power Register      ADDR_OFFSET: 0x50
		volatile uint32_t AHB2LPENR;           //AHB2 Clock Enable Low Power Register      ADDR_OFFSET: 0x54
		volatile uint32_t reserved8;           //reserved                                  ADDR_OFFSET: 0x58
		volatile uint32_t reserved9;           //reserved                                  ADDR_OFFSET: 0x5C
		volatile uint32_t APB1LPENR;           //APB1 Clock Enable Low Power Register      ADDR_OFFSET: 0x60
		volatile uint32_t APB2LPENR;           //APB2 Clock Enable Low Power Register      ADDR_OFFSET: 0x64
		volatile uint32_t reserved10;          //reserved                                  ADDR_OFFSET: 0x68
		volatile uint32_t reserved11;          //reserved                                  ADDR_OFFSET: 0x6C
		volatile uint32_t BDCR;                //Backup Domain Control Register            ADDR_OFFSET: 0x70
		volatile uint32_t CSR;                 //Clock Control & Status Register           ADDR_OFFSET: 0x74
		volatile uint32_t reserved12;          //reserved                                  ADDR_OFFSET: 0x78
		volatile uint32_t reserved13;          //reserved                                  ADDR_OFFSET: 0x7C
		volatile uint32_t SSCGR;               //Spread Spectrum CLock Generation Register ADDR_OFFSET: 0x80
		volatile uint32_t PLLI2SCFGR;          //PLLI2S Configuration Register             ADDR_OFFSET: 0x84
		volatile uint32_t reserved14;          //reserved                                  ADDR_OFFSET: 0x88
		volatile uint32_t DCKCFGR;             //Dedicated Clocks Configuration Register   ADDR_OFFSET: 0x8C
}RCC_RegDef_t;

/* Hacemos un puntero RCC_RegDef_t que apunta a la posicion exacta
 * del perifericos RCC, de modo que cada miembro de la estructura coincide
 * con cada uno de los SFR de la memoria del MCU
 */

#define RCC     ((RCC_RegDef_t *)RCC_BASE_ADOR)

/*
 * Descripcion bit a bit de cada uno de los registros que componen al periferico RCC
 */

/* 6.3.9 RCC_AHB1ENR*/

#define RCC_AHB1ENR_GPIOA_EN      0
#define RCC_AHB1ENR_GPIOB_EN      1
#define RCC_AHB1ENR_GPIOC_EN      2
#define RCC_AHB1ENR_GPIOD_EN      3
#define RCC_AHB1ENR_GPIOE_EN      4
#define RCC_AHB1ENR_GPIOH_EN      7
#define RCC_AHB1ENR_CRCEN         12
#define RCC_AHB1ENR_DMA1_EN       21
#define RCC_AHB1ENR_DMA2_EN       22

/*
 * Fin de la descripcion del periferico RCC
 */

/* Inicio de la descripcion de los elementos que componen el Periferico GPIOx */

typedef struct
{
	volatile uint32_t MODER;                //Port Mode Register                ADDR_OFFSET: 0x00
	volatile uint32_t OTYPER;               //Port Output Type Register         ADDR_OFFSET: 0x04
	volatile uint32_t OSPEEDR;              //Port Output Speed Register        ADDR_OFFSET: 0x08
	volatile uint32_t PUPDR;                //Port PullUp/PullDown Register     ADDR_OFFSET: 0x0C
	volatile uint32_t IDR;                  //Port Input Data Register          ADDR_OFFSET: 0x10
	volatile uint32_t ODR;                  //Port Output Data Register         ADDR_OFFSET: 0x14
	volatile uint32_t BSRR;                 //Port Bit Set/Reset Register       ADDR_OFFSET: 0x18
	volatile uint32_t LCKR;                 //Port Configuration Lock Register  ADDR_OFFSET: 0x1C
	volatile uint32_t AFRL;                 //Alternate Function Low Register   ADDR_OFFSET: 0x20
	volatile uint32_t AFRH;                 //Alternate Function High Register  ADDR_OFFSET: 0x24
} GPIOx_RegDef_t;

/* debemos hacer que cada GPIOx quede correctamente posicionado */

#define GPIOA    ((GPIOx_RegDef_t *) GPIOA_BASE_ADOR)
#define GPIOB    ((GPIOx_RegDef_t *) GPIOB_BASE_ADOR)
#define GPIOC    ((GPIOx_RegDef_t *) GPIOC_BASE_ADOR)
#define GPIOD    ((GPIOx_RegDef_t *) GPIOD_BASE_ADOR)
#define GPIOE    ((GPIOx_RegDef_t *) GPIOE_BASE_ADOR)
#define GPIOH    ((GPIOx_RegDef_t *) GPIOH_BASE_ADOR)

/* Valores estandares para las configuraciones */
/* 8.4.1 GPIOx_MODER (Dos bit por cada PIN)*/
#define GPIO_MODE_IN              0
#define GPIO_MODE_OUT             1
#define GPIO_MODE_ALTFN           2
#define GPIO_MODE_ANALOG          3

/* 8.4.2 GPIOx_OTYPER (Un bit por cada PIN)*/
#define GPIO_OTYPE_PUSHPULL       0
#define GPIO_OTYPE_OPENDRAIN      1

/* 8.4.3 GPIOx_OSPEEDR (Dos bit por cada PIN)*/
#define GPIO_OSPEED_LOW           0
#define GPIO_OSPEED_MEDIUM        1
#define GPIO_OSPEED_FAST          2
#define GPIO_OSPEED_HIGH          3

/* 8.4.4 GPIOx_PUPDR (Dos bit por cada PIN)*/
#define GPIO_PUPDR_NOTHING        0
#define GPIO_PUPDR_PULLUP         1
#define GPIO_PUPDR_PULLDOWN       2
#define GPIO_PUPDR_RESERVED       3

/* 8.4.5 GPIOx_IDR (Un bit por cada PIN) - Lee el estado de un pin*/
/* 8.4.6 GPIOx_oDR (Un bit por cada PIN) - Escribe el estado de un pin*/

/* Definicion de los nombres de los pines */
#define PIN_0                     0
#define PIN_1                     1
#define PIN_2                     2
#define PIN_3                     3
#define PIN_4                     4
#define PIN_5                     5
#define PIN_6                     6
#define PIN_7                     7
#define PIN_8                     8
#define PIN_9                     9
#define PIN_10                    10
#define PIN_11                    11
#define PIN_12                    12
#define PIN_13                    13
#define PIN_14                    14
#define PIN_15                    15

/* Definicion de las funciones alternativas */
#define AF0                       0b0000
#define AF1                       0b0001
#define AF2                       0b0010
#define AF3                       0b0011
#define AF4                       0b0100
#define AF5                       0b0101
#define AF6                       0b0110
#define AF7                       0b0111
#define AF8                       0b1000
#define AF9                       0b1001
#define AF10                      0b1010
#define AF11                      0b1011
#define AF12                      0b1100
#define AF13                      0b1101
#define AF14                      0b1110
#define AF15                      0b1111


#endif /* STM32F411XX_HAL_H_ */
