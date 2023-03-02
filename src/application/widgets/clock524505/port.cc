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
#define COLOR_MINUTE_TICK(cmd)           (cmd==true)?lv_color_hex(0x006b6b6b) : lv_color_hex(0x006b6b6b) 
#define COLOR_HOUR_TICK(cmd)             (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 

#define LEN_MINUTE_TICK             (15)
#define LEN_HOUR_TICK               (20)

#define WIDTH_MINUTE_NEEDLE           (3)
#define WIDTH_HOUR_NEEDLE             (3)

#define COLOR_MINUTE_NEEDLE(cmd)         (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 
#define COLOR_HOUR_NEEDLE(cmd)           (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 
#define COLOR_SECOND_NEEDLE(cmd)         (cmd==true)?lv_color_hex(0x00ff0029) : lv_color_hex(0x00ff0029) 


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{




ClockPanelComponent::ClockPanelComponent( void* screen, bool day_night){
    /* Create the object */
    obj = lv_analogclock_create( (lv_obj_t*)screen);
    lv_obj_set_pos(obj, 0, 0);                  /* No XY offset */
    lv_obj_set_size(obj, 240, 240);             /* Full screen display */
    lv_analogclock_hide_digits(obj, false);     /* Display clock number */

    
    lv_analogclock_set_major_ticks(obj, WIDTH_HOUR_NEEDLE  , LEN_HOUR_TICK  , COLOR_HOUR_TICK(day_night)  , 20);       /* Hour Tick */
    lv_analogclock_set_ticks      (obj, WIDTH_MINUTE_NEEDLE, LEN_MINUTE_TICK, COLOR_MINUTE_TICK(day_night));           /* Minute Tick */

    lv_analogclock_set_hour_needle_line(obj, 5, COLOR_HOUR_NEEDLE(day_night)  , -56);
    lv_analogclock_set_min_needle_line (obj, 5, COLOR_MINUTE_NEEDLE(day_night), -30);
    lv_analogclock_set_sec_needle_line (obj, 2, COLOR_SECOND_NEEDLE(day_night), -10);
    
    
    lv_analogclock_set_time( obj, app.resource.time.bit.hour  ,\
                                  app.resource.time.bit.minute,\
                                  app.resource.time.bit.second);

}

void ClockPanelComponent::setTime( u8 hh, u8 mm, u8 ss){
    lv_analogclock_set_time( obj, hh, mm, ss);
}


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
Widget::Widget( void * screen):rh::ClockWidget{screen},ccClockPanel{screen, isDayNight( app.resource.time.bit.hour, app.resource.time.bit.minute )}{
    /* Config screen */
    lv_obj_set_scrollbar_mode( (lv_obj_t*)screen, LV_SCROLLBAR_MODE_OFF);
    
    /* Config style option */
    lv_style_reset( &style);
    lv_style_init( &style);
    lv_style_set_bg_color(&style, lv_color_make(0x0d, 0x0d, 0x0d));
    lv_style_set_bg_opa(&style, 255);
    lv_obj_add_style((lv_obj_t*)screen, &style, LV_PART_MAIN|LV_STATE_DEFAULT);
}


int Widget::setTime( u8 hh, u8 mm, u8 ss ){
    if( hh>=24 || mm>=60 || ss>=60 ) return 1;
    ccClockPanel.setTime( hh, mm, ss);
    
    return 0;
}

/**
 * @class rh::widget524505::Widget
 * @brief 
*/
int Widget::update( void){

#warning "Need to be fixed!!! It actually do NOT have the permission to access global resource"

    setTime( app.resource.time.bit.hour, app.resource.time.bit.minute, app.resource.time.bit.second );
    
    return done();;
}
    
int Widget::setDayNight( bool day_night){
    
    return 0;
}





}
}