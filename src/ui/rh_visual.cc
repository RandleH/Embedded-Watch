
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
#include "ui/rh_visual.hh"
#include "ui/rh_asset.h"
#include "rh_color.h"


/* Namespace -----------------------------------------------------------------*/
namespace rh{



/* Private macro -------------------------------------------------------------*/
#define CLOCKWHEEL_START_ANGLE      (270)
#define CLOCKWHEEL_END_ANGLE        (270+360)

#define COLOR_SECOND_DK             M_COLOR_DARKOLIVEGREEN
#define COLOR_SECOND_LT             M_COLOR_LAWNGREEN

#define COLOR_MINUTE_DK             M_COLOR_SIENNA
#define COLOR_MINUTE_LT             M_COLOR_ORANGE

#define COLOR_HOUR_DK               M_COLOR_INDIGO
#define COLOR_HOUR_LT               M_COLOR_VOILET



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
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
UIComponent::UIComponent( void){
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

int PictureComponent::increaseTick( u32 tick ){
    if( tick==0 ) return 0;
    tick %= this->ratioTick;
    
    this->tick += tick;
    if( this->tick > ratioTick ){
        std::swap( asset[0], asset[1] );
        this->tick -= ratioTick;
    }

    lv_img_set_src( obj, asset[0]);

    return 0;
}









/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
ClockWheelPictureComponent::ClockWheelPictureComponent( void * screen, const lv_img_dsc_t *day, const lv_img_dsc_t *night):
PictureComponent( screen), day_night(false){
    addImg(   day, true);
    addImg( night, false);
}

int ClockWheelPictureComponent::setDayNight( bool day_night){
    this->day_night = day_night;
    lv_img_set_src( obj, asset[(int)day_night]);
    return 0;
}


/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
ClockWheelComponent::ClockWheelComponent( void * screen, u16 diameter, u16 width):screen((lv_obj_t*)screen),angle(0){
    init( diameter, width);
}

int ClockWheelComponent::init( u16 diameter, u16 width){
    obj = lv_arc_create( screen);
    lv_obj_set_width ( obj, diameter);         
    lv_obj_set_height( obj, diameter);

    lv_obj_set_style_arc_width( obj, width, LV_PART_MAIN      | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width( obj, width, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_arc_set_bg_angles  ( obj,   0, 360);
    lv_arc_set_start_angle( obj, CLOCKWHEEL_START_ANGLE );
    lv_arc_set_end_angle  ( obj, CLOCKWHEEL_START_ANGLE );

    lv_obj_remove_style   ( obj, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag     ( obj, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center( obj);
    if(!obj) return 1; 
    return 0;
}

int ClockWheelComponent::setRatio( u32 tick_deg){
    ratioTick = tick_deg;
    return 0;
}

int ClockWheelComponent::setColor( u32 color1, u32 color2 ){
    color[0] = lv_color_hex(color1); color[1] = lv_color_hex(color2);
    lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
    lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    return 0;
}

int ClockWheelComponent::setColor( lv_color_t color1, lv_color_t color2 ){
    color[0] = color1; color[1] = color2;
    lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
    lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    return 0;
}

int ClockWheelComponent::increaseTick( u32 tick){
    if( tick==0 ) return 0;
    tick %= this->ratioTick;
    
    u16 deltaAngle = 0;
    
    deltaAngle = (this->tick+tick)/ratioTick;
    this->angle  += deltaAngle;
    this->tick   = (this->tick+tick)%ratioTick;
    
    if( this->angle >= 360 ){
        this->angle -= 360;

        std::swap( this->color[0], this->color[1] );
        lv_obj_set_style_arc_color( obj, color[0], LV_PART_MAIN      | LV_STATE_DEFAULT );
        lv_obj_set_style_arc_color( obj, color[1], LV_PART_INDICATOR | LV_STATE_DEFAULT );
    }
    
    if( deltaAngle!=0 ){
        lv_arc_set_end_angle( obj, CLOCKWHEEL_START_ANGLE+this->angle );
    }
        
    return 0;
}

int ClockWheelComponent::increase( void){
    return 1;
}

int ClockWheelComponent::setAngle( u16 deg){
    deg%=360;
    this->angle = deg;
    lv_arc_set_end_angle( obj, CLOCKWHEEL_START_ANGLE+deg );
    return 0;
}






/******************************************************************************/
/* This class will present a clock on screen in the manner of wheels          */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
ClockWheel::ClockWheel( void * screen):
ccSecond(screen, 240, 20), ccMinute(screen, 200, 20), ccHour(screen, 160, 20), ccDayIcon(screen, &ui_img_sun_png, &ui_img_moon_png),tick(0){
    
    lv_color_t color1, color2;
    color1.full = COLOR_HOUR_DK; color2.full = COLOR_HOUR_LT;
    ccHour.setColor( color1, color2);

    color1.full = COLOR_MINUTE_DK; color2.full = COLOR_MINUTE_LT;
    ccMinute.setColor( color1, color2);

    color1.full = COLOR_SECOND_DK; color2.full = COLOR_SECOND_LT;
    ccSecond.setColor( color1, color2);

    // ccHour.setColor( COLOR_HOUR_DK, COLOR_HOUR_LT);
    // ccMinute.setColor( COLOR_MINUTE_DK, COLOR_MINUTE_LT);
    // ccSecond.setColor( COLOR_SECOND_DK, COLOR_SECOND_LT);
    
    
    ccHour.setRatio  ( 4*60*12);
    ccMinute.setRatio( 4*60);
    ccSecond.setRatio( 4);  

    ccDayIcon.setRatio( 3600*12*1440 );
}

int ClockWheel::init( void){
    return 0;
}

int ClockWheel::increaseTick( u32 tick ){
    int retval = 0;
    
    retval |= ccHour.increaseTick(tick);
    retval |= ccMinute.increaseTick(tick);
    retval |= ccSecond.increaseTick(tick);

    return retval;
}

int ClockWheel::setTime( bool am_pm, u8 hour, u8 minute, u8 second ){
    
    ccHour.setAngle  ( hour   * 360 /12 );
    ccMinute.setAngle( minute * 360 /60 );
    ccSecond.setAngle( second * 360 /60 );

    // Only 06:30~18:00 will be recognized as Day
    if(   (    (am_pm==false)    &&    ( (hour>6) || ((hour==6)&&(minute>=30)) ) )\
        || \
          ((am_pm==true ) && (hour<6)) ){
        ccDayIcon.setDayNight( false);
    }else{
        ccDayIcon.setDayNight( true);
    }
    

    ccHour.resetTick();
    ccMinute.resetTick();
    ccSecond.resetTick();
    ccDayIcon.resetTick();
    return 0;
}




}
