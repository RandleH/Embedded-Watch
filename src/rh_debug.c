/**
  ******************************************************************************
  * @file    rh_debug.c
  * @author  RandleH
  * @brief   This file contains usart driver source code and debug utility.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 RandleH.
  * All rights reserved.
  *
  * This software component is licensed by RandleH under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  * 
  ******************************************************************************
  */


/// ====================================================== 
/// Device: CN480661 FT232RL
/// ====================================================== 
///  UART Pin IO          | STM32 Pin IO | Periphral
/// ----------------------+--------------+----------------
///  TX                   | PA2          | UART2
/// ----------------------+--------------+----------------
///  RX                   | PA3          | UART2
/// ------------------------------------------------------


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio.h"

#include "rh_common.h"
#include "rh_debug.h"
#include "rh_timer.h"

#ifdef __cplusplus
extern "C"{
#endif



/* Private define ------------------------------------------------------------*/
#define UARTx               UART2

#define PIN_TX              GPIO_PIN_2   // Port A
#define PIN_RX              GPIO_PIN_3   // Port A

#define GPIOx_TX            GPIOA
#define GPIOx_RX            GPIOA



static UART_HandleTypeDef huart = {0};



/* Private functions ---------------------------------------------------------*/
/**
 * @brief   Assertion function
 * @param   expr  Expected expression
 * @retval  It is recommanded to run abort() in this funtion.
*/
static inline void __assert( bool expr){
    if( !expr){
        while(1);
    }
}



/**
 * @brief   Config clock bus for peripheral modules
 * @param   (none)
 * @retval  (none)
*/
static inline void __configClock( void ){
// #error "Please check this function and comment this line."	
#if !defined (USE_HAL_DRIVER) 
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_USART2 |
                            RCC_AHB1Periph_GPIOA  , ENABLE);                            
#else
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
#endif

}

/**
 * @brief   Config alternative function IO pin
 * @param   (none)
 * @retval  (none)
*/
static inline void __configGPIO( void ){
// #error "Please check this function and comment this line."	
#if !defined (USE_HAL_DRIVER) 
    
#else
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    GPIO_InitStruct.Pin   = PIN_TX;
	HAL_GPIO_Init( GPIOx_TX, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = PIN_RX;
    HAL_GPIO_Init( GPIOx_RX, &GPIO_InitStruct);

#endif

}

static inline void __sendBytes( const char *buf, size_t len){
    HAL_UART_Transmit(&huart, (u8*)buf, len, 1000);
}


/**
 * @brief   Config usart peripheral
 * @param   (none)
 * @retval  (none)
*/
static inline void __configUSART(void){
    huart.Instance        = USART2;
    huart.Init.BaudRate   = RH_CFG_DEBUG_BAUDRATE;				   
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits   = UART_STOPBITS_1;
    huart.Init.Parity     = UART_PARITY_NONE;
    huart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    huart.Init.Mode       = UART_MODE_TX;
    __assert( HAL_OK==HAL_UART_Init(&huart));
}

/**
 * @brief   Initialize the usart module
 * @param   (none)
 * @retval  If success, return 0.
*/
int rh_debug__init  ( void){
    __configClock();
    __configGPIO();
    __configUSART();

    return 0;
}


/**
 * @brief   Debug printf function. This function will do nothing if `CFG_DEBUG__ENABLE` is set 0.
 * @param   fmt  Formatted string with va_list
 * @retval  If success, return 0.
 *          Return 1 if buffer overflow.
 *          Return 2 if communication time out.
*/
int rh_debug__printf( const char * fmt, ...){
#if CFG_DEBUG__ENABLE     
    static char tmp[256] = {0};

    va_list ap;
    va_start( ap, fmt);
    int len = vsnprintf( tmp, 256, fmt, ap);
    va_end(ap);

    if( len==-1 )
        return 1;

    __sendBytes( tmp, len);
#endif
    return 0;
}






#ifdef __cplusplus
}
#endif


/************************ (C) COPYRIGHT RandleH *****END OF FILE****/