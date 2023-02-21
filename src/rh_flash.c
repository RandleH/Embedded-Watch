/**
  ******************************************************************************
  * @file    rh_flash.c
  * @author  RandleH
  * @brief   This file contains code for SPI EEPRAM 4MB.
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



/// Table
/// ====================================================== 
///  Flash Pin IO         | STM32 Pin IO | Peripheral
/// ----------------------+--------------+----------------
///  DI                   | PA7          | SPI - MOSI
/// ----------------------+--------------+----------------
///  CLK                  | PA5          | SPI - SCLK
/// ----------------------+--------------+----------------
///  CS                   | PA4          | GPIO
/// ----------------------+--------------+----------------
///  DO                   | PA6          | SPI - MISO
/// ----------------------+--------------+----------------
///  RST/HOLD             | VCC          | GPIO
/// ----------------------+--------------+----------------
///  WP                   | VCC          | GPIO   
/// ------------------------------------------------------


/// Memory Architecture
/// ====================================================== 
/// Block[0] Block[1] ... Block[64]
///   |                        
///   |        
///   |
///   |---------+---------+---------+       
///   |         |         |         |
/// Sector[0] Sector[1]  ...      Sector[15]
///   |
///   |
///   |
///   |----------+----------+----------+
///   |          |          |          |
/// Byte[0]    Byte[1]     ...       Byte[4095]
///
/// ------------------------------------------------------








/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "rh_flash.h"
#include "rh_timer.h"


#ifdef __cplusplus
extern "C"{
#endif



/* Private define ------------------------------------------------------------*/

#define _W25QXX_DEBUG       RH_CFG_FLASH_DEBUG
#define printf              rh_debug__log
#define wprintf             rh_debug__wprintf



#define W25Q32_READ_CMD     0x03
#define W25Q32_WRITE_CMD    0x02
#define W25Q32_ERASE_CMD    0x20

#define W25Q32_PAGE_SIZE    256
#define W25Q32_SECTOR_SIZE  4096
#define W25Q32_BLOCK_SIZE   65536


#define SPIx                SPI1
#define PIN_CS              GPIO_PIN_4
#define PIN_MOSI            GPIO_PIN_7
#define PIN_MISO            GPIO_PIN_6
#define PIN_SCLK            GPIO_PIN_5

#define GPIOx_CS            GPIOA
#define GPIOx_MOSI          GPIOA
#define GPIOx_MISO          GPIOA
#define GPIOx_SCLK          GPIOA

#define WRITE_CS(x)         do{ x==0? CLEAR_BIT( GPIOx_CS->ODR , PIN_CS) : SET_BIT( GPIOx_CS->ODR , PIN_CS ); }while(0)



#define W25Q32_CMD_WRITE_ENABLE                 0x06
#define W25Q32_CMD_WRITE_DISABLE                0x04
#define W25Q32_CMD_CHIP_ERASE                   0xC7

#define W25Q32_CMD_READ_JEDEC_ID                0x9F
#define W25Q32_CMD_DUMMY                        0xA5


typedef enum{
    W25Q10 = 1,
    W25Q20,
    W25Q40,
    W25Q80,
    W25Q16,
    W25Q32,
    W25Q64,
    W25Q128,
    W25Q256,
    W25Q512,
    Unknown
} W25QXX_ID_t;


typedef struct{
    W25QXX_ID_t ID;
    uint8_t UniqID[8];
    uint16_t PageSize;
    uint32_t PageCount;
    uint32_t SectorSize;
    uint32_t SectorCount;
    uint32_t BlockSize;
    uint32_t BlockCount;
    uint32_t CapacityInKiloByte;
    uint8_t StatusRegister1;
    uint8_t StatusRegister2;
    uint8_t StatusRegister3;
    uint8_t Lock;
} w25qxx_t;

static w25qxx_t w25qxx = {.ID=Unknown};
#define W25QXX_DUMMY_BYTE 0xA5


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



