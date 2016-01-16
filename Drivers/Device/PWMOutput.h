/*!
 * \file PWMOutput.h
 * \author no1wudi
 */

#ifndef __PWMOUTPUT_H
#define __PWMOUTPUT_H

#include "stm32f4xx_hal.h"

/*!
 *@defgroup pwm_output  PWM Output
 * @{
 */


/*!
 * \brief PWMOutput_Init    Init the pwm output module
 * \param tim1  Timer handle of first 4 channels
 * \param tim2  Timer handle of other 4 channels
 */

extern void PWMOutput_Init(TIM_HandleTypeDef * tim1,TIM_HandleTypeDef * tim2);

/*!
 * \brief PWMOutput_Apply   Apply new output for all 8 channel
 * \param output    Range from 0 ~ 21000,mapped to 1ms ~ 2ms.
 */
extern void PWMOutput_Apply(uint16_t output[8]);

/*!
 *@}
 */

#endif
