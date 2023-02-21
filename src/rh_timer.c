/**
  ******************************************************************************
  * @file    rh_timer.c
  * @author  RandleH
  * @brief   This file contains the source code of delay function on STM32F103C8T6.
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
#include "rh_common.h"
#include "rh_timer.h"

#ifdef __cplusplus
extern "C"{
#endif


/* Private define ------------------------------------------------------------*/

#define CLK_MHz        (RH_CFG_TIMER_BUS_FREQ_MHz)
#define CLK_KHz        (CLK_MHz*1000U) 
#define CLK_Hz         (CLK_MHz*1000000U)

#define TIMx_DELAY         RH_CFG_TIMER_DELAY
#define TIMx_DELAY_RCCEN   RH_CFG_TIMER_DELAY_CLKCMD

#define TIMx_COUNT         RH_CFG_TIMER_COUNT
#define TIMx_COUNT_RCCEN   RH_CFG_TIMER_COUNT_CLKCMD







/* Private function prototypes -----------------------------------------------*/
static inline void __delay( u32 psc, u32 arr, u32 ckd );
static inline void __configClock(void);



/* Private functions ---------------------------------------------------------*/
/**
 * @brief Internal timer configuration function
 * @param psc Prescaler value
 * @param arr Auto-reload value
 * @param ckd Clock division
 * @file  "rh_timer.c"
*/
static inline void __delay( u32 psc, u32 arr, u32 ckd ){
    TIMx_DELAY->ARR = arr;
    TIMx_DELAY->PSC = psc;
    switch(ckd){
        case 0:  // No DIV
            CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_0); 
            CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_1);break;
        case 2:  // DIV 2 
            SET_BIT  ( TIMx_DELAY->CR1, TIM_CR1_CKD_0);
            CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_1);break;
        case 4:  // DIV 4
            CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_0);
            SET_BIT  ( TIMx_DELAY->CR1, TIM_CR1_CKD_1);break;
        default: // Wrong configuration
            break;
    }
    
}


/**
 * @brief Internal timer clock enable function
 * @param (none)
 * @file  "rh_timer.c"
*/
static inline void __configClock(void){
// #error "Check the following clock setting and then comment this line."
    SET_BIT( RCC->APB1ENR, RH_CFG_TIMER_DELAY_CLKCMD );
    SET_BIT( RCC->APB1ENR, RH_CFG_TIMER_COUNT_CLKCMD );
}





/* Public functions ----------------------------------------------------------*/
/**
 * @brief   Timer initialization. 
 * @param   (none)
 * @retval  
 * @file  "rh_timer.c"
*/
int rh_timer__init( void){

  __configClock();

  /**
   * @details Initialize the timer for delay function
  */
  {
    TIMx_DELAY->ARR = 9999U;
    TIMx_DELAY->PSC = CLK_MHz*100;

    SET_BIT( TIMx_DELAY->EGR, TIM_EGR_UG );   // Generate an update
    
    SET_BIT( TIMx_DELAY->CR1, TIM_CR1_DIR);   // Down Counter
    SET_BIT( TIMx_DELAY->CR1, TIM_CR1_URS);   // Update only when overflow
    SET_BIT( TIMx_DELAY->CR1, TIM_CR1_OPM);   // One-pulse mode
    SET_BIT( TIMx_DELAY->CR1, TIM_CR1_ARPE);  // Auto-reload preload
    
    CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_0); // No clock division
    CLEAR_BIT( TIMx_DELAY->CR1, TIM_CR1_CKD_1);

    TIMx_DELAY->SR = 0x00000000;
    SET_BIT( TIMx_DELAY->DIER, TIM_DIER_UIE);
    
    SET_BIT( TIMx_DELAY->CR1, TIM_CR1_CEN);  // Enable timer

    while( false==READ_BIT( TIMx_DELAY->SR, TIM_SR_UIF )); // Wait until an update was generated
    
    TIMx_DELAY->SR = ~(TIM_SR_UIF);
  }
  
  
  /**
   * @details Initialize the timer for count measurement.
  */
  {
    TIMx_COUNT->ARR = 0xffffffff;
    TIMx_COUNT->PSC = CLK_MHz*100-1;
    
    SET_BIT  ( TIMx_COUNT->EGR, TIM_EGR_UG );    // Update
    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_DIR);    // Up Counter
    SET_BIT  ( TIMx_COUNT->CR1, TIM_CR1_URS);    // Update only when overflow
    SET_BIT  ( TIMx_COUNT->CR1, TIM_CR1_ARPE);   // Auto-reload preload
    SET_BIT  ( TIMx_COUNT->CR1, TIM_CR1_OPM);    // One-pulse mode
    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_CKD_0);  // Clock divided by 1
    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_CKD_1);

    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_CMS_0);  // Edge-aligned mode
    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_CMS_1);

    TIMx_COUNT->SR = 0x00000000;
    SET_BIT  ( TIMx_COUNT->DIER, TIM_DIER_UIE);
  }


  return 0;
}



