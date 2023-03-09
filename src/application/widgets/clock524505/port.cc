/**
  ******************************************************************************
  * @file    port.cc
  * @author  RandleH
  * @brief   This file contains source code for ui widget component 524505.         
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
#include <cmath>

#include "rh_debug.h"
#include "rh_color.h"
#include "./application/rh_app.hh"
#include "lvgl.h"

#include <misc/lv_math.h>          /* Directory path = " /lvgl/src/misc/lv_math.h" */


#include "./port.hh"

#include "./gui_guider.h"


#include "./lv_analogclock.h"

/* Private define ------------------------------------------------------------*/
#define COLOR_MINUTE_TICK(cmd)           (cmd==true)?lv_color_hex(0x006b6b6b) : lv_color_hex(0x006b6b6b) 
#define COLOR_HOUR_TICK(cmd)             (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 

#define LEN_MINUTE_TICK                  (15)
#define LEN_HOUR_TICK                    (20)

#define WIDTH_MINUTE_NEEDLE              (3)
#define WIDTH_HOUR_NEEDLE                (3)

#define COLOR_MINUTE_NEEDLE(cmd)         (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 
#define COLOR_HOUR_NEEDLE(cmd)           (cmd==true)?lv_color_hex(0x00ffffff) : lv_color_hex(0x00ffffff) 
#define COLOR_SECOND_NEEDLE(cmd)         (cmd==true)?lv_color_hex(0x00ff0029) : lv_color_hex(0x00ff0029) 


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget524505{



/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
/**
 * @class rh::widget524505::ClockNeedleComponent
 * @param  screen  - LVGL screen object pointer
 * @param  len     - Length of the needle tick
 * @param  width   - Width of the needle tick
 * @param  rounded - 1=Round corner for needle
 * @param  color   - Color of needle
 * @subsection rh::UIComponent
*/
ClockNeedleComponent::ClockNeedleComponent( void* screen, u8 len, u8 offset, u8 width, bool rounded, lv_color_t color):
_len(len),
_offset(offset),
_point{{120,(lv_coord_t)(120+offset)}, {120,(lv_coord_t)(120-len)}}{
    lv_style_init(&_style);
    lv_style_set_line_width(&_style, width);
    lv_style_set_line_color(&_style, color);
    lv_style_set_line_rounded(&_style, rounded);

    _obj = lv_line_create( (lv_obj_t*)screen);
    lv_line_set_points( _obj, _point, 2);     /*Set the points*/
    lv_obj_add_style( _obj, &_style, 0);
}


/**
 * @brief    A sampled version of sin values. Each value are well magnified. 
 * @details  - Sample rate:     90/256
 *           - Amplitude rate:  1/256
 *           - Usage:  Assume `n` is a deg, for  0<=n<90 , sin(n) = TB_SINE[ n*256/90 ]/256;    cos(n) = TB_SINE[ 256-n*256/90 ]/256
 *                                          
*/
const unsigned char TB_SINE[] = {
    0,2,3,5,6,8,9,11,13,14,16,17,19,20,22,23,25,27,28,30,31,33,34,36,37,39,\
    41,42,44,45,47,48,50,51,53,54,56,57,59,60,62,63,65,67,68,70,71,73,74,76,\
    77,79,80,81,83,84,86,87,89,90,92,93,95,96,98,99,100,102,103,105,106,108,\
    109,110,112,113,115,116,117,119,120,122,123,124,126,127,128,130,131,132,\
    134,135,136,138,139,140,142,143,144,146,147,148,149,151,152,153,154,156,\
    157,158,159,161,162,163,164,165,167,168,169,170,171,172,174,175,176,177,\
    178,179,180,181,183,184,185,186,187,188,189,190,191,192,193,194,195,196,\
    197,198,199,200,201,202,203,204,205,206,207,208,208,209,210,211,212,213,\
    214,215,215,216,217,218,219,220,220,221,222,223,223,224,225,226,226,227,\
    228,228,229,230,231,231,232,232,233,234,234,235,236,236,237,237,238,238,\
    239,240,240,241,241,242,242,243,243,244,244,244,245,245,246,246,247,247,\
    247,248,248,248,249,249,249,250,250,250,251,251,251,252,252,252,252,252,\
    253,253,253,253,253,254,254,254,254,254,254,254,255,255,255,255,255,255,\
    255,255,255,255
};


/**
 * @class rh::widget524505::updateAngle
 * @param (none)
*/
void ClockNeedleComponent::updateAngle( void){
    u64 angle = ((((u64)this->_tick)<<10)/_ratio);    // 从系统滴答映射成余弦表格后的角度, 范围[0:1023]
    
    switch( angle>>8 ){
        case 0:{  // [0:90]
            _point[1].x = 120 +  ((_len*TB_SINE[     angle])>>8);         // x = L*sin(a)
            _point[1].y = 120 + -((_len*TB_SINE[0xff-angle])>>8);         // y = L*cos(a) = -L*sin(pi/2-a)

            _point[0].x = 120 + -((_offset*TB_SINE[     angle])>>8);    
            _point[0].y = 120 +  ((_offset*TB_SINE[0xff-angle])>>8);    
            break;
        }
            
        case 1:{  // [90:180]
            angle -= 256;  // ASSERT( angle<256 && angle>=0 )
            _point[1].x = 120 - -((_len*TB_SINE[0xff-angle])>>8);         // x = L*sin(a+pi/2) = -L*sin(pi/2-a)
            _point[1].y = 120 - -((_len*TB_SINE[     angle])>>8);         // y = L*cos(a+pi/2) = -L*sin(a)
            _point[0].x = 120 + -((_offset*TB_SINE[0xff-angle])>>8);
            _point[0].y = 120 + -((_offset*TB_SINE[     angle])>>8);
            break;
        }
        case 2:{  // [180:270]
            angle -= 512;
            _point[1].x = 120 + -((_len*TB_SINE[      angle])>>8);        // x = L*sin(a+pi) =  -L*sin(a)
            _point[1].y = 120 +  ((_len*TB_SINE[ 0xff-angle])>>8);        // y = L*cos(a+pi) =   L*sin(pi/2-a)
            _point[0].x = 120 - -((_offset*TB_SINE[      angle])>>8);
            _point[0].y = 120 -  ((_offset*TB_SINE[ 0xff-angle])>>8);
            break;
        }
        case 3:{  // [270:360]
            angle -= 768;
            _point[1].x = 120 - ((_len*TB_SINE[ 0xff-angle])>>8);         // x = L*sin(a+3pi/2) = L*sin(pi/2-a)
            _point[1].y = 120 - ((_len*TB_SINE[      angle])>>8);         // x = L*cos(a+3pi/2) =  L*sin(a)
            _point[0].x = 120 + ((_offset*TB_SINE[ 0xff-angle])>>8);    
            _point[0].y = 120 + ((_offset*TB_SINE[      angle])>>8);
            break;
        }
        default:{
            while(1); // Something wrong
        }
    }


    lv_line_set_points( _obj, _point, 2);     /*Set the points*/
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
/* Private Component in Widget 524505                                         */
/* @category     User Interface -> Widget                                     */
/* @brief        This class will draw a knob at the center of screen.         */
/******************************************************************************/
/**
 * @class rh::widget524505::ClockNeedleKnobComponent
 * @param  screen  - LVGL screen object pointer
 * @param  radius  - Knob radius. Recommanded value: 2 | 5 | 8
 * @param  width   - Knob width. Recommand value: 1 | 2
 * @param  color   - Knob color.
*/
ClockNeedleKnobComponent::ClockNeedleKnobComponent( void* screen, u8 radius, u8 width, lv_color_t color):
rh::UIComponent{}{
    obj = lv_arc_create( (lv_obj_t*)screen);

    this->radius( radius);
    this->width( width);

    lv_obj_set_style_arc_color( obj, color, LV_PART_MAIN      | LV_STATE_DEFAULT );
    lv_arc_set_bg_angles  ( obj,   0, 360);
    lv_arc_set_start_angle( obj, 270 );
    lv_arc_set_end_angle  ( obj, 629 );
     
    lv_obj_remove_style   ( obj, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style   ( obj, NULL, LV_PART_KNOB);       /*Be sure the knob is not displayed*/
    lv_obj_clear_flag     ( obj, LV_OBJ_FLAG_CLICKABLE);    /*To not allow adjusting by click*/
    lv_obj_center( obj);
}



void ClockNeedleKnobComponent::hide( bool cmd){
    if( cmd==true ){
        lv_obj_add_flag( obj, LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag( obj, LV_OBJ_FLAG_HIDDEN);
    }
}

u8    ClockNeedleKnobComponent::width( void)   { return (u8)lv_obj_get_style_arc_width( obj, LV_PART_MAIN|LV_STATE_DEFAULT); }
void  ClockNeedleKnobComponent::width( u8 val) { lv_obj_set_style_arc_width( obj, val,  LV_PART_MAIN|LV_STATE_DEFAULT);      }

u8    ClockNeedleKnobComponent::radius( void)  { return (u8)lv_obj_get_width( obj); }
void  ClockNeedleKnobComponent::radius( u8 val){ lv_obj_set_width ( obj, val);lv_obj_set_height( obj, val);}

lv_color_t   ClockNeedleKnobComponent::color( void)            { return lv_obj_get_style_arc_color( obj, LV_PART_MAIN|LV_STATE_DEFAULT);}
void         ClockNeedleKnobComponent::color( lv_color_t val)  { lv_obj_set_style_arc_color( obj, val, LV_PART_MAIN|LV_STATE_DEFAULT);  }










/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
/**
 * @class rh::widget524505::ClockPanelComponent
 * @param  screen  - LVGL screen object pointer
 * @param  
*/
ClockPanelComponent::ClockPanelComponent( void* screen, bool day_night){
    /* Create the object */
    obj = lv_analogclock_create( (lv_obj_t*)screen);
    lv_obj_set_pos(obj, 0, 0);                  /* No XY offset */
    lv_obj_set_size(obj, 240, 240);             /* Full screen display */
    lv_analogclock_hide_digits(obj, false);     /* Display clock number */

    lv_analogclock_set_major_ticks(obj, WIDTH_HOUR_NEEDLE  , LEN_HOUR_TICK  , COLOR_HOUR_TICK(day_night)  , 10);       /* Hour Tick */
    lv_analogclock_set_ticks      (obj, WIDTH_MINUTE_NEEDLE, LEN_MINUTE_TICK, COLOR_MINUTE_TICK(day_night));           /* Minute Tick */

    lv_style_set_text_color(&style, lv_color_make(0x1f, 0x1f, 0x1f));
	lv_style_set_text_font(&style, &lv_font_montserratMedium_16);
	lv_obj_add_style( obj, &style, LV_PART_TICKS|LV_STATE_DEFAULT);
}

/**
 * @class rh::widget524505::ClockPanelComponent
 * @param  cmd  - 0: Show this object;  1: Hide this object
*/
void ClockPanelComponent::hide( bool cmd){

}





/******************************************************************************/
/* API class. Must be presented in this file                                  */
/* @category:    User Interface -> Widget                                     */
/******************************************************************************/
/**
 * @class rh::widget524505::Widget
 * @brief 
*/
Widget::Widget( void * screen):
rh::ClockWidget{screen},
ccClockPanel   {screen, isDayNight( app.resource.time.bit.hour, app.resource.time.bit.minute )},
ccNeedleSecond {screen, 110, 30, 2, false, lv_palette_main( LV_PALETTE_RED)},
ccNeedleMinute {screen,  90,  0, 4,  true, lv_color_hex( 0x00ffffff)},
ccNeedleHour   {screen,  60,  0, 4,  true, lv_color_hex( 0x00ffffff)},
ccNeedleKnob   {screen,  10, 2, lv_color_hex( 0x00ff0000)}
{
    /* Config screen */
    lv_obj_set_scrollbar_mode( (lv_obj_t*)screen, LV_SCROLLBAR_MODE_OFF);
    
    /* Config style option */
    lv_style_reset( &style);
    lv_style_init( &style);
    lv_style_set_bg_color(&style, lv_color_make(0x0d, 0x0d, 0x0d));
    lv_style_set_bg_opa(&style, 255);
    lv_obj_add_style((lv_obj_t*)screen, &style, LV_PART_MAIN|LV_STATE_DEFAULT);

    ccNeedleSecond.ratio( 60000);
    ccNeedleMinute.ratio( 60000*60 );
    ccNeedleHour.ratio( 60000*60*12 );
}

/**
 * @class rh::widget524505::Widget
 * @brief 
*/
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
        ratio = ccNeedleSecond.ratio();
        ccNeedleSecond.run( ss*ratio/60, OPT_SET);
    }
    
    return done();
}

/**
 * @class rh::widget524505::Widget
 * @brief 
*/
int Widget::update( void){
    ccNeedleSecond.run( this->tickInc, OPT_FORWARD);
    ccNeedleMinute.run( this->tickInc, OPT_FORWARD);
    ccNeedleHour.run( this->tickInc, OPT_FORWARD);
    return done();
}






}
}