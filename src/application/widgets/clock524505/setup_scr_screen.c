/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "./gui_guider.h"
#include "events_init.h"


static int screen_uiClassicClock_hour_value = 4;
static int screen_uiClassicClock_min_value = 20;
static int screen_uiClassicClock_sec_value = 50;
void screen_uiClassicClock_timer(lv_timer_t *timer)
{	clock_count(&screen_uiClassicClock_hour_value, &screen_uiClassicClock_min_value, &screen_uiClassicClock_sec_value);
	if (lv_obj_is_valid(guider_ui.screen_uiClassicClock))
	{
		lv_analogclock_set_time(guider_ui.screen_uiClassicClock, screen_uiClassicClock_hour_value, screen_uiClassicClock_min_value, screen_uiClassicClock_sec_value);
	}
}

void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	static lv_style_t style_screen_main_main_default;
	if (style_screen_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_main_main_default);
	else
		lv_style_init(&style_screen_main_main_default);
	lv_style_set_bg_color(&style_screen_main_main_default, lv_color_make(0x0d, 0x0d, 0x0d));
	lv_style_set_bg_opa(&style_screen_main_main_default, 255);
	lv_obj_add_style(ui->screen, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	static bool screen_uiClassicClock_timer_enabled = false;

	//Write codes screen_uiClassicClock
	ui->screen_uiClassicClock = lv_analogclock_create(ui->screen);
	lv_obj_set_pos(ui->screen_uiClassicClock, 0, 0);
	lv_obj_set_size(ui->screen_uiClassicClock, 240, 240);
	lv_analogclock_hide_digits(ui->screen_uiClassicClock, false);
	lv_analogclock_set_major_ticks(ui->screen_uiClassicClock, 2, 13, lv_color_make(0xff, 0xff, 0xff), 10);
	lv_analogclock_set_ticks(ui->screen_uiClassicClock, 2, 10, lv_color_make(0x6b, 0x6b, 0x6b));
	lv_analogclock_set_hour_needle_line(ui->screen_uiClassicClock, 5, lv_color_make(0xff, 0xff, 0xff), -56);
	lv_analogclock_set_min_needle_line(ui->screen_uiClassicClock, 5, lv_color_make(0xff, 0xff, 0xff), -30);
	lv_analogclock_set_sec_needle_line(ui->screen_uiClassicClock, 2, lv_color_make(0xff, 0x00, 0x27), -10);
	lv_analogclock_set_time(ui->screen_uiClassicClock, screen_uiClassicClock_hour_value, screen_uiClassicClock_min_value, screen_uiClassicClock_sec_value);

	//create timer
	if (!screen_uiClassicClock_timer_enabled) {
		lv_timer_create(screen_uiClassicClock_timer, 1000, NULL);
		screen_uiClassicClock_timer_enabled = true;
	}
	//Write style state: LV_STATE_DEFAULT for style_screen_uiclassicclock_main_main_default
	static lv_style_t style_screen_uiclassicclock_main_main_default;
	if (style_screen_uiclassicclock_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_uiclassicclock_main_main_default);
	else
		lv_style_init(&style_screen_uiclassicclock_main_main_default);
	lv_style_set_bg_color(&style_screen_uiclassicclock_main_main_default, lv_color_make(0x15, 0x15, 0x15));
	lv_style_set_bg_grad_color(&style_screen_uiclassicclock_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_uiclassicclock_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_uiclassicclock_main_main_default, 255);
	lv_style_set_shadow_width(&style_screen_uiclassicclock_main_main_default, 0);
	lv_style_set_shadow_color(&style_screen_uiclassicclock_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_shadow_opa(&style_screen_uiclassicclock_main_main_default, 255);
	lv_style_set_shadow_spread(&style_screen_uiclassicclock_main_main_default, 0);
	lv_style_set_shadow_ofs_x(&style_screen_uiclassicclock_main_main_default, 0);
	lv_style_set_shadow_ofs_y(&style_screen_uiclassicclock_main_main_default, 0);
	lv_style_set_border_color(&style_screen_uiclassicclock_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_border_width(&style_screen_uiclassicclock_main_main_default, 0);
	lv_style_set_border_opa(&style_screen_uiclassicclock_main_main_default, 255);
	lv_obj_add_style(ui->screen_uiClassicClock, &style_screen_uiclassicclock_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_uiclassicclock_main_ticks_default
	static lv_style_t style_screen_uiclassicclock_main_ticks_default;
	if (style_screen_uiclassicclock_main_ticks_default.prop_cnt > 1)
		lv_style_reset(&style_screen_uiclassicclock_main_ticks_default);
	else
		lv_style_init(&style_screen_uiclassicclock_main_ticks_default);
	lv_style_set_text_color(&style_screen_uiclassicclock_main_ticks_default, lv_color_make(0xff, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_uiclassicclock_main_ticks_default, &lv_font_montserratMedium_12);
	lv_obj_add_style(ui->screen_uiClassicClock, &style_screen_uiclassicclock_main_ticks_default, LV_PART_TICKS|LV_STATE_DEFAULT);

	//Init events for screen
	events_init_screen(ui);
}