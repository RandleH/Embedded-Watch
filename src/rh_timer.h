/**
  ******************************************************************************
  * @file    rh_timer.h
  * @author  RandleH
  * @brief   This file contains the source code of delay function on STM32F103C8T6.
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
#include "stm32f4xx.h"
#include "rh_config.h"

#ifdef __cplusplus
extern "C"{
#endif



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

int rh_timer__init   ( void);
int rh_timer__delayMS( unsigned long ms);
int rh_timer__delayUS( unsigned long us);
int rh_timer__set    ( void);
int rh_timer__get    ( void);


#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT RandleH *****END OF FILE****/