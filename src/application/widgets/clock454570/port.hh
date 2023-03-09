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


#ifndef PORT_454570_H
#define PORT_454570_H



/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget454570{


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
class Widget final: public rh::ClockWidget{
private:    


public:
    Widget( void * screen);
    int setTime( u8 hour, u8 minute, u8 second );
    int setDayNight( bool day_night);
    int update( void);
    void foo( void);
};




} //////// namespace: widget880289
} //////// namespace: rh


#endif