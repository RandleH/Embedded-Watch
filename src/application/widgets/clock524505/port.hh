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





/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{


class ClockPanelComponent : public rh::UIComponent{
private:
      
public:
    ClockPanelComponent( void *screen);
    friend class Widget;
};


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class Widget final: public rh::ClockWidget{
private:
    lv_style_t   style;  
    ClockPanelComponent  ccClockPanel;
public:
    Widget( void * screen);
    int increaseTick( u32 tick );
    int setTime( bool am_pm, u8 hour, u8 minute, u8 second );
    int setDayNight( bool day_night);
};








}
}

#endif