static SPI_HandleTypeDef hspi1 = {0};
static inline void __configXXX( void){
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

    GPIO_InitStruct.Pin  = PIN_MOSI;
    HAL_GPIO_Init( GPIOx_MOSI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = PIN_MISO;
    HAL_GPIO_Init( GPIOx_MISO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = PIN_SCLK;
    HAL_GPIO_Init( GPIOx_SCLK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = PIN_CS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init( GPIOx_CS, &GPIO_InitStruct);

    hspi1.Instance               = SPIx;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 10;
    __assert( HAL_OK==HAL_SPI_Init(&hspi1));

}








/* Exported functions ------------------------------------------------------- */

int rh_flash__init( void){
    __configXXX();
    
    W25qxx_Init();

    return 0;
}


int rh_flash__erase( u32 sector_address){
    uint8_t cmd[4];
    cmd[0] = W25Q32_ERASE_CMD;
    cmd[1] = (sector_address >> 16) & 0xFF;
    cmd[2] = (sector_address >> 8) & 0xFF;
    cmd[3] = sector_address & 0xFF;

    WRITE_CS(0);;
    HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), HAL_MAX_DELAY);
    WRITE_CS(1);;

    return 0;
}

int rh_flash__read( u32 address, u8 *data, size_t length){
    uint8_t cmd[4];
    cmd[0] = W25Q32_READ_CMD;
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    WRITE_CS(0);;
    HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, data, length, HAL_MAX_DELAY);
    WRITE_CS(1);;

    return 0;
}

int rh_flash__write( u32 address, const u8 *data, size_t length){
    uint8_t cmd[4];
    cmd[0] = W25Q32_WRITE_CMD;
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    WRITE_CS(0);;
    HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, (u8*)data, length, HAL_MAX_DELAY);
    WRITE_CS(1);;

    return 0;
}


