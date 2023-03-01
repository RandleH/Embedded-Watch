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
#include <vector>

#include "lvgl.h"
#include "rh_common.h"
#include "rh_config.h"

/* Private define ------------------------------------------------------------*/
#ifndef RH_VISUAL_HH
#define RH_VISUAL_HH 

/* Namespace -----------------------------------------------------------------*/
namespace rh{


/* Exported constants --------------------------------------------------------*/
typedef enum{
    VISUAL_SCREEN__MAIN = 0,
    VISUAL_SCREEN__WATCH,
    VISUAL_SCREEN__SETTING,
    NUM_VISUAL_SCREEN
}E_VisualScreen_t;







/* Exported types ------------------------------------------------------------*/

/******************************************************************************/
/* This class control the global visual display                               */
/* @category:    User Interface                                               */
/******************************************************************************/
class Visual{
private:
    std::vector<lv_obj_t*>   screen;

public:
    Visual();

    lv_obj_t* getScreen( E_VisualScreen_t scr );
    int       load( E_VisualScreen_t scr );

};


/******************************************************************************/
/* This class is a template class. All widgets should inherit from this.      */
/* @category:    User Interface                                               */
/******************************************************************************/
class Widget{
protected:
    u32      tick;   /*!< Tick unit */
    void    *scr;    /*!< Screen Pointer */
public:
    Widget( void* screen);
    void * getScreen(void);
    virtual int increaseTick( u32 tick);
};


/******************************************************************************/
/* This class is a template class for clock widget. All clock widgets should  */
/* inherit from this                                                          */
/* @category:    User Interface                                               */
/******************************************************************************/
class ClockWidget : public Widget{
protected:
    bool am_pm;                      /*!< [0]=AM; [1]=PM */
public:
    ClockWidget( void* screen);
    virtual int setTime( bool am_pm, u8 hour, u8 minute, u8 second );
    virtual int setDayNight( bool day_night);
};



/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
class UIComponent{
protected:
    lv_obj_t *obj;
    u32        tick;
    u32        ratioTick;
public:
    UIComponent( void);
    UIComponent( void * screen );
    int resetTick( void);
};


/******************************************************************************/
/* This class is a UI component                                               */
/* @category:    User Interface -> Widget -> Component                        */
/******************************************************************************/
class PictureComponent:public UIComponent{
protected:
    lv_obj_t *obj;
    std::vector<const lv_img_dsc_t*> asset;

public:
    PictureComponent( void * screen, const lv_img_dsc_t *lv_img[], size_t nItems);
    PictureComponent( void * screen );

    int setRatio( u32 tick);

    int addImg( const lv_img_dsc_t *lv_img[], size_t nItems);
    int addImg( const lv_img_dsc_t *lv_img, bool immediate=false);

};



}
#endif
