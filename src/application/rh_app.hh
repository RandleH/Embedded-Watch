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
/* [239023] This class handle low level embedded system program               */
/* @category:    System                                                       */
/* @uniqueID:    239023                                                       */
/******************************************************************************/
class AppSystem : public Bone{
private:

public:
    AppSystem( void);


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

    std::vector<TaskHandle_t>                  tcb;       /*!< Static Task Control Block     */
    std::vector<std::vector<StackType_t>>      stack;     /*!< Static Task Stack             */
    std::vector<TaskHandle_t>                  dytcb;     /*!< Dynamic Task Control Block    */
    std::vector<std::vector<StackType_t>>      dystack;   /*!< Dynamic Task Stack            */

public:
    AppThread( void);
};





/******************************************************************************/
/* [914842] This class handle graphic user interface                          */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
class AppGUI:public Bone{
private:
public:
    AppGUI( void);

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

    AppThread  thread;
    AppGUI     gui;
    AppSystem  system;
    

};



}


/************************ (C) COPYRIGHT RandleH *****END OF FILE***************/