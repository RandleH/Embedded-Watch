/**
  ******************************************************************************
  * @file    rh_app.hh
  * @author  RandleH
  * @brief   Main project application source code declaration.
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
#include <cstdint>
#include <cstddef>
#include <vector>
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include "rh_common.h"
#include "application/rh_visual.hh"


#include "application/widgets/clock880289/port.hh"
#include "application/widgets/clock524505/port.hh"

/* Name Space ----------------------------------------------------------------*/
namespace rh{







/* Exported types ------------------------------------------------------------*/
/******************************************************************************/
/* [453168] Bone class. All class must inherited from this                    */
/* @category:                                                                 */
/* @uniqueID:    453168                                                       */
/******************************************************************************/
class Bone{
protected:
    bool isInitialized;
public:
    Bone( void);
    int print( void);
    int reset( void);
    int init ( void);
};


/******************************************************************************/
/* [239023] This class handle program shared resource                         */
/* @category:    Resource                                                     */
/* @uniqueID:    239023                                                       */
/******************************************************************************/
class AppResource : public Bone{
public:
    volatile u32   userTick;             /*!< User Tick Signal 1KHz */
    volatile bool  UserTickOverflow;     /*!< User Tick Overflow Flag */
    
    volatile union{
        struct{
        u32   year      :12;             /*!< User Time - Year   */    
        u32   month     :4;              /*!< User Time - Month  */
        u32   date      :5;              /*!< User Time - Date   */
        u32   hour      :5;              /*!< User Time - Hour   */
        u32   minute    :6;              /*!< User Time - Minute */
        u32   second    :6;              /*!< User Time - Second */
        }bit;
        u64 _;
    }time;


    volatile bool  key0;

public:
    AppResource( void);

};




/******************************************************************************/
/* [975338] This class handle system task distribution                        */
/* @category:    Thread Control                                               */
/* @uniqueID:    975338                                                       */
/******************************************************************************/
class AppThread:public Bone{
private:              
    std::pair< EventGroupHandle_t, StaticEventGroup_t> ev_device;     /*!< Device Event Group            */
    std::pair< EventGroupHandle_t, StaticEventGroup_t> ev_network;    /*!< Network Event Group           */

    

    std::pair<TaskHandle_t,StaticTask_t>       htcb_483377;       /*!< Screen Refreash */
    StackType_t                                stak_483377[1024];


    std::pair<TaskHandle_t,StaticTask_t>       htcb_526892;       /*!< Heart Beat LED  */
    StackType_t                                stak_526892[1024];

    std::pair<TaskHandle_t,StaticTask_t>       htcb_000000;       /*!< Idle Task */
    StackType_t                                stak_000000[1024];

    std::pair<TaskHandle_t,StaticTask_t>       htcb_000001;       /*!< Timer Systick Task */
    StackType_t                                stak_000001[1024];

    std::vector<TaskHandle_t>                  dytcb;     /*!< Dynamic Task Control Block    */
    std::vector<std::vector<StackType_t>>      dystack;   /*!< Dynamic Task Stack            */

    int addDeviceInit( void);       
    int addGUIInit( void);
    int addMainService( void);
public:
    AppThread( void);
    void getIdleTaskMemory ( StaticTask_t ** , StackType_t  ** , uint32_t * );
    void getTimerTaskMemory( StaticTask_t ** , StackType_t  ** , uint32_t * );
    
    int init( void);
};





/******************************************************************************/
/* [914842] This class contains display object information of lvgl            */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
class RH_API Display{
private:
    lv_color_t      gram1[RH_CFG_GRAM_SIZE];
    lv_color_t      gram2[RH_CFG_GRAM_SIZE];    
public:
    lv_disp_t       *display; 
    lv_disp_drv_t   driver;
    lv_disp_draw_buf_t buf;
    Display( void);
};


/******************************************************************************/
/* [914842] This class handle graphic user interface                          */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
class AppGUI:public Bone{
public:
    Display       lvgl;                         /*!< Display information for LVGL library */
    lv_obj_t     *screen;                       /*!< Default screen object                */
    
    rh::widget880289::Widget      uiClockWheel;         /*!< UI Widget - Clock Wheel              */
    rh::widget524505::Widget      uiClockClassic;       /*!< UI Widget - Clock Classic            */

public:
    AppGUI( void);
    int load( void);

};



/******************************************************************************/
/* [473317] Project Application Main Class                                    */
/* @category:                                                                 */
/* @uniqueID:    473317                                                       */
/******************************************************************************/
class Application{
private:
public:
    Application( void);
    AppResource  resource;      /*!< Global Shared Resource */
    AppThread    thread;        /*!< Thread Management      */
    AppGUI       gui;           /*!< Graphic UI Management  */
    
    void start( void);

};





}

extern rh::Application app;
/************************ (C) COPYRIGHT RandleH *****END OF FILE***************/