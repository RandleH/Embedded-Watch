/**
  ******************************************************************************
  * @file    port.hh
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
#include <cstddef>
#include <utility>
#include "application/rh_visual.hh"
#include "application/rh_asset.h"
#include "rh_common.h"
#include "rh_color.h"


#ifndef PORT_524505_H
#define PORT_524505_H


#define OPT_FORWARD      0
#define OPT_SET          1
#define OPT_RESET        2
#define OPT_BACKWARD     4




/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{



/******************************************************************************/
/* Private Component in Widget 524505                                         */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class ClockNeedleComponent{
private:
    lv_obj_t  *_obj;
    lv_style_t _style;            /*!< Line style for main part such as width and color */
    u8         _len;              /*!< Length of the needle */
    u8         _offset;           /*!< Length of other side of the needle */
    u32        _tick;             /*<! Current tick count */
    lv_point_t _point[2];         /*!< End point coordinate of needle */
    u32        _ratio;            /*!< Systick tick vs. Needle Move Step */

    void       updateAngle( void);
public:
    ClockNeedleComponent( void* screen, u8 len, u8 offset, u8 width, bool rounded, lv_color_t color);

    void  run( u32 tick, int OPT_XXXX );

    void  hide( bool cmd);

    u32   ratio( void);
    void  ratio( u32 val);

    u8    length( void);
    void  length( u8 val);

    u8    width( void);
    void  width( u8 val);

    u8    offset( void);
    void  offset( u8 val);
};


/******************************************************************************/
/* Private Component in Widget 524505                                         */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class ClockNeedleKnobComponent : public rh::UIComponent{
private:

public:
    ClockNeedleKnobComponent( void* screen, u8 radius, u8 width, lv_color_t color);
    void hide( bool cmd);

    u8    width( void);
    void  width( u8 val);

    u8    radius( void);
    void  radius( u8 val);

    lv_color_t   color( void);
    void         color( lv_color_t val);
};


/******************************************************************************/
/* Private Component in Widget 524505                                         */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class ClockPanelComponent : public rh::UIComponent{
private:
    lv_style_t style;         
public:
    ClockPanelComponent( void *screen, bool day_night);
    void hide( bool cmd);
};


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class Widget final: public rh::ClockWidget{
private:
    lv_style_t               style;  
    ClockPanelComponent      ccClockPanel;
    ClockNeedleComponent     ccNeedleSecond;
    ClockNeedleComponent     ccNeedleMinute;
    ClockNeedleComponent     ccNeedleHour;
    ClockNeedleKnobComponent ccNeedleKnob;

public:
    Widget( void * screen);
    int setTime( u8 hh, u8 mm, u8 ss );
    int update( void);
};








}
}

#endif