int rh_flash__read_id( u32* id){
    if( !id ) return 1;
    
    u8 cmd   = W25Q32_CMD_READ_JEDEC_ID;
    u8 tmp[3] = {0};
	
    WRITE_CS(0);;

    HAL_SPI_Transmit (&hspi1,    &cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_SPI_Receive  (&hspi1, &tmp[0], 1, HAL_MAX_DELAY);
    HAL_SPI_Receive  (&hspi1, &tmp[1], 1, HAL_MAX_DELAY);
    HAL_SPI_Receive  (&hspi1, &tmp[2], 1, HAL_MAX_DELAY);

    
	WRITE_CS(1);

    (*id)  = (tmp[0]<<16) | (tmp[1]<<8) | (tmp[2]);
    (*id) &= 0x00ffffff;
	return 0;
}









//###################################################################################################################
uint8_t W25qxx_Spi(uint8_t Data)
{
	uint8_t ret;
	HAL_SPI_TransmitReceive(&hspi1, &Data, &ret, 1, 100);
	return ret;
}
//###################################################################################################################
uint32_t W25qxx_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	WRITE_CS(0);
	W25qxx_Spi(0x9F);
	Temp0 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	Temp1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	Temp2 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	WRITE_CS(1);
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
//###################################################################################################################
void W25qxx_ReadUniqID(void)
{
	WRITE_CS(0);
	W25qxx_Spi(0x4B);
	for (uint8_t i = 0; i < 4; i++)
		W25qxx_Spi(W25QXX_DUMMY_BYTE);
	for (uint8_t i = 0; i < 8; i++)
		w25qxx.UniqID[i] = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	WRITE_CS(1);
}
//###################################################################################################################
void W25qxx_WriteEnable(void)
{
	WRITE_CS(0);
	W25qxx_Spi(0x06);
	WRITE_CS(1);
	__delay_ms(1);
}
//###################################################################################################################
void W25qxx_WriteDisable(void)
{
	WRITE_CS(0);
	W25qxx_Spi(0x04);
	WRITE_CS(1);
	__delay_ms(1);
}
//###################################################################################################################
uint8_t W25qxx_ReadStatusRegister(uint8_t SelectStatusRegister_1_2_3)
{
	uint8_t status = 0;
	WRITE_CS(0);
	if (SelectStatusRegister_1_2_3 == 1)
	{
		W25qxx_Spi(0x05);
		status = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		w25qxx.StatusRegister1 = status;
	}
	else if (SelectStatusRegister_1_2_3 == 2)
	{
		W25qxx_Spi(0x35);
		status = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		w25qxx.StatusRegister2 = status;
	}
	else
	{
		W25qxx_Spi(0x15);
		status = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		w25qxx.StatusRegister3 = status;
	}
	WRITE_CS(1);
	return status;
}
//###################################################################################################################
void W25qxx_WriteStatusRegister(uint8_t SelectStatusRegister_1_2_3, uint8_t Data)
{
	WRITE_CS(0);
	if (SelectStatusRegister_1_2_3 == 1)
	{
		W25qxx_Spi(0x01);
		w25qxx.StatusRegister1 = Data;
	}
	else if (SelectStatusRegister_1_2_3 == 2)
	{
		W25qxx_Spi(0x31);
		w25qxx.StatusRegister2 = Data;
	}
	else
	{
		W25qxx_Spi(0x11);
		w25qxx.StatusRegister3 = Data;
	}
	W25qxx_Spi(Data);
	WRITE_CS(1);
}
//###################################################################################################################
void W25qxx_WaitForWriteEnd(void)
{
	__delay_ms(1);
	WRITE_CS(0);
	W25qxx_Spi(0x05);
	do
	{
		w25qxx.StatusRegister1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		__delay_ms(1);
	} while ((w25qxx.StatusRegister1 & 0x01) == 0x01);
	WRITE_CS(1);
}
//###################################################################################################################
bool W25qxx_Init(void)
{
	w25qxx.Lock = 1;
	
    __delay_ms(100);
	WRITE_CS(1);
	__delay_ms(100);
	uint32_t id;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx Init Begin...\r\n");
#endif
	id = W25qxx_ReadID();

#if (_W25QXX_DEBUG == 1)
	printf("w25qxx ID:0x%X\r\n", id);
#endif
	switch (id & 0x000000FF)
	{
	case 0x20: // 	w25q512
		w25qxx.ID = W25Q512;
		w25qxx.BlockCount = 1024;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q512\r\n");
#endif
		break;
	case 0x19: // 	w25q256
		w25qxx.ID = W25Q256;
		w25qxx.BlockCount = 512;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q256\r\n");
#endif
		break;
	case 0x18: // 	w25q128
		w25qxx.ID = W25Q128;
		w25qxx.BlockCount = 256;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q128\r\n");
#endif
		break;
	case 0x17: //	w25q64
		w25qxx.ID = W25Q64;
		w25qxx.BlockCount = 128;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q64\r\n");
#endif
		break;
	case 0x16: //	w25q32
		w25qxx.ID = W25Q32;
		w25qxx.BlockCount = 64;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q32\r\n");
#endif
		break;
	case 0x15: //	w25q16
		w25qxx.ID = W25Q16;
		w25qxx.BlockCount = 32;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q16\r\n");
#endif
		break;
	case 0x14: //	w25q80
		w25qxx.ID = W25Q80;
		w25qxx.BlockCount = 16;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q80\r\n");
#endif
		break;
	case 0x13: //	w25q40
		w25qxx.ID = W25Q40;
		w25qxx.BlockCount = 8;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q40\r\n");
#endif
		break;
	case 0x12: //	w25q20
		w25qxx.ID = W25Q20;
		w25qxx.BlockCount = 4;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q20\r\n");
#endif
		break;
	case 0x11: //	w25q10
		w25qxx.ID = W25Q10;
		w25qxx.BlockCount = 2;
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx Chip: w25q10\r\n");
#endif
		break;
	default:
#if (_W25QXX_DEBUG == 1)
		wprintf("w25qxx Unknown ID\r\n");
#endif
		w25qxx.Lock = 0;
		return false;
	}
	w25qxx.PageSize = 256;
	w25qxx.SectorSize = 0x1000;
	w25qxx.SectorCount = w25qxx.BlockCount * 16;
	w25qxx.PageCount = (w25qxx.SectorCount * w25qxx.SectorSize) / w25qxx.PageSize;
	w25qxx.BlockSize = w25qxx.SectorSize * 16;
	w25qxx.CapacityInKiloByte = (w25qxx.SectorCount * w25qxx.SectorSize) / 1024;
	W25qxx_ReadUniqID();
	W25qxx_ReadStatusRegister(1);
	W25qxx_ReadStatusRegister(2);
	W25qxx_ReadStatusRegister(3);
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx Page Size: %d Bytes\r\n", w25qxx.PageSize);
	printf("w25qxx Page Count: %d\r\n", w25qxx.PageCount);
	printf("w25qxx Sector Size: %d Bytes\r\n", w25qxx.SectorSize);
	printf("w25qxx Sector Count: %d\r\n", w25qxx.SectorCount);
	printf("w25qxx Block Size: %d Bytes\r\n", w25qxx.BlockSize);
	printf("w25qxx Block Count: %d\r\n", w25qxx.BlockCount);
	printf("w25qxx Capacity: %d KiloBytes\r\n", w25qxx.CapacityInKiloByte);
	printf("w25qxx Init Done\r\n");
#endif
	w25qxx.Lock = 0;
	return true;
}
//###################################################################################################################
void W25qxx_EraseChip(void)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	uint32_t StartTime = HAL_GetTick();
	printf("w25qxx EraseChip Begin...\r\n");
#endif
	W25qxx_WriteEnable();
	WRITE_CS(0);
	W25qxx_Spi(0xC7);
	WRITE_CS(1);
	W25qxx_WaitForWriteEnd();
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx EraseBlock done after %d ms!\r\n", HAL_GetTick() - StartTime);
#endif
	__delay_ms(10);
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_EraseSector(uint32_t SectorAddr)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	uint32_t StartTime = HAL_GetTick();
	printf("w25qxx EraseSector %d Begin...\r\n", SectorAddr);
#endif
	W25qxx_WaitForWriteEnd();
	SectorAddr = SectorAddr * w25qxx.SectorSize;
	W25qxx_WriteEnable();
	WRITE_CS(0);
	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x21);
		W25qxx_Spi((SectorAddr & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x20);
	}
	W25qxx_Spi((SectorAddr & 0xFF0000) >> 16);
	W25qxx_Spi((SectorAddr & 0xFF00) >> 8);
	W25qxx_Spi(SectorAddr & 0xFF);
	WRITE_CS(1);
	W25qxx_WaitForWriteEnd();
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx EraseSector done after %d ms\r\n", HAL_GetTick() - StartTime);
#endif
	__delay_ms(1);
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_EraseBlock(uint32_t BlockAddr)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx EraseBlock %d Begin...\r\n", BlockAddr);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	W25qxx_WaitForWriteEnd();
	BlockAddr = BlockAddr * w25qxx.SectorSize * 16;
	W25qxx_WriteEnable();
	WRITE_CS(0);
	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0xDC);
		W25qxx_Spi((BlockAddr & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0xD8);
	}
	W25qxx_Spi((BlockAddr & 0xFF0000) >> 16);
	W25qxx_Spi((BlockAddr & 0xFF00) >> 8);
	W25qxx_Spi(BlockAddr & 0xFF);
	WRITE_CS(1);
	W25qxx_WaitForWriteEnd();
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx EraseBlock done after %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	__delay_ms(1);
	w25qxx.Lock = 0;
}
//###################################################################################################################
uint32_t W25qxx_PageToSector(uint32_t PageAddress)
{
	return ((PageAddress * w25qxx.PageSize) / w25qxx.SectorSize);
}
//###################################################################################################################
uint32_t W25qxx_PageToBlock(uint32_t PageAddress)
{
	return ((PageAddress * w25qxx.PageSize) / w25qxx.BlockSize);
}
//###################################################################################################################
uint32_t W25qxx_SectorToBlock(uint32_t SectorAddress)
{
	return ((SectorAddress * w25qxx.SectorSize) / w25qxx.BlockSize);
}
//###################################################################################################################
uint32_t W25qxx_SectorToPage(uint32_t SectorAddress)
{
	return (SectorAddress * w25qxx.SectorSize) / w25qxx.PageSize;
}
//###################################################################################################################
uint32_t W25qxx_BlockToPage(uint32_t BlockAddress)
{
	return (BlockAddress * w25qxx.BlockSize) / w25qxx.PageSize;
}
//###################################################################################################################
bool W25qxx_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_PageSize)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
	if (((NumByteToCheck_up_to_PageSize + OffsetInByte) > w25qxx.PageSize) || (NumByteToCheck_up_to_PageSize == 0))
		NumByteToCheck_up_to_PageSize = w25qxx.PageSize - OffsetInByte;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckPage:%d, Offset:%d, Bytes:%d begin...\r\n", Page_Address, OffsetInByte, NumByteToCheck_up_to_PageSize);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	for (i = OffsetInByte; i < w25qxx.PageSize; i += sizeof(pBuffer))
	{
		WRITE_CS(0);
		WorkAddress = (i + Page_Address * w25qxx.PageSize);
		if (w25qxx.ID >= W25Q256)
		{
			W25qxx_Spi(0x0C);
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		}
		else
		{
			W25qxx_Spi(0x0B);
		}
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		HAL_SPI_Receive(&hspi1, pBuffer, sizeof(pBuffer), 100);
		WRITE_CS(1);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++)
		{
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((w25qxx.PageSize + OffsetInByte) % sizeof(pBuffer) != 0)
	{
		i -= sizeof(pBuffer);
		for (; i < w25qxx.PageSize; i++)
		{
			WRITE_CS(0);
			WorkAddress = (i + Page_Address * w25qxx.PageSize);
			W25qxx_Spi(0x0B);
			if (w25qxx.ID >= W25Q256)
			{
				W25qxx_Spi(0x0C);
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			}
			else
			{
				W25qxx_Spi(0x0B);
			}
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			HAL_SPI_Receive(&hspi1, pBuffer, 1, 100);
			WRITE_CS(1);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckPage is Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return true;
NOT_EMPTY:
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckPage is Not Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return false;
}
//###################################################################################################################
bool W25qxx_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_SectorSize)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
	if ((NumByteToCheck_up_to_SectorSize > w25qxx.SectorSize) || (NumByteToCheck_up_to_SectorSize == 0))
		NumByteToCheck_up_to_SectorSize = w25qxx.SectorSize;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckSector:%d, Offset:%d, Bytes:%d begin...\r\n", Sector_Address, OffsetInByte, NumByteToCheck_up_to_SectorSize);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	for (i = OffsetInByte; i < w25qxx.SectorSize; i += sizeof(pBuffer))
	{
		WRITE_CS(0);
		WorkAddress = (i + Sector_Address * w25qxx.SectorSize);
		if (w25qxx.ID >= W25Q256)
		{
			W25qxx_Spi(0x0C);
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		}
		else
		{
			W25qxx_Spi(0x0B);
		}
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		HAL_SPI_Receive(&hspi1, pBuffer, sizeof(pBuffer), 100);
		WRITE_CS(1);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++)
		{
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((w25qxx.SectorSize + OffsetInByte) % sizeof(pBuffer) != 0)
	{
		i -= sizeof(pBuffer);
		for (; i < w25qxx.SectorSize; i++)
		{
			WRITE_CS(0);
			WorkAddress = (i + Sector_Address * w25qxx.SectorSize);
			if (w25qxx.ID >= W25Q256)
			{
				W25qxx_Spi(0x0C);
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			}
			else
			{
				W25qxx_Spi(0x0B);
			}
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			HAL_SPI_Receive(&hspi1, pBuffer, 1, 100);
			WRITE_CS(1);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckSector is Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return true;
NOT_EMPTY:
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckSector is Not Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return false;
}
//###################################################################################################################
bool W25qxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
	if ((NumByteToCheck_up_to_BlockSize > w25qxx.BlockSize) || (NumByteToCheck_up_to_BlockSize == 0))
		NumByteToCheck_up_to_BlockSize = w25qxx.BlockSize;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckBlock:%d, Offset:%d, Bytes:%d begin...\r\n", Block_Address, OffsetInByte, NumByteToCheck_up_to_BlockSize);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	uint8_t pBuffer[32];
	uint32_t WorkAddress;
	uint32_t i;
	for (i = OffsetInByte; i < w25qxx.BlockSize; i += sizeof(pBuffer))
	{
		WRITE_CS(0);
		WorkAddress = (i + Block_Address * w25qxx.BlockSize);

		if (w25qxx.ID >= W25Q256)
		{
			W25qxx_Spi(0x0C);
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		}
		else
		{
			W25qxx_Spi(0x0B);
		}
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		HAL_SPI_Receive(&hspi1, pBuffer, sizeof(pBuffer), 100);
		WRITE_CS(1);
		for (uint8_t x = 0; x < sizeof(pBuffer); x++)
		{
			if (pBuffer[x] != 0xFF)
				goto NOT_EMPTY;
		}
	}
	if ((w25qxx.BlockSize + OffsetInByte) % sizeof(pBuffer) != 0)
	{
		i -= sizeof(pBuffer);
		for (; i < w25qxx.BlockSize; i++)
		{
			WRITE_CS(0);
			WorkAddress = (i + Block_Address * w25qxx.BlockSize);

			if (w25qxx.ID >= W25Q256)
			{
				W25qxx_Spi(0x0C);
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			}
			else
			{
				W25qxx_Spi(0x0B);
			}
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			HAL_SPI_Receive(&hspi1, pBuffer, 1, 100);
			WRITE_CS(1);
			if (pBuffer[0] != 0xFF)
				goto NOT_EMPTY;
		}
	}
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckBlock is Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return true;
NOT_EMPTY:
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx CheckBlock is Not Empty in %d ms\r\n", HAL_GetTick() - StartTime);
	__delay_ms(100);
#endif
	w25qxx.Lock = 0;
	return false;
}
//###################################################################################################################
void W25qxx_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	uint32_t StartTime = HAL_GetTick();
	printf("w25qxx WriteByte 0x%02X at address %d begin...", pBuffer, WriteAddr_inBytes);
#endif
	W25qxx_WaitForWriteEnd();
	W25qxx_WriteEnable();
	WRITE_CS(0);

	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x12);
		W25qxx_Spi((WriteAddr_inBytes & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x02);
	}
	W25qxx_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
	W25qxx_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
	W25qxx_Spi(WriteAddr_inBytes & 0xFF);
	W25qxx_Spi(pBuffer);
	WRITE_CS(1);
	W25qxx_WaitForWriteEnd();
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx WriteByte done after %d ms\r\n", HAL_GetTick() - StartTime);
#endif
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
	if (((NumByteToWrite_up_to_PageSize + OffsetInByte) > w25qxx.PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = w25qxx.PageSize - OffsetInByte;
	if ((OffsetInByte + NumByteToWrite_up_to_PageSize) > w25qxx.PageSize)
		NumByteToWrite_up_to_PageSize = w25qxx.PageSize - OffsetInByte;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx WritePage:%d, Offset:%d ,Writes %d Bytes, begin...\r\n", Page_Address, OffsetInByte, NumByteToWrite_up_to_PageSize);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	W25qxx_WaitForWriteEnd();
	W25qxx_WriteEnable();
	WRITE_CS(0);
	Page_Address = (Page_Address * w25qxx.PageSize) + OffsetInByte;
	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x12);
		W25qxx_Spi((Page_Address & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x02);
	}
	W25qxx_Spi((Page_Address & 0xFF0000) >> 16);
	W25qxx_Spi((Page_Address & 0xFF00) >> 8);
	W25qxx_Spi(Page_Address & 0xFF);
	HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite_up_to_PageSize, 100);
	WRITE_CS(1);
	W25qxx_WaitForWriteEnd();
#if (_W25QXX_DEBUG == 1)
	StartTime = HAL_GetTick() - StartTime;
	for (uint32_t i = 0; i < NumByteToWrite_up_to_PageSize; i++)
	{
		if ((i % 8 == 0) && (i > 2))
		{
			printf("\r\n");
			__delay_ms(10);
		}
		printf("0x%02X,", pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx WritePage done after %d ms\r\n", StartTime);
	__delay_ms(100);
#endif
	__delay_ms(1);
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize)
{
	if ((NumByteToWrite_up_to_SectorSize > w25qxx.SectorSize) || (NumByteToWrite_up_to_SectorSize == 0))
		NumByteToWrite_up_to_SectorSize = w25qxx.SectorSize;

	if (OffsetInByte >= w25qxx.SectorSize){
		return;
	}
	uint32_t StartPage;
	int32_t BytesToWrite;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToWrite_up_to_SectorSize) > w25qxx.SectorSize)
		BytesToWrite = w25qxx.SectorSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_SectorSize;
	StartPage = W25qxx_SectorToPage(Sector_Address) + (OffsetInByte / w25qxx.PageSize);
	LocalOffset = OffsetInByte % w25qxx.PageSize;
	do
	{
		W25qxx_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= w25qxx.PageSize - LocalOffset;
		pBuffer += w25qxx.PageSize - LocalOffset;
		LocalOffset = 0;
	} while (BytesToWrite > 0);
#if (_W25QXX_DEBUG == 1)
	printf("---w25qxx WriteSector Done\r\n");
	__delay_ms(100);
#endif
}
//###################################################################################################################
void W25qxx_WriteBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize)
{
	if ((NumByteToWrite_up_to_BlockSize > w25qxx.BlockSize) || (NumByteToWrite_up_to_BlockSize == 0))
		NumByteToWrite_up_to_BlockSize = w25qxx.BlockSize;
#if (_W25QXX_DEBUG == 1)
	printf("+++w25qxx WriteBlock:%d, Offset:%d ,Write %d Bytes, begin...\r\n", Block_Address, OffsetInByte, NumByteToWrite_up_to_BlockSize);
	__delay_ms(100);
#endif
	if (OffsetInByte >= w25qxx.BlockSize)
	{
#if (_W25QXX_DEBUG == 1)
		printf("---w25qxx WriteBlock Faild!\r\n");
		__delay_ms(100);
#endif
		return;
	}
	uint32_t StartPage;
	int32_t BytesToWrite;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToWrite_up_to_BlockSize) > w25qxx.BlockSize)
		BytesToWrite = w25qxx.BlockSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_BlockSize;
	StartPage = W25qxx_BlockToPage(Block_Address) + (OffsetInByte / w25qxx.PageSize);
	LocalOffset = OffsetInByte % w25qxx.PageSize;
	do
	{
		W25qxx_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
		StartPage++;
		BytesToWrite -= w25qxx.PageSize - LocalOffset;
		pBuffer += w25qxx.PageSize - LocalOffset;
		LocalOffset = 0;
	} while (BytesToWrite > 0);
