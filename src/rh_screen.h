/**
  ******************************************************************************
  * @file    rh_screen.h
  * @author  RandleH
  * @brief   This file contains code for screen driver.
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


#ifndef RH_SCREEN_H
#define RH_SCREEN_H

/* Includes ------------------------------------------------------------------*/
#include "rh_config.h"


/* Public define ------------------------------------------------------------*/

#define CFG_SCREEN__MAX_WAIT_MS         (HAL_MAX_DELAY)


/* Public typedef -----------------------------------------------------------*/
typedef enum{
    ScreenScanMode__L2R_U2D = 0,
    ScreenScanMode__L2R_D2U,
    ScreenScanMode__R2L_U2D,
    ScreenScanMode__R2L_D2U,
    ScreenScanMode__U2D_L2R,
    ScreenScanMode__U2D_R2L,
    ScreenScanMode__D2U_L2R,
    ScreenScanMode__D2U_R2L
}E_ScreenScanMode_t;

typedef enum{
    ScreenDispDir__V = 0,
    ScreenDispDir__H
}E_ScreenDispDir_t;

typedef uint16_t E_ScreenColor_t;


#ifdef __cplusplus
extern "C"{
#endif


/* Exported functions ------------------------------------------------------- */
int rh_screen__init       ( void);
int rh_screen__reset      ( void);

int rh_screen__fill       ( E_ScreenColor_t arg, int xs, int ys, int xe, int ye);
int rh_screen__fill_all   ( E_ScreenColor_t arg);


int rh_screen__scan_mode  ( E_ScreenScanMode_t arg);

#ifdef __cplusplus
}
#endif


/******************************************************************************/
/*               Application Interface Functions for LVGL 8.2.0               */
/******************************************************************************/

/* Includes ----------------------------------------------------------------- */
#include "rh_common.h"
#include "rh_config.h"
#include "lvgl.h"


#ifdef __cplusplus
extern "C"{
#endif

/* API functions ------------------------------------------------------------ */
int  rhlv_screen__init     ( void);
void rhlv_screen__flush_cb ( struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);


#if RH_CFG_RTOS_ENABLE==0   /* Only for bare metal development */
int rhlv_screen__init(void);
#endif

#ifdef __cplusplus
}
#endif

#endif


/************************ (C) COPYRIGHT RandleH *****END OF FILE****/