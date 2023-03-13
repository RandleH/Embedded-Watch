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
#include "./config.h"
#include "rh_common.h"
#include "rh_color.h"


#ifndef PORT_454570_H
#define PORT_454570_H


#define OPT_FORWARD      0
#define OPT_SET          1
#define OPT_RESET        2
#define OPT_BACKWARD     4


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget454570{


/******************************************************************************/
/* Private UI Component                                                       */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class ClockPanelComponent{
private:
    lv_obj_t *_obj_icon[12];
public:
    ClockPanelComponent( void* screen, u32 hex_c1, u32 hex_c2);
    void colorGradient( u32 hex_c1, u32 hex_c2);
};




class ClockNeedleComponent{
private:
    lv_obj_t* _obj;
    u32        _tick;             /*<! Current tick count */
    u32        _ratio;            /*!< Systick tick vs. Needle Move Step */
    lv_color_t _color;            /*!< Needle color */
public:
    ClockNeedleComponent( void* screen, u32 ratio, u32 hex_c, const void* imgSrc, i16 pivotX, i16 pivotY, i16 posX, i16 poxY);

    void  run( u32 tick, int OPT_XXXX );

    u32   ratio( void) const;
    void  ratio( u32 val);

    lv_color_t color( void) const;
    void       color( u32 hexColor);

};


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class Widget final: public rh::ClockWidget{
private:    
#if WIDGET_SHOW_CLOCK_PANEL
    ClockPanelComponent   ccPanel;
#endif

#if WIDGET_SHOW_NEEDLE_SECOND
    ClockNeedleComponent  ccNeedleSecond;
#endif

#if WIDGET_SHOW_NEEDLE_MINUTE
    ClockNeedleComponent  ccNeedleMinute;
#endif 

#if WIDGET_SHOW_NEEDLE_HOUR
    ClockNeedleComponent  ccNeedleHour;
#endif

public:
    Widget( void * screen);
    int setTime( u8 hour, u8 minute, u8 second );
    int setDayNight( bool day_night);
    int update( void);
};




} //////// namespace: widget880289
} //////// namespace: rh


#endif