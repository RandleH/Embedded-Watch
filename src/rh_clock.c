  /*
  *=============================================================================
  *=============================================================================
  *                    Supported STM32F40xxx/41xxx devices
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(MHz)                            | 96
  *-----------------------------------------------------------------------------
  *        HCLK(MHz)                              | 96
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        HSE Frequency(MHz)                     | 25
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 25
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 192
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 4
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Main regulator output voltage          | Scale1 mode
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 5
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Enabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  */

#include "stm32f4xx.h"

#include "rh_common.h"
#include "rh_clock.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef union{
  struct{
    uint32_t    HSION     : 1;
    uint32_t    HSIRDY    : 1;
    uint32_t    Reserved2 : 1;
    uint32_t    HSITRIM   : 5;
    uint32_t    HSICAL    : 8;
    uint32_t    HSEON     : 1;
    uint32_t    HSERDY    : 1;
    uint32_t    HSEBYP    : 1;
    uint32_t    CSSON     : 1;
    uint32_t    Reserved23_20 : 4;
    uint32_t    PLLON     : 1;
    uint32_t    PLLRDY    : 1;
    uint32_t    PLLI2SON  : 1;
    uint32_t    PLLI2SRDY : 1;
    uint32_t    Reserved31_28 : 4;
  }bit;
  uint32_t data;
}RCC_CR;

typedef union{
  struct{
    uint32_t PLLM          : 6;
    uint32_t PLLN          : 9;
    uint32_t reserved15    : 1;
    uint32_t PLLP          : 2;
    uint32_t reserved21_18 : 4;
    uint32_t PLLSRC        : 1;
    uint32_t reserved23    : 1;
    uint32_t PLLQ          : 4;
    uint32_t reserved31_28 : 4;
  }bit;
  uint32_t data;
}RCC_PLLCFGR;

typedef union{
  struct{
    uint32_t       SW          : 2;
    const uint32_t SWS         : 2;
    uint32_t       HPRE        : 4;
    uint32_t       reserved9_8 : 2;
    uint32_t       PPRE1       : 3;
    uint32_t       PPRE2       : 3;
    uint32_t       RTCPRE      : 5;
    uint32_t       MCO1        : 2;
    uint32_t       I2SSRC      : 1;
    uint32_t       MCO1PRE     : 3;
    uint32_t       MCO2PRE     : 3;
    uint32_t       MCO2        : 2;
  }bit;
  uint32_t data;
}RCC_CFGR;

typedef union{
  struct{
    uint32_t   LSIRDYF         : 1;
    uint32_t   LSERDYF         : 1;
    uint32_t   HSIRDYF         : 1;
    uint32_t   HSERDYF         : 1;
    uint32_t   PLLRDYF         : 1;
    uint32_t   PLLI2SRDYF      : 1;
    uint32_t   Reserved6       : 1;
    uint32_t   CSSF            : 1;
    uint32_t   LSIRDYIE        : 1;
    uint32_t   LSERDYIE        : 1;
    uint32_t   HSIRDYIE        : 1;
    uint32_t   HSERDYIE        : 1;
    uint32_t   PLLRDYIE        : 1;
    uint32_t   PLLI2SRDYIE     : 1;
    uint32_t   Reserved15_14   : 2;
    uint32_t   LSIRDYC         : 1;
    uint32_t   LSERDYC         : 1;
    uint32_t   HSIRDYC         : 1;
    uint32_t   HSERDYC         : 1;
    uint32_t   PLLRDYC         : 1;
    uint32_t   PLLI2SRDYC      : 1;
    uint32_t   Reserved22      : 1;
    uint32_t   CSSC            : 1;
    uint32_t   Reserved31_24   : 8;
  }bit;
  uint32_t data;
}RCC_CIR;


#include "stm32f4xx_hal_rcc.h"


static inline void __resetClock(void){
  SCB->CPACR   |= ((3UL << 10*2)|(3UL << 11*2));
  RCC->CR      |= (uint32_t)0x00000001;
  RCC->CFGR     = 0x00000000;
  RCC->CR      &= (uint32_t)0xFEF6FFFF;
  RCC->PLLCFGR  = 0x24003010;
  RCC->CR      &= (uint32_t)0xFFFBFFFF;
  RCC->CIR      = 0x00000000;

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR      |= PWR_CR_VOS;
}





/**
 * @brief   Initialize the system clock. This function should be called 
 *          befored entering main function. 
 * @param   PLLM  PLL Mux Division. Valid range [2:63]
 * @param   PLLN  Main PLL scaler.  Valid range [50:432]
 * @param   PLLP  PLL DIV           Valid value [ DIV2, DIV4, DIV6, DIV8]
 * @param   PLLQ  USB DIV           48MHz output should be guranteed
 * @retval  If success, return 0
*/
int rh_clock__init( u32 PLLM, u32 PLLN, u32 PLLP, u32 PLLQ){
  __resetClock();

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = PLLM;
  RCC_OscInitStruct.PLL.PLLN = PLLN;
  RCC_OscInitStruct.PLL.PLLP = PLLP;
  RCC_OscInitStruct.PLL.PLLQ = PLLQ;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 2;
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK){
    return 1;
  }

  HAL_RCC_EnableCSS();
  return 0;
}



#ifdef __cplusplus
}
#endif




