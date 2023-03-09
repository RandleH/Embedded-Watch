/**
  ******************************************************************************
  * @file    rh_rtc.h
  * @author  RandleH
  * @brief   This file declare functions related to real time controller.
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
#include <stdbool.h>
#include "rh_common.h"

#ifndef RH_RTC_H
#define RH_RTC_H


#ifdef __cplusplus
extern "C"{
#endif





int rh_rtc__init( bool reset, u16 yyyy, u8 mm, u8 dd, u8 hh, u8 min, u8 ss, u8 week );

int rh_rtc__getTime( u8 *hh, u8 *mm, u8 *ss );

#ifdef __cplusplus
}
#endif


#endif
