/*!
 * \file MS5611.h
 * \author no1wudi
 */
 
#ifndef __MS5611_H
#define __MS5611_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern bool MS5611_Init(SPI_HandleTypeDef * handle);
extern bool MS5611_Update(float * Pressure,float * Temperature);

#endif