#if (_W25QXX_DEBUG == 1)
	printf("---w25qxx WriteBlock Done\r\n");
	__delay_ms(100);
#endif
}
//###################################################################################################################
void W25qxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	uint32_t StartTime = HAL_GetTick();
	printf("w25qxx ReadByte at address %d begin...\r\n", Bytes_Address);
#endif
	WRITE_CS(0);

	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x0C);
		W25qxx_Spi((Bytes_Address & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x0B);
	}
	W25qxx_Spi((Bytes_Address & 0xFF0000) >> 16);
	W25qxx_Spi((Bytes_Address & 0xFF00) >> 8);
	W25qxx_Spi(Bytes_Address & 0xFF);
	W25qxx_Spi(0);
	*pBuffer = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	WRITE_CS(1);
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx ReadByte 0x%02X done after %d ms\r\n", *pBuffer, HAL_GetTick() - StartTime);
#endif
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
#if (_W25QXX_DEBUG == 1)
	uint32_t StartTime = HAL_GetTick();
	printf("w25qxx ReadBytes at Address:%d, %d Bytes  begin...\r\n", ReadAddr, NumByteToRead);
#endif
	WRITE_CS(0);

	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x0C);
		W25qxx_Spi((ReadAddr & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x0B);
	}
	W25qxx_Spi((ReadAddr & 0xFF0000) >> 16);
	W25qxx_Spi((ReadAddr & 0xFF00) >> 8);
	W25qxx_Spi(ReadAddr & 0xFF);
	W25qxx_Spi(0);
	HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, 2000);
	WRITE_CS(1);
