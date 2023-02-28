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

#include "rh_light.h"                 /* BSP Package */
#include "rh_screen.h"
#include "rh_flash.h"
#include "rh_key.h"


#include "rh_test.h"                  /* Unit Test Experiments */ 

#include "application/widgets/clock524505/lv_analogclock.h"

#endif





/* Global variables ----------------------------------------------------------*/
bool key0 = false;
rh::Application    app;


int main( int argc, char const *argv[] ){

  HAL_Init();
  HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4);
  rh_clock__init( 25, 192, 2, 4);
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
  rhlv_screen__init();

  lv_obj_t *scr = lv_obj_create(NULL);

  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	static lv_style_t style_screen_main_main_default;
	if (style_screen_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_main_main_default);
	else
		lv_style_init(&style_screen_main_main_default);
	lv_style_set_bg_color(&style_screen_main_main_default, lv_color_make(0x0d, 0x0d, 0x0d));
	lv_style_set_bg_opa(&style_screen_main_main_default, 255);
	lv_obj_add_style(scr, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool screen_uiClassicClock_timer_enabled = false;

  lv_obj_t *obj = lv_analogclock_create( (lv_obj_t*)scr);
  
  static int screen_uiClassicClock_hour_value = 4;
  static int screen_uiClassicClock_min_value = 20;
  static int screen_uiClassicClock_sec_value = 50;

  lv_obj_set_pos(obj, 0, 0);
	lv_obj_set_size(obj, 240, 240);
	lv_analogclock_hide_digits(obj, false);
	lv_analogclock_set_major_ticks(obj, 2, 13, lv_color_make(0xff, 0xff, 0xff), 10);
	lv_analogclock_set_ticks(obj, 2, 10, lv_color_make(0x6b, 0x6b, 0x6b));
	lv_analogclock_set_hour_needle_line(obj, 5, lv_color_make(0xff, 0xff, 0xff), -56);
	lv_analogclock_set_min_needle_line(obj, 5, lv_color_make(0xff, 0xff, 0xff), -30);
	lv_analogclock_set_sec_needle_line(obj, 2, lv_color_make(0xff, 0x00, 0x27), -10);
	lv_analogclock_set_time(obj, screen_uiClassicClock_hour_value, screen_uiClassicClock_min_value, screen_uiClassicClock_sec_value);

  lv_scr_load(scr);


  while(1){
    if( (app.resource.userTick/1000) %2 ){
      rh_light__switch( RH_LED_IDX__BLUE, true);
    }else{
      rh_light__switch( RH_LED_IDX__BLUE, false);
    }
    lv_tick_inc(10);
    lv_timer_handler();
  }

#endif  
    return 0;
}