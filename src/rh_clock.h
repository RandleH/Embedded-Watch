/**
  ******************************************************************************
  * @file    rh_clock.h
  * @author  RandleH
  * @brief   This file contains the source code of clock configuration.
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


#include "rh_common.h"

#ifndef RH_CLOCK_H
#define RH_CLOCK_H

#ifdef __cplusplus
extern "C"{
#endif



int  rh_clock__init( u32 PLLM, u32 PLLN, u32 PLLP, u32 PLLQ);

void rh_clock__debug_printf( int(*printf)(const char *fmt, ...));

#ifdef __cplusplus
}
#endif

#endif


/************************ (C) COPYRIGHT RandleH *****END OF FILE****/