#if (_W25QXX_DEBUG == 1)
	StartTime = HAL_GetTick() - StartTime;
	for (uint32_t i = 0; i < NumByteToRead; i++)
	{
		if ((i % 8 == 0) && (i > 2))
		{
			printf("\r\n");
			__delay_ms(10);
		}
		printf("0x%02X,", pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx ReadBytes done after %d ms\r\n", StartTime);
	__delay_ms(100);
#endif
	__delay_ms(1);
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize)
{
	while (w25qxx.Lock == 1)
		__delay_ms(1);
	w25qxx.Lock = 1;
	if ((NumByteToRead_up_to_PageSize > w25qxx.PageSize) || (NumByteToRead_up_to_PageSize == 0))
		NumByteToRead_up_to_PageSize = w25qxx.PageSize;
	if ((OffsetInByte + NumByteToRead_up_to_PageSize) > w25qxx.PageSize)
		NumByteToRead_up_to_PageSize = w25qxx.PageSize - OffsetInByte;
#if (_W25QXX_DEBUG == 1)
	printf("w25qxx ReadPage:%d, Offset:%d ,Read %d Bytes, begin...\r\n", Page_Address, OffsetInByte, NumByteToRead_up_to_PageSize);
	__delay_ms(100);
	uint32_t StartTime = HAL_GetTick();
#endif
	Page_Address = Page_Address * w25qxx.PageSize + OffsetInByte;
	WRITE_CS(0);
	if (w25qxx.ID >= W25Q256)
	{
		W25qxx_Spi(0x0C);
		W25qxx_Spi((Page_Address & 0xFF000000) >> 24);
	}
	else
	{
		W25qxx_Spi(0x0B);
	}
	W25qxx_Spi((Page_Address & 0xFF0000) >> 16);
	W25qxx_Spi((Page_Address & 0xFF00) >> 8);
	W25qxx_Spi(Page_Address & 0xFF);
	W25qxx_Spi(0);
	HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead_up_to_PageSize, 100);
	WRITE_CS(1);
#if (_W25QXX_DEBUG == 1)
	StartTime = HAL_GetTick() - StartTime;
	for (uint32_t i = 0; i < NumByteToRead_up_to_PageSize; i++)
	{
		if ((i % 8 == 0) && (i > 2))
		{
			printf("\r\n");
			__delay_ms(10);
		}
		printf("0x%02X,", pBuffer[i]);
	}
	printf("\r\n");
	printf("w25qxx ReadPage done after %d ms\r\n", StartTime);
	__delay_ms(100);
#endif
	__delay_ms(1);
	w25qxx.Lock = 0;
}
//###################################################################################################################
void W25qxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize)
{
	if ((NumByteToRead_up_to_SectorSize > w25qxx.SectorSize) || (NumByteToRead_up_to_SectorSize == 0))
		NumByteToRead_up_to_SectorSize = w25qxx.SectorSize;
#if (_W25QXX_DEBUG == 1)
	printf("+++w25qxx ReadSector:%d, Offset:%d ,Read %d Bytes, begin...\r\n", Sector_Address, OffsetInByte, NumByteToRead_up_to_SectorSize);
	__delay_ms(100);
#endif
	if (OffsetInByte >= w25qxx.SectorSize)
	{
#if (_W25QXX_DEBUG == 1)
		printf("---w25qxx ReadSector Faild!\r\n");
		__delay_ms(100);
#endif
		return;
	}
	uint32_t StartPage;
	int32_t BytesToRead;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToRead_up_to_SectorSize) > w25qxx.SectorSize)
		BytesToRead = w25qxx.SectorSize - OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_SectorSize;
	StartPage = W25qxx_SectorToPage(Sector_Address) + (OffsetInByte / w25qxx.PageSize);
	LocalOffset = OffsetInByte % w25qxx.PageSize;
	do
	{
		W25qxx_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= w25qxx.PageSize - LocalOffset;
		pBuffer += w25qxx.PageSize - LocalOffset;
		LocalOffset = 0;
	} while (BytesToRead > 0);
