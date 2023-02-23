/**
  ******************************************************************************
  * @file    rh_app.cc
  * @author  RandleH
  * @brief   Main project application source code defination.
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
#include "rh_app.hh"



/* Name Space ----------------------------------------------------------------*/
namespace rh{





/******************************************************************************/
/* Bone class. All class must inherited from this                             */
/* @category:                                                                 */
/******************************************************************************/
Bone::Bone( void):
isInitialized(false){

}

    
int Bone::print( void){
    return 0;
}

int Bone::reset( void){
    return 0;
}

int Bone::init ( void){
    return 0;
}



/******************************************************************************/
/* [239023] This class handle low level embedded system program               */
/* @category:    System                                                       */
/* @uniqueID:    239023                                                       */
/******************************************************************************/
AppSystem::AppSystem( void){

}









/******************************************************************************/
/* [975338] This class handle system task distribution                        */
/* @category:    Thread Control                                               */
/* @uniqueID:    975338                                                       */
/******************************************************************************/
AppThread::AppThread( void){
    
}












/******************************************************************************/
/* [914842] This class handle graphic user interface                          */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
AppGUI::AppGUI( void){

}











/******************************************************************************/
/* [473317] Project Application Main Class                                    */
/* @category:                                                                 */
/* @uniqueID:    473317                                                       */
/******************************************************************************/
Application::Application( void){

}
















}//////// END OF NAMESPACE /////////

/************************ (C) COPYRIGHT RandleH *****END OF FILE***************/