/**
 * @brief Block delay function using TIMx_DELAY module in milliseconds
 * @param ms milliseconds
 * @file  "rh_timer.c"
*/
int rh_timer__delayMS( unsigned long ms){
///=========================================================================
///               (Prescaler+1)*(AutoreloadValue+1)*CLK_Division*1000
///  Tout(ms) = --------------------------------------------------------
///                                  CLK_Frequency
///=========================================================================
    u32 psc = CLK_MHz-1, arr = ms*1000-1, ckd = 0;
    if( ms==0 ){
        return 0;
    }else if ( ms<65    ){
        psc = CLK_MHz-1; arr = ms*1000-1; ckd = 0;
    }else if ( ms<655   ){
        psc = CLK_MHz*10-1; arr = ms*100-1; ckd = 0;
    }else if ( ms<6553  ){
        psc = CLK_MHz*100-1; arr = ms*10-1; ckd = 0;
    }else if ( ms<=13107 ){
        psc = CLK_MHz*100-1; arr = ms*5-1;  ckd = 2;
    }else{
        rh_timer__delayMS(13107);
        return rh_timer__delayMS(ms-13107);
    }
    __delay( psc, arr, ckd);

  SET_BIT( TIMx_DELAY->EGR, TIM_EGR_UG);
  SET_BIT( TIMx_DELAY->CR1, TIM_CR1_CEN);

  while( 0==READ_BIT( TIMx_DELAY->SR, TIM_SR_UIF) );

  TIMx_DELAY->SR = ~(TIM_SR_UIF);
  
  return 0;
}

/**
 * @brief Block delay function using TIMx_DELAY module in microseconds
 * @param us microseconds
 * @file  "rh_timer.c"
*/
int rh_timer__delayUS( unsigned long us){
    if( us==0 ) return 0;
    while( us>=65536 ){
        rh_timer__delayMS(60);
        us -= 60000;
    }
    u32 psc = CLK_MHz-1, arr = us-1, ckd = 0;
    __delay( psc, arr, ckd);
    return 0;
}

/**
 * @brief  Set a start point. This function must be called before get the value of `rh_timer__get()`
 * @param  (none)
 * @retval If success, return 0.
*/
int rh_timer__set( void){
    TIMx_COUNT->ARR = 0xffffffff;
    TIMx_COUNT->SR  = (~TIM_SR_UIF);
    SET_BIT( TIMx_COUNT->CR1, TIM_CR1_CEN);  // Enable timer
    return 0;
}

/**
 * @brief  Stop the counter. `rh_timer__set()` should be called before using this function.
 * @param  (none)
 * @retval Return the value in millionsecond if success.
 *         Return -1 if time out.
*/
int rh_timer__get( void){
    u32 cnt = TIMx_COUNT->CNT;
    u32 psc = TIMx_COUNT->PSC;

    CLEAR_BIT( TIMx_COUNT->CR1, TIM_CR1_CEN);
    TIMx_COUNT->ARR = 0xffffffff;
    SET_BIT  ( TIMx_COUNT->CR1, TIM_CR1_ARPE);   // Auto-reload preload
    SET_BIT  ( TIMx_COUNT->EGR, TIM_EGR_UG );   // Generate an update

    TIMx_COUNT->SR  = 0x00000000;

    
    return ((cnt+1)*(psc+1))/(CLK_KHz);
}



#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT RandleH *****END OF FILE****/








