/*!
 *
 * \file   HMC5883.h
 * \author no1wudi
 */

#ifndef __HMC5883_H
#define __HMC5883_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern bool HMC5883_Init(I2C_HandleTypeDef * handle);
extern bool HMC5883_Update(float axis[3]);

#endif
