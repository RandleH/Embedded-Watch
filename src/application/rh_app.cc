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
#include "rh_config.h"
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
}
#endif





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
/* [914842] This class contains display object information of lvgl            */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
extern"C"{
    void rhlv_screen__flush_cb(struct _lv_disp_drv_t *, const lv_area_t *, lv_color_t *);
}
Display::Display( void):
gram1{0},gram2{0}{
    lv_init();
    __asm("nop");
    lv_disp_drv_init( &this->driver );
    this->driver.draw_buf = &this->buf;
    this->driver.flush_cb = rhlv_screen__flush_cb;
    this->driver.hor_res = RH_CFG_SCREEN_HEIGHT;
    this->driver.ver_res = RH_CFG_SCREEN_WIDTH;
    lv_disp_draw_buf_init( &this->buf, this->gram1, this->gram2, RH_CFG_GRAM_SIZE);
    
    this->display = lv_disp_drv_register( &this->driver);
}




/******************************************************************************/
/* [914842] This class handle graphic user interface                          */
/* @category:    User Interface                                               */
/* @uniqueID:    914842                                                       */
/******************************************************************************/
AppGUI::AppGUI( void):
lvgl(),screen(lv_obj_create(NULL)), uiClockWheel(lv_obj_create(NULL)), uiClockClassic(lv_obj_create(NULL)){

    lv_obj_set_style_bg_color ( screen, lv_color_hex(0x191919), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa   ( screen,                    255, LV_PART_MAIN | LV_STATE_DEFAULT );

    uiClockWheel.setTime( app.resource.time.bit.hour, app.resource.time.bit.minute, app.resource.time.bit.second );

}

int AppGUI::load( void){
    lv_scr_load( screen);
    return 0;
}









/******************************************************************************/
/* [473317] Project Application Main Class                                    */
/* @category:                                                                 */
/* @uniqueID:    473317                                                       */
/******************************************************************************/
Application::Application( void){

}

void Application::start( void){
    vTaskStartScheduler();
}














}//////// END OF NAMESPACE /////////

/************************ (C) COPYRIGHT RandleH *****END OF FILE***************/