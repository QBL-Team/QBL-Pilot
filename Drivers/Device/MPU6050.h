/*!
 *
 * \file   MPU6050.h
 * \author no1wudi
 */

#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern bool MPU6050_Init(I2C_HandleTypeDef * handle);
extern bool MPU6050_Update(float axis[6],float * temp);

#endif