#if (_W25QXX_DEBUG == 1)
	printf("---w25qxx ReadSector Done\r\n");
	__delay_ms(100);
#endif
}
//###################################################################################################################
void W25qxx_ReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize)
{
	if ((NumByteToRead_up_to_BlockSize > w25qxx.BlockSize) || (NumByteToRead_up_to_BlockSize == 0))
		NumByteToRead_up_to_BlockSize = w25qxx.BlockSize;
#if (_W25QXX_DEBUG == 1)
	printf("+++w25qxx ReadBlock:%d, Offset:%d ,Read %d Bytes, begin...\r\n", Block_Address, OffsetInByte, NumByteToRead_up_to_BlockSize);
	__delay_ms(100);
#endif
	if (OffsetInByte >= w25qxx.BlockSize)
	{
#if (_W25QXX_DEBUG == 1)
		printf("w25qxx ReadBlock Faild!\r\n");
		__delay_ms(100);
#endif
		return;
	}
	uint32_t StartPage;
	int32_t BytesToRead;
	uint32_t LocalOffset;
	if ((OffsetInByte + NumByteToRead_up_to_BlockSize) > w25qxx.BlockSize)
		BytesToRead = w25qxx.BlockSize - OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_BlockSize;
	StartPage = W25qxx_BlockToPage(Block_Address) + (OffsetInByte / w25qxx.PageSize);
	LocalOffset = OffsetInByte % w25qxx.PageSize;
	do
	{
		W25qxx_ReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
		StartPage++;
		BytesToRead -= w25qxx.PageSize - LocalOffset;
		pBuffer += w25qxx.PageSize - LocalOffset;
		LocalOffset = 0;
	} while (BytesToRead > 0);
#if (_W25QXX_DEBUG == 1)
	printf("---w25qxx ReadBlock Done\r\n");
	__delay_ms(100);
#endif
}
//###################################################################################################################



#ifdef __cplusplus
}
#endif
