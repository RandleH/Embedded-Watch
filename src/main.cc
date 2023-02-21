/**
  ******************************************************************************
  * @file    main.cc
  * @author  RandleH
  * @brief   Main program entrance.
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
#include "stm32f4xx_hal.h"            /* STM32 HAL Library */
#include "stm32f4xx_hal_cortex.h"

#include "lvgl.h"                     /* Third Party Graphic Library */

#include "rh_common.h"                /* Project Configuration */
#include "rh_config.h"
#include "rh_color.h"

#include "rh_clock.h"                 /* System Configuration */
#include "rh_debug.h"

#include "rh_light.h"                 /* BSP Package */
#include "rh_timer.h"
#include "rh_screen.h"
#include "rh_flash.h"
#include "rh_key.h"

#include "rh_test.h"                  /* Unit Test Experiments */ 

#include "ui/rh_visual.hh"            /* Application Module */











/* Global variables ----------------------------------------------------------*/
bool key0 = false;



/**
 * @brief 
 * @param argc
 * @param argv[]
 * 
*/
int main( int argc, char const *argv[] ){
    HAL_Init();
    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4);

    rh_clock__init( 25, 192, 2, 4);
    rh_timer__init();
    rh_debug__init();
    rhlv_screen__init();
    rh_screen__init();
    rh_light__init();
    rh_flash__init();
    rh_key__init();

    rh_debug__printf("APP Start!!\n");

    rh::Visual     gVisual;
    rh::ClockWheel uiClockWheel( gVisual.getScreen( rh::VISUAL_SCREEN__MAIN));

    gVisual.load( rh::VISUAL_SCREEN__MAIN);
    
    uiClockWheel.setTime( false,  5, 5, 0);

    while(1){
        rh_timer__set();
        if( key0==true ){
            rh_timer__delayMS(50);
            if( RH_KEY__B==rh_key__read() ){
                rh_debug__log( "Key0 triggered!\n");
                rh_light__toggle( RH_LED_IDX__BLUE);
            }
            key0 = false;
        }

        uiClockWheel.increaseTick(1);

        lv_tick_inc( rh_timer__get()+1 );
        lv_timer_handler();
    }
    return 0;
}