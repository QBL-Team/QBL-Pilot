/*!
 * \file PWMInput.h
 * \author no1wudi
 */
#ifndef __PWMINPUT_H
#define __PWMINPUT_H


#include "stm32f4xx_hal.h"
#include <stdbool.h>
/*!
 *@defgroup pwm_input  PWM Input
 * @{
 */
 
/*!
 * \brief PWMInput_Init Init timer for caputre
 * \param timer1    Capture timer 1
 * \param timer2    Capture timer 2
 */
extern void PWMInput_Init(TIM_HandleTypeDef * timer1,TIM_HandleTypeDef * timer2);

/*!
 * \brief PWMInput_GetInput Get capture value
 * \param readout   Unit in us,Array for capture value,0~3 mapped to timer1's channel 1 ~ channel 4;4 ~ 7 mapped to timer2's channel 1 ~ channel 4
 */

extern void PWMInput_GetInput(uint16_t readout[8]);
 
/*!
 *@}
 */

#endif
