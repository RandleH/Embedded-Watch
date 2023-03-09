/**
  ******************************************************************************
  * @file    port.cc
  * @author  RandleH
  * @brief   This file contains source code for ui widget component 880289.         
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



/* Private define ------------------------------------------------------------*/
#define CLOCKWHEEL_START_ANGLE      (270)
#define CLOCKWHEEL_END_ANGLE        (270+360)

#define COLOR_SECOND_DK             RH_COLOR_MAKE( 126,  48,  93)
#define COLOR_SECOND_LT             RH_COLOR_MAKE( 237,  29, 113)

#define COLOR_MINUTE_DK             RH_COLOR_MAKE(  61, 129,  38)
#define COLOR_MINUTE_LT             RH_COLOR_MAKE(  88, 238,  36)

#define COLOR_HOUR_DK               RH_COLOR_MAKE(  49,  53, 114)
#define COLOR_HOUR_LT               RH_COLOR_MAKE(  70,  78, 226)



/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget880289{




/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
ClockWheelPictureComponent::ClockWheelPictureComponent( void * screen, const lv_img_dsc_t *day, const lv_img_dsc_t *night):
PictureComponent( screen), day_night(false){
    addImg(   day, true);
    addImg( night, false);
}

int ClockWheelPictureComponent::setDayNight( bool day_night){
    this->day_night = day_night;
    lv_img_set_src( obj, asset[(int)day_night]);
    return 0;
}




/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
ClockWheelComponent::ClockWheelComponent( void * screen, u16 diameter, u16 width):screen((lv_obj_t*)screen),angle(0){
    obj = lv_arc_create( (lv_obj_t*)screen);
    lv_obj_set_width ( obj, diameter);         
    lv_obj_set_height( obj, diameter);

    lv_obj_set_style_arc_width( obj, width, LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width( obj, width, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_arc_set_bg_angles  ( obj,   0, 360);
    lv_arc_set_start_angle( obj, CLOCKWHEEL_START_ANGLE );
    lv_arc_set_end_angle  ( obj, CLOCKWHEEL_START_ANGLE );

    lv_obj_remove_style   ( obj, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag     ( obj, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center( obj);
}

int ClockWheelComponent::setRatio( u32 tick_deg){
    ratioTick = tick_deg;
    return 0;
}

int ClockWheelComponent::setColor( u32 color1, u32 color2 ){
    color[0] = lv_color_hex(color1); color[1] = lv_color_hex(color2);
    lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
    lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    return 0;
}

int ClockWheelComponent::setColor( lv_color_t color1, lv_color_t color2 ){
    color[0] = color1; color[1] = color2;
    lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
    lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    return 0;
}

int ClockWheelComponent::increaseTick( u32 tick){
    if( tick==0 ) return 0;
    tick %= this->ratioTick;
    
    u16 deltaAngle = 0;
    
    deltaAngle = (this->tick+tick)/ratioTick;
    this->angle  += deltaAngle;
    this->tick   = (this->tick+tick)%ratioTick;
    
    if( this->angle >= 360 ){
        this->angle -= 360;

        std::swap( this->color[0], this->color[1] );
        lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
        lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    }
    
    if( deltaAngle!=0 ){
        lv_arc_set_end_angle( obj, CLOCKWHEEL_START_ANGLE+this->angle );
    }
        
    return 0;
}

int ClockWheelComponent::setTick( u32 tick){
    return 0;
}


int ClockWheelComponent::setAngle( u16 deg){
    deg%=360;
    this->angle = deg;
    lv_arc_set_end_angle( obj, CLOCKWHEEL_START_ANGLE+deg );
    return 0;
}






/******************************************************************************/
/* This class will present a clock on screen in the manner of wheels          */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
Widget::Widget( void * screen):
rh::ClockWidget{screen},ccSecond(screen, 240, 20), ccMinute(screen, 200, 20), ccHour(screen, 160, 20), ccDayIcon(screen, &ui_img_sun_png, &ui_img_moon_png),tickMod(0),style{0}{
    
    lv_color_t color1, color2;
    color1.full = COLOR_HOUR_DK; color2.full = COLOR_HOUR_LT;
    ccHour.setColor( color1, color2);

    color1.full = COLOR_MINUTE_DK; color2.full = COLOR_MINUTE_LT;
    ccMinute.setColor( color1, color2);

    color1.full = COLOR_SECOND_DK; color2.full = COLOR_SECOND_LT;
    ccSecond.setColor( color1, color2);
    
    ccHour.setRatio  ( 4*60*12);
    ccMinute.setRatio( 4*60);
    ccSecond.setRatio( 4);

    ccDayIcon.setRatio( 3600*12*1440 );

    lv_style_reset( &style);
    lv_style_init( &style);
    lv_style_set_bg_color(&style, lv_color_make(0x0d, 0x0d, 0x0d));
    lv_style_set_bg_opa(&style, 255);
    lv_obj_add_style((lv_obj_t*)screen, &style, LV_PART_MAIN|LV_STATE_DEFAULT);
}


int Widget::update( void){
    int retval = 0;

    /******************************************************************************/
    /* Important Notes: The following code ONLY works for these conditions        */
    /*  - `tickInc`           - 1 tick/ms                                         */
    /*  - `tickIncComponent`  - 1440/1000 tick/ms                                 */
    /******************************************************************************/
#warning "May optimize using left shift"
    u32 tmp = (tickMod+(tickInc<<1)+tickInc);

    u32 tickIncComponent = tmp<125? tmp>>7 : tmp/125;
    tickMod              = tmp%125;

    retval |= ccHour.increaseTick(tickIncComponent);
    retval |= ccMinute.increaseTick(tickIncComponent);
    retval |= ccSecond.increaseTick(tickIncComponent);

    return retval | done();
}



int Widget::setTime( u8 hour, u8 minute, u8 second ){
    
    ccHour.setAngle  ( hour   * 360 /12 );
    ccMinute.setAngle( minute * 360 /60 );
    ccSecond.setAngle( second * 360 /60 );

    ccDayIcon.setDayNight( isDayNight( hour, minute));
    
    ccHour.resetTick();
    ccMinute.resetTick();
    ccSecond.resetTick();
    ccDayIcon.resetTick();
    return 0;
}

int Widget::setDayNight( bool day_night){
    return ccDayIcon.setDayNight( day_night);
}





}
}

/************************ (C) COPYRIGHT RandleH *****END OF FILE***************/