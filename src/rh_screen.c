/**
  ******************************************************************************
  * @file    rh_screen.c
  * @author  RandleH
  * @brief   This file contains screen driver source code.
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

/// ====================================================== 
/// Device: GC9A01 240x240 TFT Screen
/// ====================================================== 
///  Screen Pin IO        | STM32 Pin IO | Periphral
/// ----------------------+--------------+----------------
///  SCL                  | PA5          | SPI1
/// ----------------------+--------------+----------------
///  SDA                  | PA7          | SPI1
/// ----------------------+--------------+----------------
///  DC                   | PC15         | GPIO
/// ----------------------+--------------+----------------
///  RST                  | PB2          | GPIO
/// ----------------------+--------------+----------------
///  CS                   | PC14         | GPIO
/// ------------------------------------------------------




/* Private define ------------------------------------------------------------*/
#define SPIx                SPI1

#define PIN_SDA             GPIO_PIN_5   // Port A
#define PIN_SCL             GPIO_PIN_7   // Port A
#define PIN_DC              GPIO_PIN_15  // Port C
#define PIN_RST             GPIO_PIN_2   // Port B
#define PIN_CS              GPIO_PIN_14  // Port C

#define GPIOx_SDA           GPIOA
#define GPIOx_SCL           GPIOA
#define GPIOx_DC            GPIOC
#define GPIOx_RST           GPIOB
#define GPIOx_CS            GPIOC



//  HAL_GPIO_WritePin( GPIOx_CS, PIN_CS, x==0?GPIO_PIN_RESET:GPIO_PIN_SET);
#define WRITE_CS(x)         do{ x==0? CLEAR_BIT( GPIOx_CS->ODR , PIN_CS) : SET_BIT( GPIOx_CS->ODR , PIN_CS ); }while(0)
#define WRITE_DC(x)         do{ x==0? CLEAR_BIT( GPIOx_DC->ODR , PIN_DC ): SET_BIT( GPIOx_DC->ODR , PIN_DC ); }while(0)
#define WRITE_RST(x)        do{ x==0? CLEAR_BIT( GPIOx_RST->ODR, PIN_RST): SET_BIT( GPIOx_RST->ODR, PIN_RST); }while(0)




#define SCREEN_WIDTH        RH_CFG_SCREEN_WIDTH
#define SCREEN_HEIGHT       RH_CFG_SCREEN_HEIGHT





/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#if defined (USE_HAL_DRIVER)
# include "stm32f4xx_hal_gpio.h"
# include "stm32f4xx_hal_rcc.h"
# include "stm32f4xx_hal_dma.h"
#endif

#include "rh_screen.h"
#include "rh_common.h"
#include "rh_timer.h"
#include "rh_debug.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C"{
#endif


/* Private functions ---------------------------------------------------------*/
/**
 * @brief   Assertion function
 * @param   expr  Expected expression
 * @retval  It is recommanded to run abort() in this funtion.
*/
static inline void __assert( bool expr){
    if( !expr){
        while(1);
    }
}


/**
 * @brief  Module delay function
 * @param  ms   Milliseconds
 * @retval (none)
*/
static inline void __delay_ms( unsigned long ms){
// #error "Please check this function and comment this line." 
    rh_timer__delayMS(ms);
    // vTaskDelay(ms);
}



