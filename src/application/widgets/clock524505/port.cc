/**
  ******************************************************************************
  * @file    port.cc
  * @author  RandleH
  * @brief   This file contains source code for ui widget component 524505.         
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
#include "rh_color.h"
#include "lvgl.h"
#include "./port.hh"

#include "./gui_guider.h"


#include "./lv_analogclock.h"

/* Private define ------------------------------------------------------------*/



/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{






class ClockPanelComponent : public rh::UIComponent{
public:
    ClockPanelComponent( void *screen);

};

static int screen_uiClassicClock_hour_value = 4;
static int screen_uiClassicClock_min_value = 20;
static int screen_uiClassicClock_sec_value = 50;
ClockPanelComponent::ClockPanelComponent( void* screen){
    this->obj = lv_analogclock_create( (lv_obj_t*)screen);

    lv_obj_set_pos(obj, 0, 0);
	lv_obj_set_size(obj, 240, 240);
	lv_analogclock_hide_digits(obj, false);
	lv_analogclock_set_major_ticks(obj, 2, 13, lv_color_make(0xff, 0xff, 0xff), 10);
	lv_analogclock_set_ticks(obj, 2, 10, lv_color_make(0x6b, 0x6b, 0x6b));
	lv_analogclock_set_hour_needle_line(obj, 5, lv_color_make(0xff, 0xff, 0xff), -56);
	lv_analogclock_set_min_needle_line(obj, 5, lv_color_make(0xff, 0xff, 0xff), -30);
	lv_analogclock_set_sec_needle_line(obj, 2, lv_color_make(0xff, 0x00, 0x27), -10);
	lv_analogclock_set_time(obj, screen_uiClassicClock_hour_value, screen_uiClassicClock_min_value, screen_uiClassicClock_sec_value);

}


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
// class Widget : public rh::ClockWidget{
// private:
//     ClockPanelComponent ccClockPanel;

// public:
//     Widget( void * screen);
//     int init( void);
//     int increaseTick( u32 tick );
//     int setTime( bool am_pm, u8 hour, u8 minute, u8 second );
//     int setDayNight( bool day_night);
// };


Widget::Widget( void * screen):rh::ClockWidget{screen}{
    lv_ui config;
    setup_scr_screen( &config);
    

}




}
}