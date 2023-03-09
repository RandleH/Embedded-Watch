/**
  ******************************************************************************
  * @file    rh_rtc.c
  * @author  RandleH
  * @brief   This file declare functions related to real time controller.
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
#include "rh_rtc.h"
#include "rh_timer.h"
#include "stm32f4xx.h"









/* Private define ------------------------------------------------------------*/
#define DELAY_MS(x)     do{ rh_timer__delayMS(x) }while(0)
#define DELAY_US(x)     do{ rh_timer__delayUS(x) }while(0)









/* Private functions ---------------------------------------------------------*/
static inline void __enableWriteProtection( void){
    RTC->WPR = 0xFF;                                      // Enable write protection
}

static inline void __disableWriteProtection( void){
    RTC->WPR = 0xCA;                                      // Unlock the write protection on all RTC register
    RTC->WPR = 0x53;
}

static inline bool __enterInitMode( void){
    u32 retry = 0xffff;
    SET_BIT( RTC->ISR, RTC_ISR_INIT);                 // Enter initialization mode
    while( retry-- && false==READ_BIT( RTC->ISR, RTC_ISR_INITF));
    if( retry==0 ){
        return false;
    }
    return true;
}

static inline bool __exitInitMode( void){
    RTC->ISR &= ~(1 << 7);                            // Exit initialization mode
    return true;
}

static u8 __bcd( u8 dec){
    u8 bcdhigh = 0;
    while (dec >= 10){
        bcdhigh++;
        dec -= 10;
    }
    return ((u8)(bcdhigh << 4) | dec);
}

static u8 __dec( u8 bcd){
    u8 tmp = 0;
    tmp = (bcd >> 4) * 10;
    return (tmp + (bcd & 0X0F));
}

static void __setTime( u8 hh, u8 mm, u8 ss){
    __disableWriteProtection();
    __enterInitMode();

    u32 tmp = 0;

    tmp |= __bcd(hh)<<16;
    tmp |= __bcd(mm)<<8;
    tmp |= __bcd(ss);

    RTC->TR = tmp;

    __enableWriteProtection();
    __exitInitMode();
}

static void __setDate( u16 yy, u8 mm, u8 dd, u8 week){
    __disableWriteProtection();
    __enterInitMode();

    u32 tmp = 0;
    
    tmp |= (week&0x07)<<13;
    tmp |= __bcd(yy)<<16;
    tmp |= __bcd(mm)<<8;
    tmp |= __bcd(dd);

    RTC->DR = tmp;

    __enableWriteProtection();
    __exitInitMode();
}

/**
 * @brief       等待RSF同步
 * @param       无
 * @retval      0,成功;1,失败;
 */
static int __waitSynchro(void){
    uint32_t retry = 0XFFFFF;
    __disableWriteProtection();

    while (retry && ((RTC->ISR & (1 << 5)) == 0x00))
    {
        retry--;    /* 等待影子寄存器同步 */
    }
    
    if (retry == 0)return 1;/* 同步失败 */

    __enableWriteProtection();
    return 0;
}




/* Functions -----------------------------------------------------------------*/
/**
 * @brief This function will initialize the RTC module.
 * @param reset  - If true, it will reset the power register. 
 *                 It is recommanded to reset when it is the first time for target to run
 * @retval  Retuan 0 if success.
 *          Return 1 if LSI clock is failed to enable.
 *          Return 2 if LSE clock is failed to enable. The LSE crystal may not be on the board.
 *          Return 3 if RTC can not enter the initialization mode.
*/
int rh_rtc__init( bool reset, u16 yyyy, u8 mm, u8 dd, u8 hh, u8 min, u8 ss, u8 week ){

    SET_BIT( RCC->APB1ENR, RCC_APB1ENR_PWREN );          // Enable backup power clock bus
    SET_BIT( PWR->CR     , PWR_CR_DBP);                  // Disable backup domain write protection

    SET_BIT( RCC->CSR   , RCC_CSR_LSION);                // Enable LSI clock bus
    
    {// Wait until success
        u8 retry = 0xff;
        while( retry-- && false==READ_BIT( RCC->CSR, RCC_CSR_LSIRDY) ); 
        if( retry==0 ){
            return 1;
        }
    }
    
    SET_BIT( RCC->BDCR  , RCC_BDCR_LSEON);               // Enable LSE clock bus

    {// Wait until success
        u8 retry = 0xff;
        while( retry-- && false==READ_BIT( RCC->BDCR, RCC_BDCR_LSERDY ) );
        if( retry==0 ){
            return 2;
        }
    }

    {// RTC clock source selection: Clear selection
        u32 tmp = RCC->BDCR;
        CLEAR_BIT( tmp, RCC_BDCR_RTCSEL_0 );             // [00]: No clock; [01]: LSE; [10]: LSI; [11]:HSE;
        CLEAR_BIT( tmp, RCC_BDCR_RTCSEL_1 );
        RCC->BDCR = tmp;
    }

    {// RTC clock source selection: LSE  selection
        u32 tmp = RCC->BDCR;
        SET_BIT  ( tmp, RCC_BDCR_RTCSEL_0 );     
        CLEAR_BIT( tmp, RCC_BDCR_RTCSEL_1 );

        SET_BIT  ( tmp, RCC_BDCR_RTCEN    );              // Enable RTC clock bus
        RCC->BDCR = tmp;
    }

    __disableWriteProtection();
    RTC->CR  = 0;                                         // Reset RTC control rigister
    
    if( !__enterInitMode() )                              // Enter initialization mode
        return 3;


    u32 pres_s=0xff, pres_a=0x7f;                         // Set synchronous/asynchronous prescaler factor
    
    RTC->PRER  = (u32)((0x7fff)&pres_s);                  // Frtc = Fclk/[(pres_s+1)*(pres_a+1)]
    RTC->PRER |= (u32)(((0x7f)&pres_a)<<16);              // Fclk = 36.768 kHz

    CLEAR_BIT( RTC->CR, RTC_CR_FMT);                      // 24 Hour Format
    
    __exitInitMode();
    __enableWriteProtection();
    
    if( reset){
        __setTime( hh, min, ss);
        __setDate( yyyy, mm, dd, week);
    }
    
    return 0;
}


int rh_rtc__getTime( u8 *hh, u8 *mm, u8 *ss ){
    if( 0!=__waitSynchro()){
        return 1;
    }

    u32 temp = RTC->TR;

    if( hh)
        *hh = ((temp >> 22)&0x01)==true ? 12+__dec((temp >> 16) & 0X3F) : __dec((temp >> 16) & 0X3F);
    
    if(mm)
        *mm = __dec((temp >> 8) & 0X7F);
    
    if(ss)
        *ss = __dec(temp & 0X7F);

    return 0;
}






