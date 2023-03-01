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
#include "./application/rh_app.hh"
#include "lvgl.h"
#include "./port.hh"

#include "./gui_guider.h"


#include "./lv_analogclock.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_MINUTE_TICK           lv_color_hex(0x006b6b6b)
#define COLOR_HOUR_TICK             lv_color_hex(0x00ffffff)

#define LEN_MINUTE_TICK             (10)
#define LEN_HOUR_TICK               (13)

#define WIDTH_MINUTE_TICK           (3)
#define WIDTH_HOUR_TICK             (3)

#define COLOR_MINUTE_NEEDLE         lv_color_hex(0x00ffffff)
#define COLOR_HOUR_NEEDLE           lv_color_hex(0x00ffffff)
#define COLOR_SECOND_NEEDLE         lv_color_hex(0x00ff0029)


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{




ClockPanelComponent::ClockPanelComponent( void* screen){
    /* Create the object */
    obj = lv_analogclock_create( (lv_obj_t*)screen);
    lv_obj_set_pos(obj, 0, 0);                  /* No XY offset */
    lv_obj_set_size(obj, 240, 240);             /* Full screen display */
    lv_analogclock_hide_digits(obj, false);     /* Display clock number */
    lv_analogclock_set_major_ticks(obj, WIDTH_HOUR_TICK  , LEN_HOUR_TICK  , COLOR_HOUR_TICK  , 10);       /* Hour Tick */
    lv_analogclock_set_ticks      (obj, WIDTH_MINUTE_TICK, LEN_MINUTE_TICK, COLOR_MINUTE_TICK);           /* Minute Tick */

    lv_analogclock_set_hour_needle_line(obj, 5, COLOR_HOUR_NEEDLE  , -56);
    lv_analogclock_set_min_needle_line (obj, 5, COLOR_MINUTE_NEEDLE, -30);
    lv_analogclock_set_sec_needle_line (obj, 2, COLOR_SECOND_NEEDLE, -10);
    
    
    lv_analogclock_set_time( obj, app.resource.time.bit.hour  ,\
                                  app.resource.time.bit.minute,\
                                  app.resource.time.bit.second);

}


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
Widget::Widget( void * screen):rh::ClockWidget{screen},ccClockPanel{screen}{
    /* Config screen */
    lv_obj_set_scrollbar_mode( (lv_obj_t*)screen, LV_SCROLLBAR_MODE_OFF);
    
    /* Config style option */
    lv_style_reset( &style);
    lv_style_init( &style);
    lv_style_set_bg_color(&style, lv_color_make(0x0d, 0x0d, 0x0d));
    lv_style_set_bg_opa(&style, 255);
    lv_obj_add_style((lv_obj_t*)screen, &style, LV_PART_MAIN|LV_STATE_DEFAULT);
}

int Widget::increaseTick( u32 tick){
    return 0;
}

int Widget::setTime( bool am_pm, u8 hour, u8 minute, u8 second ){
    if( hour>=24 || minute>=60 || second>=60 ) return 1;

    
    lv_analogclock_set_time( ccClockPanel.obj, hour, minute, second);
    return 0;
}
    
int Widget::setDayNight( bool day_night){
    return 0;
}





}
}