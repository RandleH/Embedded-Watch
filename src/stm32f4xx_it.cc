/**
  ******************************************************************************
  * @file    stm32f4xx_it.cc
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"




/* Private includes ----------------------------------------------------------*/
#include "rh_debug.h"
#include "rh_timer.h"
#include "rh_key.h"
#include "rh_light.h"
#include "application/rh_app.hh"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/



/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void){
  while(1);
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){while(1);}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void){while(1);}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void){while(1);}


/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void){
  while (1){}
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void){}

#if !RH_CFG_RTOS_ENABLE
/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}
#endif

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
void DMA2_Stream3_IRQHandler(void){
}

void DMA2_Stream2_IRQHandler(void){
}


extern bool key0;
void EXTI0_IRQHandler( void){
  
    if( RH_KEY__B==rh_key__read() ){
       key0 = true;
      __HAL_GPIO_EXTI_CLEAR_IT( GPIO_PIN_0);
      //...//
    }
    NVIC_ClearPendingIRQ( EXTI0_IRQn );
}


/**
 * @brief   Timer User Application Interrupt Service Routine Function. 
 * @param   (none)
 * @retval  (none)
 * @file  "rh_timer.c"
*/
#include <time.h>
void RH_CFG_TIMER_USERAPP_ISRFUNC(void){
  ++app.resource.userTick;
  static u32 tmpSec=0;

  if( app.resource.userTick-tmpSec==1000U ){           /* Modify*/
    if( app.resource.time.bit.second==59U ){
      app.resource.time.bit.second = 0;
      if( app.resource.time.bit.minute==59U ){
        app.resource.time.bit.minute = 0;
        if( app.resource.time.bit.hour==23U ){
          app.resource.time.bit.hour = 0;
          // Bits[11:0]
          // 0B 101011010101 = 0x0AD5   Months that have 31 Days 
          // 0B 010100101000 = 0x0528   Months that have 30 Days
          // 0B 000000000010 = 0x0002   February is the special one

          if( (app.resource.time.bit.date==31&&((1<<(app.resource.time.bit.month-1))&0x0AD5))
              ||
              (app.resource.time.bit.date==30&&((1<<(app.resource.time.bit.month-1))&0x0528))
              ||
              (app.resource.time.bit.date==28&&app.resource.time.bit.month==2&&((app.resource.time.bit.year&0x03)==0))
              || 
              (app.resource.time.bit.date==29&&app.resource.time.bit.month==2&&((app.resource.time.bit.year&0x03)==1))){
            
            app.resource.time.bit.date = 1;
            if(app.resource.time.bit.month==12U){
              app.resource.time.bit.month=1;
              ++app.resource.time.bit.year ;            /*!< Happy New Year!!! */
            }else{
              ++app.resource.time.bit.month;
            }
          }else{
            ++app.resource.time.bit.date;
          }



        }else{
          ++app.resource.time.bit.hour;
        }
      }else{
        ++app.resource.time.bit.minute;
      }
    }else{
      ++app.resource.time.bit.second;
    }


    tmpSec = app.resource.userTick;
  }




  RH_CFG_TIMER_USERAPP->SR = ~(TIM_SR_UIF);
}




#ifdef __cplusplus
}
#endif


/* USER CODE END 1 */
