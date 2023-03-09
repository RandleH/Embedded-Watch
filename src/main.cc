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
#include "rh_timer.h"

#include "application/rh_app.hh"      /* Application Program */

#if RH_CFG_RTOS_ENABLE==1

#else
# include "rh_light.h"                 /* BSP Package */
# include "rh_screen.h"
# include "rh_flash.h"
# include "rh_key.h"
# include "rh_rtc.h"

# include "rh_test.h"                  /* Unit Test Experiments */ 

# include "application/widgets/clock524505/lv_analogclock.h"
#endif





/* Global variables ----------------------------------------------------------*/
bool key0 = false;
rh::Application    app;


int main( int argc, char const *argv[] ){

  HAL_Init();
  HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4);
  rh_clock__init( 25, 192, 2, 4);
  // rh_rtc__init( false, 1970, 1,1,0,0,0, 1);
  rh_timer__init();
  rh_debug__init();
#if RH_CFG_RTOS_ENABLE==1 //////////////////////////////////////////////////
  app.thread.init();
  app.start();
  while(1);

#else //////////////////////////////////////////////////////////////////////
  
  rh_screen__init();
  rh_key__init();
  rh_light__init();
  
  
 
  u32 tmp = app.resource.userTick;
  
  
  void*  list[] = { &app.gui.uiClockClassic, &app.gui.uiClockWheel};
  rh::ClockWidget  *pClkWidget = &app.gui.uiClockClassic;

  pClkWidget->setTick( 0 );
  lv_scr_load( (lv_obj_t*)pClkWidget->getScreen());

  
  pClkWidget->setTime( app.resource.time.bit.hour, app.resource.time.bit.minute, app.resource.time.bit.second );
  
  app.gui.uiClockLouisVuitton.foo();
  
  while(1){

    // if( key0==true ){
    //   if( RH_KEY__B==rh_key__read() ){    
    //     rh_light__toggle( RH_LED_IDX__BLUE);
    //     std::swap( list[0], list[1]);
    //     pClkWidget = (rh::ClockWidget*)list[0];

    //     pClkWidget->setTime( app.resource.time.bit.hour, app.resource.time.bit.minute, app.resource.time.bit.second);
    //     lv_scr_load( (lv_obj_t*)pClkWidget->getScreen() );
    //   }
    //   key0 = false;
    // }

    // {
    //   u32 tmpVolatile = app.resource.userTick;
    //   pClkWidget->incTick( tmpVolatile-tmp );
    //   tmp = tmpVolatile;
    // }

    // pClkWidget->update();
    
    
    lv_tick_inc(10);
    lv_timer_handler();
  }

#endif
  return 0;
}



