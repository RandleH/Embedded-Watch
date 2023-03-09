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
#include "lvgl.h"
#include "./port.hh"
#include "asset/asset.h"


/* Namespace -----------------------------------------------------------------*/
namespace rh{
namespace widget454570{

Widget::Widget( void* screen):rh::ClockWidget{screen}{

}

int Widget::setTime( u8 hour, u8 minute, u8 second ){return -1;}
int Widget::setDayNight( bool day_night){return -1;}
int Widget::update( void){return -1;}

void Widget::foo( void){
    ///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_Screen1;
lv_obj_t *ui_Image1;
lv_obj_t *ui_Image2;
lv_obj_t *ui_Image3;
lv_obj_t *ui_Image4;
lv_obj_t *ui_Image5;
lv_obj_t *ui_Image6;
lv_obj_t *ui_Image7;
lv_obj_t *ui_Image8;
lv_obj_t *ui_Image9;
lv_obj_t *ui_Image10;
lv_obj_t *ui_Image11;
lv_obj_t *ui_Image12;
lv_obj_t *ui_Arc2;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////


#define COLOR_1     lv_color_hex( 0x921b7b)
#define COLOR_2     lv_color_hex( 0x891d96)
#define COLOR_3     lv_color_hex( 0x741d96)
#define COLOR_4     lv_color_hex( 0x522e98)
#define COLOR_5     lv_color_hex( 0x2e4398)
#define COLOR_6     lv_color_hex( 0x2a6593)
#define COLOR_7     lv_color_hex( 0x2e899c)

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////



lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);


ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);


// 3
ui_Image1 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image1, &ui_img_1625705288);
lv_obj_set_width( ui_Image1, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image1, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image1, lv_pct(-3) );
lv_obj_set_y( ui_Image1, lv_pct(0) );
lv_obj_set_align( ui_Image1, LV_ALIGN_RIGHT_MID );
lv_obj_add_flag( ui_Image1, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image1,192);
lv_obj_set_style_img_recolor_opa(ui_Image1, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image1, COLOR_1, 0);

// 6
ui_Image2 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image2, &ui_img_1375507867);
lv_obj_set_width( ui_Image2, LV_SIZE_CONTENT);  /// 48
lv_obj_set_height( ui_Image2, LV_SIZE_CONTENT);   /// 47
lv_obj_set_x( ui_Image2, 0 );
lv_obj_set_y( ui_Image2, lv_pct(-3) );
lv_obj_set_align( ui_Image2, LV_ALIGN_BOTTOM_MID );
lv_obj_add_flag( ui_Image2, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image2,192);
lv_obj_set_style_img_recolor_opa(ui_Image2, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image2, COLOR_4, 0);

// 9
ui_Image3 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image3, &ui_img_1625705288);
lv_obj_set_width( ui_Image3, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image3, LV_SIZE_CONTENT);   /// 32
lv_obj_set_y( ui_Image3, 0 );
lv_obj_set_x( ui_Image3, lv_pct(3) );
lv_obj_set_align( ui_Image3, LV_ALIGN_LEFT_MID );
lv_obj_add_flag( ui_Image3, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image3, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image3,192);
lv_obj_set_style_img_recolor_opa(ui_Image3, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image3, COLOR_7, 0);

// 12
ui_Image4 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image4, &ui_img_1625705288);
lv_obj_set_width( ui_Image4, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image4, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image4, 0 );
lv_obj_set_y( ui_Image4, lv_pct(3) );
lv_obj_set_align( ui_Image4, LV_ALIGN_TOP_MID );
lv_obj_add_flag( ui_Image4, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image4, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image4,192);
lv_obj_set_style_img_recolor_opa(ui_Image4, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image4, COLOR_4, 0);

// 1
ui_Image5 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image5, &ui_img_1682387597);
lv_obj_set_width( ui_Image5, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image5, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image5, 50 );
lv_obj_set_y( ui_Image5, -82 );
lv_obj_set_align( ui_Image5, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image5, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image5, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image5,192);
lv_obj_set_style_img_recolor_opa(ui_Image5, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image5, COLOR_3, 0);

// 11
ui_Image6 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image6, &ui_img_1181490689);
lv_obj_set_width( ui_Image6, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image6, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image6, -50 );
lv_obj_set_y( ui_Image6, -82 );
lv_obj_set_align( ui_Image6, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image6, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image6, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image6,192);
lv_obj_set_style_img_recolor_opa(ui_Image6, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image6, COLOR_5, 0);

// 2
ui_Image7 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image7, &ui_img_1181490689);
lv_obj_set_width( ui_Image7, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image7, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image7, 82 );
lv_obj_set_y( ui_Image7, -50 );
lv_obj_set_align( ui_Image7, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image7, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image7, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image7,192);
lv_obj_set_style_img_recolor_opa(ui_Image7, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image7, COLOR_1, 0);

// 10
ui_Image8 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image8, &ui_img_1682387597);
lv_obj_set_width( ui_Image8, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image8, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image8, -82 );
lv_obj_set_y( ui_Image8, -50 );
lv_obj_set_align( ui_Image8, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image8, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image8, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image8,192);
lv_obj_set_style_img_recolor_opa(ui_Image8, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image8, COLOR_6, 0);

// 8
ui_Image9 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image9, &ui_img_1181490689);
lv_obj_set_width( ui_Image9, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image9, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image9, -82 );
lv_obj_set_y( ui_Image9, 50 );
lv_obj_set_align( ui_Image9, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image9, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image9, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image9,192);
lv_obj_set_style_img_recolor_opa(ui_Image9, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image9, COLOR_6, 0);

// 7
ui_Image10 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image10, &ui_img_1682387597);
lv_obj_set_width( ui_Image10, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image10, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image10, -50 );
lv_obj_set_y( ui_Image10, 82 );
lv_obj_set_align( ui_Image10, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image10, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image10, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image10,192);
lv_obj_set_style_img_recolor_opa(ui_Image10, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image10, COLOR_5, 0);

// 5
ui_Image11 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image11, &ui_img_1181490689);
lv_obj_set_width( ui_Image11, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image11, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image11, 50 );
lv_obj_set_y( ui_Image11, 82 );
lv_obj_set_align( ui_Image11, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image11, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image11, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image11,192);
lv_obj_set_style_img_recolor_opa(ui_Image11, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image11, COLOR_3, 0);

// 4
ui_Image12 = lv_img_create(ui_Screen1);
lv_img_set_src(ui_Image12, &ui_img_1682387597);
lv_obj_set_width( ui_Image12, LV_SIZE_CONTENT);  /// 32
lv_obj_set_height( ui_Image12, LV_SIZE_CONTENT);   /// 32
lv_obj_set_x( ui_Image12, 82 );
lv_obj_set_y( ui_Image12, 50 );
lv_obj_set_align( ui_Image12, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image12, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image12, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_img_set_zoom(ui_Image12,192);
lv_obj_set_style_img_recolor_opa(ui_Image12, 0xff, 0); 
lv_obj_set_style_img_recolor(ui_Image12, COLOR_1, 0);

ui_Arc2 = lv_arc_create(ui_Screen1);
lv_obj_set_width( ui_Arc2, 15);
lv_obj_set_height( ui_Arc2, 15);
lv_obj_set_align( ui_Arc2, LV_ALIGN_CENTER );
lv_obj_remove_style( ui_Arc2, NULL, LV_PART_KNOB | LV_PART_INDICATOR);
lv_obj_clear_flag( ui_Arc2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN );    /// Flags
lv_arc_set_bg_angles(ui_Arc2,0,359);


lv_disp_load_scr( ui_Screen1);

}


}
}


