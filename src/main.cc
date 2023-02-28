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

#include "application/rh_app.hh"



#include <CMSIS_RTOS_V2/cmsis_os2.h>







/* Global variables ----------------------------------------------------------*/
bool key0 = false;






rh::Application    app;










int main( int argc, char const *argv[] ){
    HAL_Init();
    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4);

    
    
    rh_clock__init( 25, 192, 2, 4);
    rh_timer__init();
    rh_debug__init();
    

    rh_light__init();


    
    
   
    app.thread.init();
    app.start();
    


    while(1){
        
        // if( (app.resource.userTick/1000) %2 ){
        //     rh_light__switch( RH_LED_IDX__BLUE, true);
        // }else{
        //     rh_light__switch( RH_LED_IDX__BLUE, false);
        // }

        
    }
    return 0;
}