/**
 * @brief   Config clock bus for peripheral modules
 * @param   (none)
 * @retval  (none)
*/
static inline void __configClock( void ){
// #error "Please check this function and comment this line."    
    SET_BIT( RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN|
                           RCC_AHB1ENR_GPIOBEN|
                           RCC_AHB1ENR_GPIOCEN);
    SET_BIT( RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
}

/**
 * @brief   Config alternative function IO pin
 * @param   (none)
 * @retval  (none)
*/
static inline void __configGPIO( void ){
// #error "Please check this function and comment this line."    
#if !defined (USE_HAL_DRIVER)
    
#else
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin( GPIOx_RST, PIN_RST, GPIO_PIN_RESET);
    HAL_GPIO_WritePin( GPIOx_DC , PIN_DC , GPIO_PIN_RESET);
    HAL_GPIO_WritePin( GPIOx_CS , PIN_CS , GPIO_PIN_RESET);

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin   = PIN_RST;
    HAL_GPIO_Init( GPIOx_RST, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = PIN_DC;
    HAL_GPIO_Init( GPIOx_DC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = PIN_CS;
    HAL_GPIO_Init( GPIOx_CS, &GPIO_InitStruct);
    
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    GPIO_InitStruct.Pin   = PIN_SDA;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    HAL_GPIO_Init( GPIOx_SDA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = PIN_SCL;
    HAL_GPIO_Init( GPIOx_SCL, &GPIO_InitStruct);

#endif
}




static SPI_HandleTypeDef hspi     = {0};
static inline void __configSPI(void){
    
    hspi.Instance               = SPIx;
    hspi.Init.Mode              = SPI_MODE_MASTER;
    hspi.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi.Init.NSS               = SPI_NSS_SOFT;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  // MUST set to `SPI_BAUDRATEPRESCALER_2`
    hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.CRCPolynomial     = 10;
    
    __assert( HAL_OK==HAL_SPI_Init(&hspi));
}


static inline void __transmitByte( u8 data){
    CLEAR_BIT( SPIx->CR1, SPI_CR1_SPE);
    CLEAR_BIT( SPIx->CR1, SPI_CR1_DFF);
    SET_BIT( SPIx->CR1, SPI_CR1_BIDIOE);
    if( !READ_BIT( SPIx->CR1, SPI_CR1_SPE) ){
        SET_BIT( SPIx->CR1, SPI_CR1_SPE);
    }
    SPIx->DR = data;

    while( !READ_BIT( SPIx->SR, SPI_SR_TXE));  // Blocking function
}





static inline void __writeCmd( u8 cmd) {
    WRITE_DC(0);
    // HAL_SPI_Transmit(&hspi, &cmd, sizeof(cmd), HAL_MAX_DELAY);  
    __transmitByte( cmd);  
}



static inline void __writeDataByte( u8 dat){
    WRITE_DC(1);
    // HAL_SPI_Transmit(&hspi, &dat, sizeof(dat), HAL_MAX_DELAY);
    __transmitByte( dat);
}



static void __setArea(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    //set the X coordinates
    __writeCmd(0x2A);
    __writeDataByte(0x00);
    __writeDataByte(x0);
    __writeDataByte(0x00);
    __writeDataByte(x1);

    //set the Y coordinates
    __writeCmd(0x2B);
    __writeDataByte(0x00);
    __writeDataByte(y0);
    __writeDataByte(0x00);
    __writeDataByte(y1);

    __writeCmd(0X2C);
}





/* Exported functions ------------------------------------------------------- */

/**
 * @brief   Initialize the screen
 * @param   (none)
 * @retval  If success, return 0.
 *          Return 1 if screen ID is wrong.
*/
int rh_screen__init(void){    
    WRITE_CS(0);

    __configClock();
    __configGPIO();
    __configSPI();


    rh_screen__reset();
    
    // Set the read / write scan direction of the frame memory
    __writeCmd(0x36); //MX, MY, RGB mode
    __writeDataByte(0xC8);    //0x08 set RGB


    __writeCmd(0xEF);
    __writeCmd(0xEB);
    __writeDataByte(0x14); 
    
    __writeCmd(0xFE);             
    __writeCmd(0xEF); 

    __writeCmd(0xEB);    
    __writeDataByte(0x14); 

    __writeCmd(0x84);            
    __writeDataByte(0x40); 

    __writeCmd(0x85);            
    __writeDataByte(0xFF); 

    __writeCmd(0x86);            
    __writeDataByte(0xFF); 

    __writeCmd(0x87);            
    __writeDataByte(0xFF);

    __writeCmd(0x88);            
    __writeDataByte(0x0A);

    __writeCmd(0x89);            
    __writeDataByte(0x21); 

    __writeCmd(0x8A);            
    __writeDataByte(0x00); 

    __writeCmd(0x8B);            
    __writeDataByte(0x80); 

    __writeCmd(0x8C);            
    __writeDataByte(0x01); 

    __writeCmd(0x8D);            
    __writeDataByte(0x01); 

    __writeCmd(0x8E);            
    __writeDataByte(0xFF); 

    __writeCmd(0x8F);            
    __writeDataByte(0xFF); 


    __writeCmd(0xB6);
    __writeDataByte(0x00);
    __writeDataByte(0x20);

    __writeCmd(0x36);
    __writeDataByte(0xC8);//Set as vertical screen

    __writeCmd(0x3A);            
    __writeDataByte(0x05); 


    __writeCmd(0x90);            
    __writeDataByte(0x08);
    __writeDataByte(0x08);
    __writeDataByte(0x08);
    __writeDataByte(0x08); 

    __writeCmd(0xBD);            
    __writeDataByte(0x06);
    
    __writeCmd(0xBC);            
    __writeDataByte(0x00);    

    __writeCmd(0xFF);            
    __writeDataByte(0x60);
    __writeDataByte(0x01);
    __writeDataByte(0x04);

    __writeCmd(0xC3);            
    __writeDataByte(0x13);
    __writeCmd(0xC4);            
    __writeDataByte(0x13);

    __writeCmd(0xC9);            
    __writeDataByte(0x22);

    __writeCmd(0xBE);            
    __writeDataByte(0x11); 

    __writeCmd(0xE1);            
    __writeDataByte(0x10);
    __writeDataByte(0x0E);

    __writeCmd(0xDF);            
    __writeDataByte(0x21);
    __writeDataByte(0x0c);
    __writeDataByte(0x02);

    __writeCmd(0xF0);   
    __writeDataByte(0x45);
    __writeDataByte(0x09);
    __writeDataByte(0x08);
    __writeDataByte(0x08);
    __writeDataByte(0x26);
    __writeDataByte(0x2A);

    __writeCmd(0xF1);    
    __writeDataByte(0x43);
    __writeDataByte(0x70);
    __writeDataByte(0x72);
    __writeDataByte(0x36);
    __writeDataByte(0x37);  
    __writeDataByte(0x6F);


    __writeCmd(0xF2);   
    __writeDataByte(0x45);
    __writeDataByte(0x09);
    __writeDataByte(0x08);
    __writeDataByte(0x08);
    __writeDataByte(0x26);
    __writeDataByte(0x2A);

    __writeCmd(0xF3);   
    __writeDataByte(0x43);
    __writeDataByte(0x70);
    __writeDataByte(0x72);
    __writeDataByte(0x36);
    __writeDataByte(0x37); 
    __writeDataByte(0x6F);

    __writeCmd(0xED);    
    __writeDataByte(0x1B); 
    __writeDataByte(0x0B); 

    __writeCmd(0xAE);            
    __writeDataByte(0x77);
    
    __writeCmd(0xCD);            
    __writeDataByte(0x63);        


    __writeCmd(0x70);            
    __writeDataByte(0x07);
    __writeDataByte(0x07);
    __writeDataByte(0x04);
    __writeDataByte(0x0E); 
    __writeDataByte(0x0F); 
    __writeDataByte(0x09);
    __writeDataByte(0x07);
    __writeDataByte(0x08);
    __writeDataByte(0x03);

    __writeCmd(0xE8);            
    __writeDataByte(0x34);

    __writeCmd(0x62);            
    __writeDataByte(0x18);
    __writeDataByte(0x0D);
    __writeDataByte(0x71);
    __writeDataByte(0xED);
    __writeDataByte(0x70); 
    __writeDataByte(0x70);
    __writeDataByte(0x18);
    __writeDataByte(0x0F);
    __writeDataByte(0x71);
    __writeDataByte(0xEF);
    __writeDataByte(0x70); 
    __writeDataByte(0x70);

    __writeCmd(0x63);            
    __writeDataByte(0x18);
    __writeDataByte(0x11);
    __writeDataByte(0x71);
    __writeDataByte(0xF1);
    __writeDataByte(0x70); 
    __writeDataByte(0x70);
    __writeDataByte(0x18);
    __writeDataByte(0x13);
    __writeDataByte(0x71);
    __writeDataByte(0xF3);
    __writeDataByte(0x70); 
    __writeDataByte(0x70);

    __writeCmd(0x64);            
    __writeDataByte(0x28);
    __writeDataByte(0x29);
    __writeDataByte(0xF1);
    __writeDataByte(0x01);
    __writeDataByte(0xF1);
    __writeDataByte(0x00);
    __writeDataByte(0x07);

    __writeCmd(0x66);            
    __writeDataByte(0x3C);
    __writeDataByte(0x00);
    __writeDataByte(0xCD);
    __writeDataByte(0x67);
    __writeDataByte(0x45);
    __writeDataByte(0x45);
    __writeDataByte(0x10);
    __writeDataByte(0x00);
    __writeDataByte(0x00);
    __writeDataByte(0x00);

    __writeCmd(0x67);            
    __writeDataByte(0x00);
    __writeDataByte(0x3C);
    __writeDataByte(0x00);
    __writeDataByte(0x00);
    __writeDataByte(0x00);
    __writeDataByte(0x01);
    __writeDataByte(0x54);
    __writeDataByte(0x10);
    __writeDataByte(0x32);
    __writeDataByte(0x98);

    __writeCmd(0x74);            
    __writeDataByte(0x10);    
    __writeDataByte(0x85);    
    __writeDataByte(0x80);
    __writeDataByte(0x00); 
    __writeDataByte(0x00); 
    __writeDataByte(0x4E);
    __writeDataByte(0x00);                    
    
    __writeCmd(0x98);            
    __writeDataByte(0x3e);
    __writeDataByte(0x07);

    __writeCmd(0x35);    
    __writeCmd(0x21);

    __writeCmd(0x11);
    __delay_ms(120);
    __writeCmd(0x29);
    __delay_ms(20);

    WRITE_CS(1);
    
    rh_screen__scan_mode( ScreenScanMode__R2L_D2U );

    rh_screen__fill_all( ((10)<<11) | ((20)<<5) | ((10)<<0)  );
    
    return 0;
}



int rh_screen__reset(void){
    WRITE_RST(1);
    __delay_ms(100);
    WRITE_RST(0);
    __delay_ms(100);
    WRITE_RST(1);
    __delay_ms(100);
    return 0;
}

/**
 * @brief   Fill entire screen with specific color
 * @param   arg  Fill color
 * @retval  If success, return 0.
 *          Return 1 if position is invalid
*/
int rh_screen__fill_all( E_ScreenColor_t arg){
    WRITE_CS(0);

    u8 data[] = { arg>>8, arg&0xff};
    __setArea(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);

    WRITE_DC(1);
    for( int y=0; y<SCREEN_HEIGHT; ++y){
        for( int x=0; x<SCREEN_WIDTH; ++x){
            // HAL_SPI_Transmit(&hspi, data, sizeof(data), HAL_MAX_DELAY);
            __transmitByte( data[0]);
            __transmitByte( data[1]);
        }
    }
    WRITE_CS(1);
    return 0;
}

/**
 * @brief   Fill a rectangular area on screen with specific color
 * @param   arg  Fill color
 * @param   xs   Start position on Axis X (Left-up corner)
 * @param   ys   Start position on Axis Y (Left-up corner)
 * @param   xe   End position on Axis X (Right-down corner)
 * @param   ye   End position on Axis Y (Right-down corner)
 * @retval  If success, return 0.
 *          Return 1 if position is invalid
*/
int rh_screen__fill( E_ScreenColor_t arg, int xs, int ys, int xe, int ye){
    if( xe<xs || ye<ys || ye<0 || ys<0) return 1;
    WRITE_CS(0);
    __setArea( xs, ys, xe, ye);

    WRITE_DC(1);
    for( int y=ys; y<=ye; ++y){
        for( int x=xs; x<=xe; ++x){
            // HAL_SPI_Transmit(&hspi, data, sizeof(data), HAL_MAX_DELAY);
            __transmitByte( (u8)(arg>>8));
            __transmitByte( (u8)(arg&0xff));
        }
    }
    WRITE_CS(1);
    return 0;
}

/**
 * @brief   This function will change the display direction
 * @param   arg   See the reference of @ScreenScanMode__X2X_X2X
 * @retval  If success, return 0.
 *          Return 1 if params is wrong.
*/
int rh_screen__scan_mode   ( E_ScreenScanMode_t arg){

#warning "NOT WORKING"
    WRITE_CS(0);
    __writeCmd(0x36);
    
    switch(arg){
        case ScreenScanMode__L2R_U2D: // PASSED
            __writeDataByte(0x08);
            break;
        case ScreenScanMode__R2L_D2U: // PASSED
            __writeDataByte(0xC8);
            break;
        case ScreenScanMode__R2L_U2D: // PASSED
            
            break;
        case ScreenScanMode__L2R_D2U: // PASSED

            break;
        case ScreenScanMode__D2U_L2R: // PASSED
            
            break;
        case ScreenScanMode__D2U_R2L: // PASSED
            
            break;
        case ScreenScanMode__U2D_L2R: // PASSED
            
            break;    
        case ScreenScanMode__U2D_R2L: // PASSED
            
            break;
        default:
            return 1;
    }
    WRITE_CS(1);
    return 0;
}


int rh_screen__flush_buf  ( E_ScreenColor_t buf[], int xs, int ys, int xe, int ye){
    if( xe<xs || ye<ys || ye<0 || ys<0) return 1;
    WRITE_CS(0);
    __setArea( xs, ys, xe, ye);

    WRITE_DC(1);
    for( int y=ys; y<=ye; ++y){
        for( int x=xs; x<=xe; ++x, ++buf){
            // HAL_SPI_Transmit(&hspi, data, sizeof(data), HAL_MAX_DELAY);
            __transmitByte( (u8)((*buf)>>8));
            __transmitByte( (u8)((*buf)&0xff));
        }
    }
    WRITE_CS(1);
    return 0;
}







/******************************************************************************/
/*               Application Interface Functions for LVGL 8.2.0               */
/******************************************************************************/

/* Includes ----------------------------------------------------------------- */



/* Private define ------------------------------------------------------------*/
#define LV_GRAM_BUF_SIZE      ( RH_CFG_GRAM_SIZE)


/* Private variables ---------------------------------------------------------*/
static lv_disp_draw_buf_t  disp_buf;
static lv_disp_drv_t       disp_drv;
static lv_disp_t          *disp_handle;
static lv_color_t          lv_gram1[RH_CFG_GRAM_SIZE];
static lv_color_t          lv_gram2[RH_CFG_GRAM_SIZE];

/* API functions ------------------------------------------------------------ */

#if LV_USE_LOG
static void rhlv__screen_log( const char *buf){
    rh_debug__printf("%s\n", buf);
}
#endif

void rhlv_screen__flush_cb(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p){
#if CFG_DEBUG__LOG
    
#endif
    WRITE_CS(0);
    __setArea( area->x1, area->y1, area->x2, area->y2);

    WRITE_DC(1);
    for( int y=area->y1; y<=area->y2; ++y){
        for( int x=area->x1; x<=area->x2; ++x, ++color_p){
            __transmitByte( (u8)((color_p->full)>>8)   );
            __transmitByte( (u8)((color_p->full)&0xff) );
        }
    }
    
    WRITE_CS(1);

    lv_disp_flush_ready( disp_drv);
}

/**
 * @brief  
 * @retval Pointer to the display driver
*/
lv_disp_t* rhlv_screen__getDisp( void){
    return &disp_drv;
}


/**
 * @brief   This function is to register the device to LVGL library
 * @param   (none)
 * @retval  If success, return 0. Return 1 if errors occur
*/
int rhlv_screen__init(void){
    lv_init();
    lv_disp_drv_init( &disp_drv );
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = rhlv_screen__flush_cb;
    disp_drv.hor_res = SCREEN_HEIGHT;
    disp_drv.ver_res = SCREEN_WIDTH;
    lv_disp_draw_buf_init( &disp_buf, lv_gram1, lv_gram2, LV_GRAM_BUF_SIZE);
    

    disp_handle = lv_disp_drv_register( &disp_drv);
#if LV_USE_LOG    
    lv_log_register_print_cb( rhlv__screen_log);
#endif    

    if( disp_handle==NULL)
        return 1;

    return 0;
}



#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT RandleH *****END OF FILE****/










////////////////////////////// Trash Code //////////////////////////////


#if 0
static u16 dma_buf_TX[RH_CFG_GRAM_SIZE] = {0};
int tx_finished = 1;
static inline void __transmitBytesDMA( const u8* buf, size_t len ){
    memcpy( dma_buf_TX, buf, len);


    HAL_SPI_Transmit_DMA(  &hspi, (u8*)buf, len ) ;


    // while( !tx_finished && READ_BIT(SPIx->SR, SPI_SR_BSY) );
    
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_EN);

    // while(READ_BIT(DMA2_Stream3->CR, DMA_SxCR_EN));
    
    // tx_finished = 0;
    // SPIx->CR1 |= SPI_CR1_SPE;
    // DMA2->LIFCR |= DMA_LIFCR_CTCIF3|DMA_LIFCR_CHTIF3|DMA_LIFCR_CTEIF3|DMA_LIFCR_CDMEIF3|DMA_LIFCR_CFEIF3;
    // DMA2_Stream3->PAR  = (u32)&(SPI1->DR);
    // // DMA2_Stream3->M0AR = (u32)( memcpy( dma_buf_TX, buf, len)  );

    // DMA2_Stream3->M0AR = (u32)memset( dma_buf_TX, 0x78, len);
    // DMA2_Stream3->NDTR = len;
    // DMA2_Stream3->CR  |= DMA_SxCR_EN;
}


DMA_HandleTypeDef hspi_dma = {0};
    RCC->AHB1ENR|=RCC_AHB1ENR_DMA2EN;
    hspi_dma.Instance = DMA2_Stream3;
    hspi_dma.Init.Channel = DMA_CHANNEL_3;
    hspi_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hspi_dma.Init.PeriphInc = DMA_PINC_DISABLE;
    hspi_dma.Init.MemInc = DMA_MINC_ENABLE;
    hspi_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hspi_dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hspi_dma.Init.Mode = DMA_NORMAL;
    hspi_dma.Init.Priority = DMA_PRIORITY_HIGH;
    hspi_dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hspi_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hspi_dma.Init.MemBurst = DMA_MBURST_SINGLE;
    hspi_dma.Init.PeriphBurst = DMA_PBURST_SINGLE;

    __assert( HAL_OK==HAL_DMA_Init( &hspi_dma));
    
    

    __HAL_LINKDMA( &hspi, hdmatx, hspi_dma);

    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    NVIC_EnableIRQ(DMA2_Stream3_IRQn);

#if RH_CFG_GRAM_DMA_ENABLE
    // SET_BIT( SPIx->CR2, SPI_CR2_TXDMAEN);
#endif



#if RH_CFG_GRAM_DMA_ENABLE

/// ===============================================================================
///  DMA Base  | Stream | Channel | Direction          | Data Align | Interrupt
/// -----------+--------+---------+--------------------+------------+--------------
///  DMA2      | 3      | CH3     | Memory->Peripheral | 8-bit      | Yes
/// -------------------------------------------------------------------------------

/**
 * @brief
 * @param 
 * @retval 
*/
static inline void __configDMA(void){
    // SET_BIT  ( RCC->AHB1ENR    , RCC_AHB1ENR_DMA2EN);  // DMA Clock
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_EN       );  // Disable DMA2 Stream3
    // while( (DMA2_Stream3->CR)&DMA_SxCR_EN){;}          // Wait until disabled...

    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_CHSEL_0  );  // Channel 3
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_CHSEL_1  );
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_CHSEL_2  );

    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_MSIZE_0  );  // 8-Bit Data Size
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_MSIZE_1  );

    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_PSIZE_0  );  // 8-Bit Data Size
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_PSIZE_1  );
   
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_MINC     );  
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_PINC     );

    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_DIR_0    );  // Memory->Peripheral
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_DIR_1    );

    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_TCIE     );  
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_HTIE     );  
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_TEIE     );  
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_DMEIE    );

    // DMA2_Stream3->FCR |= DMA_SxFCR_DMDIS;
    // DMA2_Stream3->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);

    // NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    // RCC->AHB1ENR|=RCC_AHB1ENR_DMA2EN;
	// DMA2_Stream3->CR&=~DMA_SxCR_EN;
	// while((DMA2_Stream3->CR)&DMA_SxCR_EN){;}
	// DMA2_Stream3->CR=DMA_SxCR_MINC|DMA_SxCR_DIR_0|DMA_SxCR_TCIE|DMA_SxCR_HTIE|DMA_SxCR_TEIE|DMA_SxCR_DMEIE;

    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_CHSEL_0  );  // Channel 3
    // SET_BIT  ( DMA2_Stream3->CR, DMA_SxCR_CHSEL_1  );
    // CLEAR_BIT( DMA2_Stream3->CR, DMA_SxCR_CHSEL_2  );

	// DMA2_Stream3->FCR |= DMA_SxFCR_DMDIS;
    // DMA2_Stream3->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);
	
	// NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}
#endif

__transmitBytesDMA( (u8*)color_p, (area->x2-area->x1+1)*(area->y2-area->y1+1)*sizeof(*color_p));

#endif
