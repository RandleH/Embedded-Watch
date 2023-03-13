/**
  ******************************************************************************
  * @file    port.cc
  * @author  RandleH
  * @brief   Clock widget style: Louis Vuitton.
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
#include "rh_color.h"
#include "rh_config.h"
#include "application/rh_math.hh"
#include "lvgl.h"
#include "./port.hh"
#include "./asset.h"


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget454570{


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
ClockPanelComponent::ClockPanelComponent( void* screen, u32 hex_c1, u32 hex_c2){

    /* Screen Background Setting */
    lv_obj_clear_flag( (lv_obj_t*)screen, LV_OBJ_FLAG_SCROLLABLE );    
    lv_obj_set_style_bg_color( (lv_obj_t*)screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa( (lv_obj_t*)screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    
    /* Initialize 12 LV icon around clock panel */
    for( size_t i=0; i<12; ++i){
        _obj_icon[i] = lv_img_create( (lv_obj_t*)screen);
        lv_obj_set_width  ( _obj_icon[i], LV_SIZE_CONTENT);
        lv_obj_set_height ( _obj_icon[i], LV_SIZE_CONTENT);
        lv_img_set_zoom   ( _obj_icon[i], 192);
        lv_obj_set_style_img_recolor_opa( _obj_icon[i], 0xff, 0); 
        lv_obj_clear_flag ( _obj_icon[i], LV_OBJ_FLAG_SCROLLABLE );
        if( i!=(3-1) && i!=(6-1) && i!=(9-1) && i!=(12-1) ){
            lv_obj_set_align( _obj_icon[i], LV_ALIGN_CENTER );
        }
    }

    /* Add image icon source */    
    lv_img_set_src( _obj_icon[6-1] , &ui_img_1375507867);

    lv_img_set_src( _obj_icon[3-1] , &ui_img_1625705288);
    lv_img_set_src( _obj_icon[9-1] , &ui_img_1625705288);
    lv_img_set_src( _obj_icon[12-1], &ui_img_1625705288);
    
    lv_img_set_src( _obj_icon[2-1] , &ui_img_1181490689);
    lv_img_set_src( _obj_icon[5-1] , &ui_img_1181490689);
    lv_img_set_src( _obj_icon[8-1] , &ui_img_1181490689);
    lv_img_set_src( _obj_icon[11-1], &ui_img_1181490689);

    lv_img_set_src( _obj_icon[1-1] , &ui_img_1682387597);
    lv_img_set_src( _obj_icon[4-1] , &ui_img_1682387597);
    lv_img_set_src( _obj_icon[7-1] , &ui_img_1682387597);
    lv_img_set_src( _obj_icon[10-1], &ui_img_1682387597);

    
    /* Icon setting at 3 o'clock */
    lv_obj_set_align( _obj_icon[3-1], LV_ALIGN_RIGHT_MID );
    lv_obj_set_x( _obj_icon[3-1], lv_pct(-3) ); lv_obj_set_y( _obj_icon[3-1], lv_pct(0) );

    /* Icon setting at 6 o'clock */
    lv_obj_set_align( _obj_icon[6-1], LV_ALIGN_BOTTOM_MID );
    lv_obj_set_x( _obj_icon[6-1], lv_pct(0) );  lv_obj_set_y( _obj_icon[6-1], lv_pct(-3) );

    /* Icon setting at 9 o'clock */
    lv_obj_set_align( _obj_icon[9-1], LV_ALIGN_LEFT_MID );
    lv_obj_set_x( _obj_icon[9-1], lv_pct(3) );  lv_obj_set_y( _obj_icon[9-1], lv_pct(0) );
    
    /* Icon setting at 12 o'clock */
    lv_obj_set_align( _obj_icon[12-1], LV_ALIGN_TOP_MID );
    lv_obj_set_x( _obj_icon[12-1], lv_pct(0) ); lv_obj_set_y( _obj_icon[12-1], lv_pct(3) );

    const u16 absOffset_x=50, absOffset_y=82;

    /* Icon setting at {1,2,4,5,7,8,10,11} o'clock */
    lv_obj_set_x( _obj_icon[ 1-1],  absOffset_x ); lv_obj_set_y( _obj_icon[ 1-1], -absOffset_y );
    lv_obj_set_x( _obj_icon[ 2-1],  absOffset_y ); lv_obj_set_y( _obj_icon[ 2-1], -absOffset_x );
    lv_obj_set_x( _obj_icon[ 4-1],  absOffset_x ); lv_obj_set_y( _obj_icon[ 4-1],  absOffset_y );
    lv_obj_set_x( _obj_icon[ 5-1],  absOffset_y ); lv_obj_set_y( _obj_icon[ 5-1],  absOffset_x );
    lv_obj_set_x( _obj_icon[ 7-1], -absOffset_x ); lv_obj_set_y( _obj_icon[ 7-1],  absOffset_y );
    lv_obj_set_x( _obj_icon[ 8-1], -absOffset_y ); lv_obj_set_y( _obj_icon[ 8-1],  absOffset_x );
    lv_obj_set_x( _obj_icon[10-1], -absOffset_y ); lv_obj_set_y( _obj_icon[10-1], -absOffset_x );
    lv_obj_set_x( _obj_icon[11-1], -absOffset_x ); lv_obj_set_y( _obj_icon[11-1], -absOffset_y );
    
    /* Color Gradient */
    colorGradient( hex_c1, hex_c2);
}

void ClockPanelComponent::colorGradient( u32 hex_c1, u32 hex_c2){
    lv_color32_t *p1=(lv_color32_t*)&hex_c1, *p2=(lv_color32_t*)&hex_c2;
    lv_color32_t color = {.full = hex_c1};
    const u8 idxTable[] = {
        1,   3,
        2,   2, 4,
        2,   1, 5,
        2,   6,12,
        2,  11, 7,
        2,  10, 8,
        1,   9
    };
    const u8 *pIdx = idxTable;
    const u8 step_r = LV_ABS(p2->ch.red   - p1->ch.red  ) / 7;
    const u8 step_g = LV_ABS(p2->ch.green - p1->ch.green) / 7;
    const u8 step_b = LV_ABS(p2->ch.blue  - p1->ch.blue ) / 7;
    const u8 mod_r  = LV_ABS(p2->ch.red   - p1->ch.red  ) % 7;
    const u8 mod_g  = LV_ABS(p2->ch.green - p1->ch.green) % 7;
    const u8 mod_b  = LV_ABS(p2->ch.blue  - p1->ch.blue ) % 7;
    const u8 dir    = (p2->ch.red>p1->ch.red)<<2 | (p2->ch.green>p1->ch.green)<<1 | (p2->ch.blue>p1->ch.blue)<<0;

    u8 i=0;
    do{
        u8 cnt = *pIdx++;
        while( cnt-- ){
            lv_obj_set_style_img_recolor( _obj_icon[(*pIdx++) -1], lv_color_hex(color.full), 0);
        }

        (dir&(1<<2)) ? color.ch.red   += (step_r + (i<mod_r) ) : color.ch.red   -= (step_r + (i<mod_r) );
        (dir&(1<<1)) ? color.ch.green += (step_g + (i<mod_g) ) : color.ch.green -= (step_g + (i<mod_g) );
        (dir&(1<<0)) ? color.ch.blue  += (step_b + (i<mod_b) ) : color.ch.blue  -= (step_b + (i<mod_b) );

        ++i;
    }while(i<7);

}




/******************************************************************************/
/* Private UI Component                                                       */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
ClockNeedleComponent::ClockNeedleComponent( void* screen, u32 ratio, u32 hexColor, const void* imgSrc, i16 pivotX, i16 pivotY, i16 posX, i16 posY):
_ratio(ratio),_color( lv_color_hex(hexColor)){
    _obj = lv_img_create( (lv_obj_t*)screen);
    lv_img_set_src( _obj, imgSrc);
    lv_obj_set_width( _obj, LV_SIZE_CONTENT);  /// 63
    lv_obj_set_height( _obj, LV_SIZE_CONTENT);   /// 16
    lv_obj_set_x( _obj, posX );
    lv_obj_set_y( _obj, posY );
    lv_obj_set_align( _obj, LV_ALIGN_LEFT_MID );
    lv_obj_add_flag( _obj, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( _obj, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_img_set_pivot(_obj, pivotX, pivotY);
    
    lv_obj_set_style_img_recolor_opa( _obj, 0xff, 0); 
    lv_obj_set_style_img_recolor( _obj, lv_color_hex( hexColor), 0);
}

#include "rh_debug.h"
void  ClockNeedleComponent::run( u32 tick, int OPT_XXXX ){

    static i16 angle_old = 0;
    static bool flag = false;

    switch( OPT_XXXX){
        case OPT_SET:{
            this->_tick = tick;
            break;
        }
        case OPT_FORWARD:{
            this->_tick += tick;
            break;
        }
        default:{
            break;
        }
    }

    if( this->_tick >= _ratio ){
        this->_tick %= _ratio;
    }

    

    i16 angle;

    switch( _ratio ){ // Some of the common ratio cam be optimized
        case 60*1000:{
            angle = ((_tick<<1)+_tick)/3;
            break;
        }
        case 60*1000*60:{
            angle = _tick/1000;
            break;
        }
        case 60*1000*60*12:{
            angle = _tick/12000;
            break;
        }
        
        default:{  // Overflow may occur
            angle = this->_tick * 3600/ _ratio;
            break;
        }
    }


    if( !flag){
        rh_debug__printf( "Initial angle = %d\n", angle);
        rh_debug__printf( "Initial local tick = %d\n", _tick);
        rh_debug__printf( "Initial tick = %d\n", tick);
        flag = true;
    }


    if( angle_old != angle){
        lv_img_set_angle(_obj, angle-900);
        angle_old = angle;
    }

}

u32   ClockNeedleComponent::ratio( void) const    { return _ratio;}
void  ClockNeedleComponent::ratio( u32 val)       { _ratio = val; }


lv_color_t ClockNeedleComponent::color( void) const  { return _color;}
void       ClockNeedleComponent::color( u32 hexColor){
    _color = lv_color_hex(hexColor);
    lv_obj_set_style_img_recolor( _obj, _color, 0);
}








/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
Widget::Widget( void* screen):
rh::ClockWidget{screen}

#if WIDGET_SHOW_CLOCK_PANEL
    ,ccPanel{         /* INIT LIST FOR Clock Panel */
        screen,       /*!< LVGL screen source pointer */ 
        0x00921b7b,   /*!< Initial leftmost color of LV icon */
        0x002e899c    /*!< Initial rightmost color of LV icon */
    }
#endif

,ccPanel2{
    screen
}

#if WIDGET_SHOW_NEEDLE_SECOND
    ,ccNeedleSecond{        /* INIT LIST FOR Clock Needle */
        screen,             /*!< LVGL screen source pointer */ 
        60*1000,            /*!< Tick count that takes the needle turn around assumming 1kHz tick rate */
        0x00ffa70e,         /*!< Initial needle color */
        &ui_img_1832269932, /*!< Needle style image source (LVGL iamge) */
        8,                  /*!< Rotation pivot X */
        8,                  /*!< Rotation pivot Y */
        113,                /*!< X Position of needle assumming align left */
        0                   /*!< Y Position of needle assumming align middle */
    }
#endif

#if WIDGET_SHOW_NEEDLE_MINUTE
    ,ccNeedleMinute{        /* INIT LIST FOR Clock Needle */
        screen,             /*!< LVGL screen source pointer */ 
        60*1000*60,         /*!< Tick count that takes the needle turn around assumming 1kHz tick rate */
        0x00ffa70e,         /*!< Initial needle color */
        &ui_img_1832269932, /*!< Needle style image source (LVGL iamge) */
        8,                  /*!< Rotation pivot X */
        8,                  /*!< Rotation pivot Y */
        113,                /*!< X Position of needle assumming align left */
        0                   /*!< Y Position of needle assumming align middle */
    }
#endif

#if WIDGET_SHOW_NEEDLE_HOUR
    ,ccNeedleHour{          /* INIT LIST FOR Clock Needle */
        screen,             /*!< LVGL screen source pointer */ 
        60*1000*60*12,      /*!< Tick count that takes the needle turn around assumming 1kHz tick rate */
        0x00cd860a,         /*!< Initial needle color */
        &ui_img_2070302622, /*!< Needle style image source (LVGL iamge) */
        8,                  /*!< Rotation pivot X */
        8,                  /*!< Rotation pivot Y */
        113,                /*!< X Position of needle assumming align left */
        0                   /*!< Y Position of needle assumming align middle */
    }
#endif
{   /* Do nothing at function body */  }



int Widget::setTime( u8 hh, u8 mm, u8 ss ){
    if( hh>=24 || mm>=60 || ss>=60 ) return 1;

    hh %= 12;
    u32 ratio;

#if WIDGET_SHOW_NEEDLE_SECOND
    {
        ratio = ccNeedleSecond.ratio();
        ccNeedleSecond.run( ss*ratio/60, OPT_SET);
    }
#endif

#if WIDGET_SHOW_NEEDLE_HOUR    
    {
        ratio = ccNeedleHour.ratio();
        ccNeedleHour.run( hh*ratio/12 + mm*ratio/(60*12) + ss*ratio/(60*60*12), OPT_SET);
    }
#endif

#if WIDGET_SHOW_NEEDLE_MINUTE
    {
        ratio = ccNeedleMinute.ratio();
        ccNeedleMinute.run( mm*ratio/60 + ss*ratio/(60*60), OPT_SET);
    }
#endif        
    return done();
}

int Widget::setDayNight( bool day_night){return -1;}


/**
 * @class rh::widget524505::Widget
 * @brief 
*/
int Widget::update( void){
#if WIDGET_SHOW_NEEDLE_SECOND
    ccNeedleSecond.run( this->tickInc, OPT_FORWARD);
#endif

#if WIDGET_SHOW_NEEDLE_MINUTE    
    ccNeedleMinute.run( this->tickInc, OPT_FORWARD);
#endif

#if WIDGET_SHOW_NEEDLE_HOUR
    ccNeedleHour.run( this->tickInc, OPT_FORWARD);
#endif    
    return done();
}


}
}


