/**
  ******************************************************************************
  * @file    rh_test.c
  * @author  RandleH
  * @brief   This file contains test function for BSP device. To use, make sure
  *          enable the correspond config macro defined in "rh_config.h"
  * @include "rh_config.h"
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
#include "rh_test.h"
#include "rh_color.h"
#include "lvgl.h"

/* Exported functions ------------------------------------------------------- */
#if (RH_CFG_TEST_BSP)



static void callback_TestScreen__arc( void * obj, int32_t val){
    lv_arc_set_value(obj, val); 
}




/**
 * @brief This function will show a demo widge on screen if everything configed correctly.
 *        This function relies on `specific graphical library.
 * @param arg Reference on `E_TestScreen_t`
 *          @ref  TestScreen__helloworld
 *          @ref  TestScreen__colorwheel
 * @example 
 *          rh_test__bsp_screen( <option> );
 *          while(1){
 *              refreash_screen();
 *              sleep_ms(10);
 *          }
 * 
 * @retval If success, return 0.
*/
int rh_test__bsp_screen( E_TestScreen_t arg ){
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    switch( arg){
        
        case TestScreen__helloworld:{
            lv_color_t  color_screen_bkg = {.full = M_COLOR_DARKGRAY};
            lv_color_t  color_label      = {.full = M_COLOR_WHITE   };

            /*Change the active screen's background color*/
            lv_obj_t * screen = lv_scr_act();  // Get the active screen
            lv_obj_set_style_bg_color( screen, color_screen_bkg, LV_PART_MAIN); // Change its background color

            /*Create a white label, set its text and align it to the center*/
            lv_obj_t * label = lv_label_create(screen);
            lv_label_set_text(label, "Hello world");
            lv_obj_set_style_text_color(lv_scr_act(), color_label, LV_PART_MAIN);
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            break;
        }
        

        /**
         * @details 在屏幕上创建一个彩色环, 旋钮在底部, 为红色. 环中有一个滑块.
        */
        case TestScreen__colorwheel:{
            lv_obj_t * ui_Screen1 = lv_obj_create(NULL);
            lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
            lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x5A5A5A), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

            lv_obj_t * ui_Switch2 = lv_switch_create(ui_Screen1);
            lv_obj_set_width( ui_Switch2, 50);
            lv_obj_set_height( ui_Switch2, 25);
            lv_obj_set_x( ui_Switch2, 2 );
            lv_obj_set_y( ui_Switch2, -9 );
            lv_obj_set_align( ui_Switch2, LV_ALIGN_CENTER );
            lv_obj_set_style_bg_color(ui_Switch2, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_Switch2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

            lv_obj_t * ui_Colorwheel2 = lv_colorwheel_create(ui_Screen1,true);
            lv_obj_set_width( ui_Colorwheel2, 150);
            lv_obj_set_height( ui_Colorwheel2, 150);
            lv_obj_set_align( ui_Colorwheel2, LV_ALIGN_CENTER );

            lv_disp_load_scr( ui_Screen1);
            break;
        }
        
        /**
         * @details 在屏幕上旋转环形条. 围绕屏幕进行旋转. 中间有星巴克图案(可不加). 底部有进度条.
        */
        case TestScreen__spinner:{
            lv_obj_t * ui_Screen1 = lv_obj_create(NULL);
            lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
            lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x090909), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(ui_Screen1, LV_GRAD_DIR_NONE, LV_PART_MAIN| LV_STATE_DEFAULT);

            lv_obj_t * ui_ProgressBarBoot = lv_bar_create(ui_Screen1);
            lv_bar_set_range(ui_ProgressBarBoot, 0,255);
            lv_bar_set_value(ui_ProgressBarBoot,80,LV_ANIM_OFF);
            lv_obj_set_width( ui_ProgressBarBoot, lv_pct(30));
            lv_obj_set_height( ui_ProgressBarBoot, lv_pct(4));
            lv_obj_set_x( ui_ProgressBarBoot, 0 );
            lv_obj_set_y( ui_ProgressBarBoot, 75 );
            lv_obj_set_align( ui_ProgressBarBoot, LV_ALIGN_CENTER );
            lv_obj_set_style_bg_color(ui_ProgressBarBoot, lv_color_hex(0x2D2D2D), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_ProgressBarBoot, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_ProgressBarBoot, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_ProgressBarBoot, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
            
            extern lv_img_dsc_t ui_img_242849904;
            lv_obj_t * ui_Image1 = lv_img_create(ui_Screen1);
            lv_img_set_src(ui_Image1, &ui_img_242849904);
            lv_obj_set_width( ui_Image1, 240);
            lv_obj_set_height( ui_Image1, 240);
            lv_obj_set_align( ui_Image1, LV_ALIGN_CENTER );
            lv_obj_add_flag( ui_Image1, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
            lv_obj_clear_flag( ui_Image1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
            lv_img_set_zoom(ui_Image1,98);

            lv_obj_t * ui_Spinner2 = lv_spinner_create(ui_Screen1,1000,90);
            lv_obj_set_width( ui_Spinner2, 240);
            lv_obj_set_height( ui_Spinner2, 240);
            lv_obj_set_align( ui_Spinner2, LV_ALIGN_CENTER );
            lv_obj_clear_flag( ui_Spinner2, LV_OBJ_FLAG_CLICKABLE );    /// Flags
            lv_obj_set_style_arc_color(ui_Spinner2, lv_color_hex(0x343434), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_arc_opa(ui_Spinner2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(ui_Spinner2, lv_color_hex(0xFFF0F0), LV_PART_INDICATOR | LV_STATE_DEFAULT );
            lv_obj_set_style_arc_opa(ui_Spinner2, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
            
            lv_disp_load_scr( ui_Screen1);
            break;
        }

        case TestScreen__arc:{ // http://lvgl.100ask.net/8.2/widgets/core/arc.html
            lv_obj_t * ui_Screen1 = lv_obj_create(NULL);
            lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x090909), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

            lv_obj_t * ui_Arc = lv_arc_create( ui_Screen1);
            lv_obj_set_width( ui_Arc, 240);
            lv_obj_set_height( ui_Arc, 240); 

            lv_obj_set_style_arc_color( ui_Arc, lv_color_hex(0x2D2D2D), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_arc_opa  ( ui_Arc, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color( ui_Arc, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
            lv_obj_set_style_arc_opa  ( ui_Arc, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width( ui_Arc, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width( ui_Arc, 30, LV_PART_INDICATOR | LV_STATE_DEFAULT);

            lv_arc_set_rotation( ui_Arc, 270);
            lv_arc_set_bg_angles( ui_Arc, 0, 360);
            lv_obj_remove_style( ui_Arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
            lv_obj_clear_flag( ui_Arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
            lv_obj_center( ui_Arc);

            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, ui_Arc);
            lv_anim_set_exec_cb(&a, callback_TestScreen__arc);
            lv_anim_set_time(&a, 100000);
            lv_anim_set_repeat_count(&a, 0);    /*Just for the demo*/
            lv_anim_set_repeat_delay(&a, 1000);
            lv_anim_set_values(&a, 0, 100);
            lv_disp_load_scr( ui_Screen1);
            lv_anim_start(&a);

            
            break;
        }    
        default:
            break;
    }

    return 0;
}

#endif




