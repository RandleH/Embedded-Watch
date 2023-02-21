/**
  ******************************************************************************
  * @file    rh_test.h
  * @author  RandleH
  * @brief   This file contains test function for BSP device. To use, make sure
  *          enable the correspond config macro defined in "rh_config.h"
  * @include "rh_config.h"
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

#ifndef RH_TEST_H
#define RH_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "rh_config.h"
#include "rh_debug.h"


/* Exported functions ------------------------------------------------------- */
#if (RH_CFG_TEST_BSP)

typedef enum{
    TestDevice__screen,
    TestDevice__wireless,
    TestDevice__bluethooth,
    TestDevice__usbflash
}E_TestDevice_t;

typedef enum{
    TestScreen__helloworld,
    TestScreen__colorwheel,
    TestScreen__spinner,
    TestScreen__arc
}E_TestScreen_t;

int rh_test__bsp_screen( E_TestScreen_t arg);

#endif

#endif
