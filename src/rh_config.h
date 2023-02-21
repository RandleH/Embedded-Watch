/**
  ******************************************************************************
  * @file    template.txt
  * @author  RandleH
  * @brief   This file contains code template.
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

/// The following source file will be affected!!!
/// =============================================================== 
///  Name                 | Directory    | Config Code Line
/// ----------------------+--------------+-------------------------
///  rh_debug.x           |              | 
/// ----------------------+--------------+-------------------------
///  rh_screen.x          |              | 43
/// ----------------------+--------------+-------------------------
///  rh_light.x           |              | 
/// ----------------------+--------------+-------------------------
///  rh_task.x            |              | 
/// ---------------------------------------------------------------




#ifndef _RH_CONFIG_H
#define _RH_CONFIG_H

#include "RH_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RH_CFG_RECORD_VERSION                 "RH_CONFIG - 02/13/2023"


/* Screen Config -------------------------------------------------------------*/
#define RH_CFG_SCREEN_HEIGHT                  (240U)                        // < select >
#define RH_CFG_SCREEN_WIDTH                   (240U)                        // < select >

#define RH_CFG_GRAPHIC_COLOR_BIN              (0U)                          // Do NOT modify
#define RH_CFG_GRAPHIC_COLOR_RGB565           (1U)                          // Do NOT modify
#define RH_CFG_GRAPHIC_COLOR_RGB888           (2U)                          // Do NOT modify
#define RH_CFG_GRAPHIC_COLOR_TYPE              1                            // < select > < above option >

#define RH_CFG_GRAM_INTERNAL                  (0U)                          // Do NOT modify
#define RH_CFG_GRAM_EXTADDR                   (1U)                          // Do NOT modify
#define RH_CFG_GRAM_EXTSECT                   (2U)                          // Do NOT modify
#define RH_CFG_GRAM_EXTPTR                    (3U)                          // Do NOT modify
#define RH_CFG_GRAM_TYPE                      RH_CFG_GRAM_INTERNAL          // < select > < above option >

#define RH_CFG_GRAM_DMA_ENABLE                (1)                           // < select > < 0=disable : 1=enable >
#define RH_CFG_GRAM_SIZE                      (10*RH_CFG_SCREEN_WIDTH)      // < select >  


/* Debug Config --------------------------------------------------------------*/
#define RH_CFG_DEBUG_TITLE_WARNING            "[WARNING]"
#define RH_CFG_DEBUG_TITLE_ERROR              "[ERROR]"
#define RH_CFG_DEBUG_TITLE_LOG                "[LOG]"

#define RH_CFG_DEBUG_ERROR_MASSEGE_ENABLE     (1)                           // < select > < 0=disable : 1=enable >
#define RH_CFG_DEBUG_WARNING_MASSEGE_ENABLE   (1)                           // < select > < 0=disable : 1=enable >
#define RH_CFG_DEBUG_LOG_MASSEGE_ENABLE       (1)                           // < select > < 0=disable : 1=enable >


/* Timer Config --------------------------------------------------------------*/
#define RH_CFG_TIMER_BUS_FREQ_MHz             (96U)

#define RH_CFG_TIMER_DELAY                    TIM3   
#define RH_CFG_TIMER_DELAY_ISRFUNC            TIM3_Handler
#define RH_CFG_TIMER_DELAY_CLKCMD             RCC_APB1ENR_TIM3EN

#define RH_CFG_TIMER_COUNT                    TIM2                          // < select > < TIM2 | TIM5 >    MUST support 32-bit count mode.
#define RH_CFG_TIMER_COUNT_ISRFUNC            TIM2_Handler
#define RH_CFG_TIMER_COUNT_CLKCMD             RCC_APB1ENR_TIM2EN


#define RH_CFG_TIMER_PWM                      TIM4


/* Flash Config --------------------------------------------------------------*/
#define RH_CFG_FLASH_DEBUG                    (1)                           // < select > < 0=disable : 1=enable >



/* TEST Config ---------------------------------------------------------------*/
#define RH_CFG_TEST_BSP                       (1)                           // < select > < 0=disable : 1=enable >

/* RTOS Config ---------------------------------------------------------------*/
#define RH_CFG_RTOS_ENABLE                    (0)                           // < select > < 0=disable : 1=enable >








/* Legacy Code (NO USE for this project) -------------------------------------*/

#ifndef RH_ASSERT
  #define RH_ASSERT( expr )                   assert( expr )
  #define STBTT_assert(expr)                  RH_ASSERT(expr)
#else
  #error " 'RH_ASSERT' has been defined. "
#endif

#ifndef RH_MESSAGE
#define RH_MESSAGE(format,...)                printf(format,##__VA_ARGS__)
#endif

#ifndef RH_CALLOC
  #define RH_CALLOC(x,size)                   calloc(x,size)
#else
  #error " 'RH_CALLOC' has been defined. "
#endif

#ifndef RH_MALLOC
//#include "BLK_lib.h"
  #define RH_MALLOC(x)                        malloc(x)

#else
  #error " 'RH_MALLOC' has been defined. "
#endif

