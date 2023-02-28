/*
 * Copyright 2023 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "./events_init.h"
#include <stdio.h>
#include "lvgl.h"

static lv_timer_t  * task_meter;


void events_init(lv_ui *ui)
{
}

void video_play(lv_ui *ui)
{

}

void screen_meter_timer_cb(struct _lv_timer_t * dummy){
//////// Do nothing /////////
}




static void screen_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_SCREEN_UNLOAD_START:
	{
		lv_timer_del(task_meter);
	}
		break;
	case LV_EVENT_SCREEN_LOADED:
	{
		task_meter = lv_timer_create(screen_meter_timer_cb, 100, &guider_ui);
	}
		break;
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen, screen_event_handler, LV_EVENT_ALL, ui);
}
