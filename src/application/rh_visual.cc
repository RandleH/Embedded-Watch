
/**
  ******************************************************************************
  * @file    rh_visual.hh
  * @author  RandleH
  * @brief   This file contains source code for visual control program.
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
#include <cstddef>
#include <utility>
#include "application/rh_visual.hh"
#include "application/rh_asset.h"
#include "rh_color.h"


/* Namespace -----------------------------------------------------------------*/
namespace rh{



/* Private macro -------------------------------------------------------------*/
#define CLOCKWHEEL_START_ANGLE      (270)
#define CLOCKWHEEL_END_ANGLE        (270+360)

#define COLOR_SECOND_DK             RH_COLOR_MAKE( 126,  48,  93)
#define COLOR_SECOND_LT             RH_COLOR_MAKE( 237,  29, 113)

#define COLOR_MINUTE_DK             RH_COLOR_MAKE(  61, 129,  38)
#define COLOR_MINUTE_LT             RH_COLOR_MAKE(  88, 238,  36)

#define COLOR_HOUR_DK               RH_COLOR_MAKE(  49,  53, 114)
#define COLOR_HOUR_LT               RH_COLOR_MAKE(  70,  78, 226)



/* Private functions ---------------------------------------------------------*/
Visual::Visual():screen((size_t)(NUM_VISUAL_SCREEN)){
    for( auto &ptr : screen ){
        ptr = lv_obj_create(NULL);
    }
    
    lv_obj_set_style_bg_color ( screen[VISUAL_SCREEN__MAIN], lv_color_hex(0x090909), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa   ( screen[VISUAL_SCREEN__MAIN],                    255, LV_PART_MAIN | LV_STATE_DEFAULT );
}

lv_obj_t* Visual::getScreen( E_VisualScreen_t scr ){
    return this->screen[scr];
}

int Visual::load( E_VisualScreen_t scr ){
    if( !this->screen[scr] ) return 1;
    lv_scr_load( this->screen[scr] );
    return 0;
}



/******************************************************************************/
/* This class is a template class. All widget should inherit from this.       */
/* @category:    User Interface                                               */
/******************************************************************************/
Widget::Widget( void* screen):tick(0){
    /* Do nothing */
}

int Widget::increaseTick( u32 tick){
    this->tick += tick;
    return 0;
}


/******************************************************************************/
/* This class is a template class for clock widget. All clock widgets should  */
/* inherit from this                                                          */
/* @category:    User Interface                                               */
/******************************************************************************/
ClockWidget::ClockWidget( void* screen):Widget{screen},am_pm(true){
    
}

int ClockWidget::setTime( bool am_pm, u8 hour, u8 minute, u8 second ){
    this->am_pm = am_pm;
    return 1;
}

int ClockWidget::setDayNight( bool day_night){
    return 1;
}




/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
UIComponent::UIComponent( void)
:tick(0),ratioTick(1000){
    /* Do nothing */
}

UIComponent::UIComponent( void* screen):tick(0),ratioTick(1000){
    obj = lv_obj_create( (lv_obj_t*)screen);
}

int UIComponent::resetTick( void){
    this->tick = 0;
    return 0;
}



/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
PictureComponent::PictureComponent( void * screen, const lv_img_dsc_t *lv_img[], size_t nItems){
    obj = lv_img_create(((lv_obj_t*)screen));
    
    for( size_t i=0; i<nItems; ++i ){
        asset.push_back( lv_img[i]);
    }

    lv_img_set_src(obj, asset[0]);
    lv_obj_set_width( obj, LV_SIZE_CONTENT);    
    lv_obj_set_height( obj, LV_SIZE_CONTENT);  
    lv_obj_set_align( obj, LV_ALIGN_CENTER );
    lv_obj_add_flag( obj, LV_OBJ_FLAG_ADV_HITTEST );   
    lv_obj_clear_flag( obj, LV_OBJ_FLAG_SCROLLABLE );   
}

PictureComponent::PictureComponent( void * screen ){
    obj = lv_img_create(((lv_obj_t*)screen));
    lv_img_set_src(obj, NULL);
}

int PictureComponent::setRatio( u32 tick){
    ratioTick = tick;
    return 0;
}

int PictureComponent::addImg( const lv_img_dsc_t *lv_img[], size_t nItems){
    for( size_t i=0; i<nItems; ++i ){
        asset.push_back( lv_img[i]);
    }

    if( NULL==lv_img_get_src( obj) ){
        lv_img_set_src(obj, asset[0]);
        lv_obj_set_width( obj, LV_SIZE_CONTENT);   
        lv_obj_set_height( obj, LV_SIZE_CONTENT);  
        lv_obj_set_align( obj, LV_ALIGN_CENTER );
        lv_obj_add_flag( obj, LV_OBJ_FLAG_ADV_HITTEST );   
        lv_obj_clear_flag( obj, LV_OBJ_FLAG_SCROLLABLE );    
    }

    return 0;
}

int PictureComponent::addImg( const lv_img_dsc_t *lv_img, bool immediate){

    asset.push_back( lv_img);

    if( NULL==lv_img_get_src( obj) ){
        lv_img_set_src(obj, asset[0]);
        lv_obj_set_width( obj, LV_SIZE_CONTENT);   
        lv_obj_set_height( obj, LV_SIZE_CONTENT);  
        lv_obj_set_align( obj, LV_ALIGN_CENTER );
        lv_obj_add_flag( obj, LV_OBJ_FLAG_ADV_HITTEST );   
        lv_obj_clear_flag( obj, LV_OBJ_FLAG_SCROLLABLE );    
    }else if( immediate){
        lv_img_set_src(obj, asset.back());
    }

    return 0;
}



}