#ifndef RH_REALLOC
#define RH_REALLOC(p,s)                       realloc(p,s)
#else
  #error " 'RH_REALLOC' has been defined. "
#endif

#ifndef RH_FREE
  #define RH_FREE(x)                          free(x)//BLK_FUNC( Memory, debug_free )(x,free)
#else
  #error " 'RH_FREE' has been defined. "
#endif


#define BLK_POKER_CALLOC(x,s)                 calloc(x,s)
#define BLK_POKER_MALLOC(x)                   malloc(x)
#define BLK_POKER_FREE(x)                     free(x)
#define BLK_POKER_ASSERT(expr)                assert(expr)
       
#define BLK_DATA_CALLOC(x,s)                  calloc(x,s)
#define BLK_DATA_MALLOC(x)                    malloc(x)
#define BLK_DATA_FREE(x)                      free(x)
#define BLK_DATA_ASSERT(expr)                 assert(expr)
#define BLK_DATA_HASH_CALLOC(x,s)             calloc(x,s)
#define BLK_DATA_HASH_MALLOC(x)               malloc(x)
#define BLK_DATA_HASH_FREE(x)                 free(x)
       
#define BLK_DSP_CALLOC(x,s)                   calloc(x,s)
#define BLK_DSP_MALLOC(x)                     malloc(x)
#define BLK_DSP_FREE(x)                       free(x)
#define BLK_DSP_ASSERT(expr)                  assert(expr)
       
#define BLK_GRAPH_CALLOC(x,s)                 calloc(x,s)
#define BLK_GRAPH_MALLOC(x)                   malloc(x)
#define BLK_GRAPH_FREE(x)                     free(x)
#define BLK_GRAPH_ASSERT(expr)                assert(expr)


#define RH_CFG_MALLOC_SIZE__NONE              ((1U)<<0)                     // Do NOT modify
#define RH_CFG_MALLOC_SIZE__64B               ((1U)<<6)                     // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__128B              ((1U)<<7)                     // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__256B              ((1U)<<8)                     // Do NOT modify  
#define RH_CFG_MALLOC_SIZE__512B              ((1U)<<9)                     // Do NOT modify  
#define RH_CFG_MALLOC_SIZE__1KB               ((1U)<<10)                    // Do NOT modify
#define RH_CFG_MALLOC_SIZE__2KB               ((1U)<<11)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__4KB               ((1U)<<12)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__8KB               ((1U)<<13)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__16KB              ((1U)<<14)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__32KB              ((1U)<<15)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__64KB              ((1U)<<16)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__128KB             ((1U)<<17)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__256KB             ((1U)<<18)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__512KB             ((1U)<<19)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__1MB               ((1U)<<20)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__2MB               ((1U)<<21)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__4MB               ((1U)<<22)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__8MB               ((1U)<<23)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__16MB              ((1U)<<24)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__32MB              ((1U)<<25)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__64MB              ((1U)<<26)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__128MB             ((1U)<<27)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__256MB             ((1U)<<28)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__512MB             ((1U)<<29)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE__1GB               ((1U)<<30)                    // Do NOT modify 
#define RH_CFG_MALLOC_SIZE                    RH_CFG_MALLOC_SIZE__NONE      // < select > < above option >

#define RH_CFG_HASHTABLE_SIZE__64B            ((1U)<<6)                     // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__128B           ((1U)<<7)                     // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__256B           ((1U)<<8)                     // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__512B           ((1U)<<9)                     // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__1KB            ((1U)<<10)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__2KB            ((1U)<<11)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__4KB            ((1U)<<12)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__8KB            ((1U)<<13)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__16KB           ((1U)<<14)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__32KB           ((1U)<<15)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__64KB           ((1U)<<16)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__128KB          ((1U)<<17)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__256KB          ((1U)<<18)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE__512KB          ((1U)<<19)                    // Do NOT modify
#define RH_CFG_HASHTABLE_SIZE                 RH_CFG_HASHTABLE_SIZE__64B    // < select > < above option >


#define RH_CFG_GRAPHIC_MALLOC_CACHE           (1U)                          // < select > < 0=disable : 1=enable >

#define RH_CFG_FONT_DATA_EXTERN_TTF           (0U)                          // Do NOT modify
#define RH_CFG_FONT_DATA_LOCAL_ARRAY          (1U)                          // Do NOT modify
#define RH_CFG_FONT_DATA_LOCAL_BITMAP         (2U)                          // Do NOT modify
#define RH_CFG_FONT_DATA_TYPE                 (0U)                          // < select > < above option >

#define RH_CFG_OUTPUT_FONT_PNG                (0U)                          // < select > < 0=disable : 1=enable >

#define RH_CFG_FONT_STYLE__CourierNew         (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__CourierNew_Italic  (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__CourierNew_Bold    (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__NewYork            (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__NewYork_Italic     (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__Unscii             (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__Optima             (1U)                          // < select > < 0=disable : 1=enable >
#define RH_CFG_FONT_STYLE__Sign_Printer       (1U)                          // < select > < 0=disable : 1=enable >


#ifdef __cplusplus
}
#endif

#endif

