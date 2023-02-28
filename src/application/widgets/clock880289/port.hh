/**
  ******************************************************************************
  * @file    clock880289.hh
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
#include <cstddef>
#include <utility>
#include "application/rh_visual.hh"
#include "application/rh_asset.h"
#include "rh_common.h"
#include "rh_color.h"


#ifndef PORT_880289_H
#define PORT_880289_H





/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget880289{


/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
class ClockWheelPictureComponent : public PictureComponent{
private:
    bool  day_night;
public:
    ClockWheelPictureComponent( void * screen, const lv_img_dsc_t *day, const lv_img_dsc_t *night);

    int setDayNight( bool day_night);

};



/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
class ClockWheelComponent:public UIComponent{
private:
    lv_obj_t * screen;
    lv_obj_t * obj;
    u16        angle;
    lv_color_t color[2];

public:
    ClockWheelComponent( void * screen, u16 diameter, u16 width);

    int init( u16 diameter, u16 width);
    int setRatio( u32 tick_deg);
    int setColor( u32 color1, u32 color2 );
    int setColor( lv_color_t color1, lv_color_t color2 );

    int increaseTick( u32 tick);
    int increase( void);
    int setAngle( u16 deg);

friend class Widget;
};





/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class Widget{
private:    
    ClockWheelComponent           ccSecond;
    ClockWheelComponent           ccMinute;
    ClockWheelComponent           ccHour;
    ClockWheelPictureComponent    ccDayIcon;
    u32 tick;
    bool am_pm;

public:
    Widget( void * screen);
    int init( void);
    int increaseTick( u32 tick );
    int setTime( bool am_pm, u8 hour, u8 minute, u8 second );
    int setDayNight( bool day_night);
};











} //////// namespace: widget880289
} //////// namespace: rh


#endif