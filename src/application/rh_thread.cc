





#include "application/rh_app.hh"
#include "rh_timer.h"
#include "rh_screen.h"
#include "rh_key.h"
#include "rh_light.h"
#include "rh_flash.h"


namespace rh{


#ifdef __cplusplus
extern "C"{
#endif
/**
 * @brief This function is mainly served for FreeRTOS. No need to called by user.
*/
void vApplicationGetIdleTaskMemory( StaticTask_t ** A, StackType_t  ** B, uint32_t * C   ){
    app.thread.getIdleTaskMemory( A, B, C);
}

/**
 * @brief This function is mainly served for FreeRTOS. No need to called by user.
*/
void vApplicationGetTimerTaskMemory( StaticTask_t ** A, StackType_t  ** B, uint32_t * C   ){
    app.thread.getTimerTaskMemory( A, B, C);
}




/**
 * @brief This task is for device initialization and will only run at once and 
 *        then will delete itself.
 * @param (none)
 * @note  Required minimum stack memory: 1024KB
 * @category Thread -> Dynamic Task Component
*/
RH_STATIC void __task_047685( void*){
    u8 retval = 0;
    while(1){
        taskENTER_CRITICAL();
        retval |= rh_screen__init();
        retval |= rh_light__init();
        retval |= rh_flash__init();
        retval |= rh_key__init();
        taskEXIT_CRITICAL();
        if( retval!=0 ){
            // [...] Raise a flag to an event group
        }
        vTaskDelete( NULL);
    }
    
}

/**
 * @brief This task is for loading the screen first time and 
 *        then will delete itself.
 * @param (none)
 * @note  Required minimum stack memory: 256KB
 * @category Thread -> Dynamic Task Component
*/
RH_STATIC void __task_264978( void*){
    while(1){
        app.gui.load(); 
        vTaskDelete( NULL);
    }
}

/**
 * @brief This task is for clock wheel ui angle update
 * @param tick User time tick
 * @note  - Memory: 256 KB
 *        - Period:   1 RTOS Tick
 * @category Thread -> Dynamic Task Component
*/
RH_STATIC void __task_747842( void* tick){

    u32 lastUserTick  = app.resource.userTick;   /*!< User tick record int the previous round */
    u32 tickRemainder = 0;                       /*!< Remainder of tick counts. Mutiplied by 3 in order to preserve accuracy */
    while(1){

        /******************************************************************************/
        /* Important Notes: The following code ONLY works for these conditions        */
        /*  - User Tick from `app` is in 1KHz frequency modulation                    */
        /*  - UI Tick for `uiCLockWheel` is in 1440 ticks per minute                  */
        /******************************************************************************/
        u32 deltaUserTick = 0; 
        {
            u32 tmp = app.resource.userTick;
            deltaUserTick = tmp - lastUserTick;
            lastUserTick  = tmp;
        }

        app.gui.uiClockWheel.increaseTick( (tickRemainder+deltaUserTick*3)/125 );
        tickRemainder = (tickRemainder+deltaUserTick*3)%125;
        
        // Only 06:30~18:00 will be recognized as Day
        if( ( app.resource.time.bit.hour==6 && app.resource.time.bit.minute>=30 )
            ||
            (app.resource.time.bit.hour >6 && app.resource.time.bit.hour<18     )){
            app.gui.uiClockWheel.setDayNight( false);
        }else{
            app.gui.uiClockWheel.setDayNight( true);
        }

        vTaskDelay(10);
    }
}








/**
 * @brief This task is for screen refreash and needs to run periodically
 * @param (none)
 * @note  - Memory: 256 KB
 *        - Period: 10 RTOS Tick
 * @category Thread -> Static Task Component
*/
RH_STATIC void __task_483377( void*){
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    xLastWakeTime = xTaskGetTickCount(); 

    while(1){
        vTaskDelayUntil( &xLastWakeTime, xFrequency ); 
            
        lv_tick_inc(xFrequency);
        lv_timer_handler();
    }
}

/**
 * @brief This task is for heart beat led and needs to run periodically
 * @param (none)
 * @note  Required minimum stack memory: 256KB
 * @category Thread -> Static Task Component
*/
RH_STATIC void __task_526892( void*){
    while(1){
        rh_light__switch( RH_LED_IDX__BLUE, true);
        vTaskDelay(300);
        rh_light__switch( RH_LED_IDX__BLUE, false);
        vTaskDelay(2000);
    }
}



#ifdef __cplusplus
}
#endif



/******************************************************************************/
/* [975338] This class handle system task distribution                        */
/* @category:    Thread Control                                               */
/* @uniqueID:    975338                                                       */
/******************************************************************************/
AppThread::AppThread(){

}

void AppThread::getIdleTaskMemory ( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t  ** ppxIdleTaskStackBuffer, uint32_t * pulIdleTaskStackSize){
    *ppxIdleTaskTCBBuffer    = &this->htcb_000000.second;                    
    *ppxIdleTaskStackBuffer  =  this->stak_000000;
    *pulIdleTaskStackSize    =  sizeof(this->stak_000000)/sizeof(StackType_t);            
}


void AppThread::getTimerTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t  ** ppxIdleTaskStackBuffer, uint32_t * pulIdleTaskStackSize){
    *ppxIdleTaskTCBBuffer    = &this->htcb_000001.second;                    
    *ppxIdleTaskStackBuffer  =  this->stak_000001;
    *pulIdleTaskStackSize    =  sizeof(this->stak_000001)/sizeof(StackType_t);
}

int AppThread::init( void){
    int retval = 0;
    retval |= addDeviceInit();
    retval |= addGUIInit();
    retval |= addMainService();
    return retval;
}

int AppThread::addDeviceInit( void){
    int retval = 0;
    this->dytcb.push_back({});
    retval |= pdPASS!=xTaskCreate( __task_047685, (const char*)"047685", 1024, NULL, 3U, &this->dytcb.back());
    
    return retval;
}

int AppThread::addGUIInit( void){
    int retval = 0;
    this->dytcb.push_back({});
    retval |= pdPASS!=xTaskCreate( __task_264978, (const char*)"264978", 1024, NULL, 3U, &this->dytcb.back());

    return 0;
}

int AppThread::addMainService( void){
    int retval = 0;
    
    htcb_483377.first = xTaskCreateStatic( __task_483377, "483377", sizeof(stak_483377)/sizeof(StackType_t), NULL, 48U, stak_483377, &htcb_483377.second );
    htcb_526892.first = xTaskCreateStatic( __task_526892, "526892", sizeof(stak_526892)/sizeof(StackType_t), NULL,  3U, stak_526892, &htcb_526892.second );
    
    this->dytcb.push_back({});
    retval |= pdPASS!=xTaskCreate( __task_747842, (const char*)"747842", 1024, NULL, 3U, &this->dytcb.back());

    retval |= (!htcb_483377.first)&&(!htcb_526892.first);

    return retval;
}





}