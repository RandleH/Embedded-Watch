/**
  ******************************************************************************
  * @file    rh_light.c
  * @author  RandleH
  * @brief   This file contains led control source code.
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


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "rh_light.h"

#ifdef __cplusplus
extern "C"{
#endif

/* Private define ------------------------------------------------------------*/
// #define LED_R_PIN     GPIO_Pin_9
// #define LED_G_PIN     GPIO_Pin_10
#define LED_B_PIN     GPIO_PIN_13
#define GPIOx         GPIOC


/// Table
/// ====================================================== 
///  LED Pin IO           | STM32 Pin IO | Periphral
/// ----------------------+--------------+----------------
///  GREEN [GND=ON]       | PF9          | GPIO
/// ----------------------+--------------+----------------
///  RED   [GND=ON]       | PF10         | GPIO
/// ------------------------------------------------------


/** 
 * @brief  Peripherals Clock function
 * @param  (none)   
 * @retval (none)
*/
static inline void __configClock(void){
// #error "Please check this function and comment this line."	  
#if !defined (USE_HAL_DRIVER)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
#else
    __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
}

/** 
 * @brief  Config IO pin function
 * @param  (none)
 * @retval (none)
*/
static inline void __configGPIO(void){
#if defined (USE_HAL_DRIVER)    
    GPIO_InitTypeDef config = {
        .Pin   = LED_B_PIN,
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Pull  = GPIO_PULLUP,
        .Speed = GPIO_SPEED_LOW
    };
    HAL_GPIO_Init( GPIOx, &config );
#else
    GPIO_InitTypeDef config = {
        .GPIO_Pin = LED_G_PIN | LED_R_PIN,
        .GPIO_OType = GPIO_OType_PP,
        .GPIO_PuPd  = GPIO_PuPd_UP,
        .GPIO_Speed = GPIO_Speed_2MHz,
        .GPIO_Mode  = GPIO_Mode_OUT
    };
    GPIO_Init( GPIOx, &config);
    GPIO_ResetBits( GPIOx, LED_G_PIN | LED_R_PIN);
#endif    
    
}




/* Exported functions ------------------------------------------------------- */

/**
 * @brief   Initialize the screen
 * @param   (none)
 * @retval  (none)
*/
int rh_light__init(void){
    __configClock();
    __configGPIO();
    rh_light__switch( RH_LED_IDX__BLUE, false);
    return 0;
}

/**
 * @brief   Initialize the light io
 * @param   idx     Which led light?
 *          @arg    RH_LED_IDX__BLUE  : 2
 * @param   cmd     true=ON. false=OFF
 * @retval  (none)
*/
void rh_light__switch( int idx, bool cmd ){
#if defined (USE_HAL_DRIVER)    
    switch( idx ){
        case RH_LED_IDX__BLUE:
            HAL_GPIO_WritePin( GPIOx, LED_B_PIN, cmd==true?GPIO_PIN_RESET:GPIO_PIN_SET);
            break;
        default:
            break;
    }
#else
    switch( idx ){
        case RH_LED_IDX__RED:        
            cmd==true ? GPIO_ResetBits( GPIOx, LED_R_PIN):GPIO_SetBits( GPIOx, LED_R_PIN);
            break;
        case RH_LED_IDX__GREEN:
            cmd==true ? GPIO_ResetBits( GPIOx, LED_G_PIN):GPIO_SetBits( GPIOx, LED_G_PIN);
            break;
        default:
            break;    
    }
#endif
}

/**
 * @brief   Toggle a led io
 * @param   idx     Which led light?
 *          @arg    RH_LED_IDX__RED   : 0
 *          @arg    RH_LED_IDX__GREEN : 1
 * @retval  (none)
*/
void rh_light__toggle( int idx){
#if defined (USE_HAL_DRIVER) 
    switch( idx ){
        case RH_LED_IDX__BLUE:
            HAL_GPIO_TogglePin( GPIOx, LED_B_PIN);
            break;
        default:
            break;    
    }
#else    
    switch( idx ){
        case RH_LED_IDX__RED:
            GPIO_ToggleBits( GPIOx, LED_R_PIN);
            break;
        case RH_LED_IDX__GREEN:
            GPIO_ToggleBits( GPIOx, LED_G_PIN);
            break;
        default:
            break;    
    }
#endif
}


#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT RandleH *****END OF FILE****/


