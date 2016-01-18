/*!
 * \file W25QXX.h
 * \author no1wudi
 */
 
#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern bool W25Q_Init(SPI_HandleTypeDef * handle);
extern bool W25Q_CheckForIdle(void);
extern bool W25Q_EraseSector(uint32_t addr);
extern bool W25Q_EraseChip(void);
extern bool W25Q_Write(uint32_t addr,uint8_t * buffer,uint16_t length);
extern bool W25Q_Read(uint32_t addr,uint8_t * buffer,uint16_t length);

#endif
