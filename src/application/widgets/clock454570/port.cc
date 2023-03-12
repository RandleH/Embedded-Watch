/**
  ******************************************************************************
  * @file    port.cc
  * @author  RandleH
  * @brief   This file contains source code for ui widget component 454570.         
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
#include "./asset/asset.h"


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget454570{


/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
ClockPanelComponent::ClockPanelComponent( void* screen){

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
    
    /* Default Color Gradient */
    colorGradient( 0x00921b7b, 0x002e899c);
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
ClockNeedleComponent::ClockNeedleComponent( void* screen, u8 len, u8 offset, lv_color_t color):
_len(len),
_offset(offset),
_point{{120,(lv_coord_t)(120+offset)},{120,(lv_coord_t)(120-len)}}{
    lv_style_init(&_style);
    lv_style_set_line_width(&_style, 20);
    lv_style_set_line_color(&_style, color);
    lv_style_set_line_rounded(&_style, false);
    

    _obj = lv_line_create( (lv_obj_t*)screen);
    lv_line_set_points( _obj, _point, 2);     /*Set the points*/
    lv_obj_add_style( _obj, &_style, 0);
}

/**
 * @class rh::widget524505::updateAngle
 * @param (none)
*/
void ClockNeedleComponent::updateAngle( void){
    
    static u64 angle_old = 0;
    u64 angle = ((((u64)this->_tick)<<10)/_ratio);    // 从系统滴答映射成余弦表格后的角度, 范围[0:1023]

    if( angle_old==angle )
        return;

    switch( angle>>8 ){
        case 0:{  // [0:90]
            _point[1].x = 120 +  ((_len*rh::math::TB_SINE[     angle])>>8);         // x = L*sin(a)
            _point[1].y = 120 + -((_len*rh::math::TB_SINE[0xff-angle])>>8);         // y = L*cos(a) = -L*sin(pi/2-a)

            _point[0].x = 120 + -((_offset*rh::math::TB_SINE[     angle])>>8);    
            _point[0].y = 120 +  ((_offset*rh::math::TB_SINE[0xff-angle])>>8);    
            break;
        }
            
        case 1:{  // [90:180]
            angle -= 256;  // ASSERT( angle<256 && angle>=0 )
            _point[1].x = 120 - -((_len*rh::math::TB_SINE[0xff-angle])>>8);         // x = L*sin(a+pi/2) = -L*sin(pi/2-a)
            _point[1].y = 120 - -((_len*rh::math::TB_SINE[     angle])>>8);         // y = L*cos(a+pi/2) = -L*sin(a)
            _point[0].x = 120 + -((_offset*rh::math::TB_SINE[0xff-angle])>>8);
            _point[0].y = 120 + -((_offset*rh::math::TB_SINE[     angle])>>8);
            break;
        }
        case 2:{  // [180:270]
            angle -= 512;
            _point[1].x = 120 + -((_len*rh::math::TB_SINE[      angle])>>8);        // x = L*sin(a+pi) =  -L*sin(a)
            _point[1].y = 120 +  ((_len*rh::math::TB_SINE[ 0xff-angle])>>8);        // y = L*cos(a+pi) =   L*sin(pi/2-a)
            _point[0].x = 120 - -((_offset*rh::math::TB_SINE[      angle])>>8);
            _point[0].y = 120 -  ((_offset*rh::math::TB_SINE[ 0xff-angle])>>8);
            break;
        }
        case 3:{  // [270:360]
            angle -= 768;
            _point[1].x = 120 - ((_len*rh::math::TB_SINE[ 0xff-angle])>>8);         // x = L*sin(a+3pi/2) = L*sin(pi/2-a)
            _point[1].y = 120 - ((_len*rh::math::TB_SINE[      angle])>>8);         // x = L*cos(a+3pi/2) =  L*sin(a)
            _point[0].x = 120 + ((_offset*rh::math::TB_SINE[ 0xff-angle])>>8);    
            _point[0].y = 120 + ((_offset*rh::math::TB_SINE[      angle])>>8);
            break;
        }
        default:{
            while(1); // Something wrong
        }
    }

    
    lv_line_set_points( _obj, _point, 2);     /*Set the points*/

    angle_old = angle;
}

void  ClockNeedleComponent::run( u32 tick, int OPT_XXXX ){
    switch( OPT_XXXX){
        case OPT_FORWARD:{   // Move the needle in the cw direction
            this->_tick += tick;
            if( this->_tick >= _ratio ){
                this->_tick %= _ratio;
            }
            break;
        }
        case OPT_SET:{       // Set the needle in a absolute position
            if( tick >= _ratio ){
                tick %= _ratio;
            }
            this->_tick = tick;
            break;
        }
        case OPT_RESET:{      // Reset the needle to the 12'o clock position
            this->_tick = 0;
            break;
        }
        case OPT_BACKWARD:{  // Move the needle in the ccw direction 
            this->_tick -= tick;
            if( this->_tick >= _ratio ){
                this->_tick %= _ratio;
            }
            break;
        }
        default:{            // No operation
            break;
        }

    }

    updateAngle();
}

void ClockNeedleComponent::hide( bool cmd){
    if( cmd==true ){
        lv_obj_add_flag( _obj, LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag( _obj, LV_OBJ_FLAG_HIDDEN);
    }
}

u32   ClockNeedleComponent::ratio( void)     { return _ratio;}
void  ClockNeedleComponent::ratio( u32 val)  { _ratio = val; }

u8    ClockNeedleComponent::length( void)    { return _len; }
void  ClockNeedleComponent::length( u8 val)  { _len = val;  }

u8    ClockNeedleComponent::width( void)     { return (u8)lv_obj_get_style_line_width( _obj, LV_PART_MAIN);}
void  ClockNeedleComponent::width( u8 val)   { lv_obj_set_style_line_width( _obj, val, LV_PART_MAIN);      }

u8    ClockNeedleComponent::offset( void)    { return _offset; }
void  ClockNeedleComponent::offset( u8 val)  { _offset = val;  }














/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
Widget::Widget( void* screen):
rh::ClockWidget{screen},
ccPanel{screen},
ccNeedleMinute{screen, 80, 20, lv_color_hex(0x00fbd300)},
ccNeedleHour  {screen, 50, 20, lv_color_hex(0x00fb0000)}{

    // ccNeedleSecond.ratio( 60000);
    ccNeedleHour.ratio( 60000*60 );
    ccNeedleMinute.ratio( 60000*60*12 );
}

int Widget::setTime( u8 hh, u8 mm, u8 ss ){
    if( hh>=24 || mm>=60 || ss>=60 ) return 1;

    hh %= 12;
    u32 ratio;
    {
        ratio = ccNeedleHour.ratio();
        ccNeedleHour.run( hh*ratio/12 + mm*ratio/(60*12) + ss*ratio/(60*60*12), OPT_SET);
    }

    {
        ratio = ccNeedleMinute.ratio();
        ccNeedleMinute.run( mm*ratio/60 + ss*ratio/(60*60), OPT_SET);
    }
    
    {
        // ratio = ccNeedleSecond.ratio();
        // ccNeedleSecond.run( ss*ratio/60, OPT_SET);
    }
    
    return done();
}

int Widget::setDayNight( bool day_night){return -1;}


/**
 * @class rh::widget524505::Widget
 * @brief 
*/
int Widget::update( void){
    // ccNeedleSecond.run( this->tickInc, OPT_FORWARD);
    ccNeedleMinute.run( this->tickInc, OPT_FORWARD);
    ccNeedleHour.run( this->tickInc, OPT_FORWARD);
    return done();
}


}
}


