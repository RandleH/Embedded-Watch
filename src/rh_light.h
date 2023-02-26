/**
  ******************************************************************************
  * @file    rh_light.h
  * @author  RandleH
  * @brief   This file contains led control source code.
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

#ifndef RH_LIGHT_H
#define RH_LIGHT_H



/* Includes ------------------------------------------------------------------*/
#include "rh_common.h"


#define RH_LED_IDX__RED         (0)
#define RH_LED_IDX__GREEN       (1)
#define RH_LED_IDX__BLUE        (2)


#ifdef __cplusplus
extern "C"{
#endif

int  rh_light__init( void);
void rh_light__switch( int idx, bool cmd);
void rh_light__toggle( int idx);


#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT RandleH *****END OF FILE****/

