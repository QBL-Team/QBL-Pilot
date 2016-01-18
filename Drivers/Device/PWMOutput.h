/*!
 * \file PWMOutput.h
 * \author no1wudi
 */

#ifndef __PWMOUTPUT_H
#define __PWMOUTPUT_H

#include "stm32f4xx_hal.h"

/*!
 *@defgroup pwm_output  PWM Output 驱动
 * @{
 */


/*!
 * \brief PWMOutput_Init    初始化定时器，用于PWM脉冲输出
 * \param tim1  用于输出的定时器1
 * \param tim2  用于输出的定时器2
 */

extern void PWMOutput_Init(TIM_HandleTypeDef * tim1,TIM_HandleTypeDef * tim2);

/*!
 * \brief PWMOutput_Apply   应用输出到定时器的各通道
 * \param output    范围为0 ~ 21000,对应着脉冲宽度1ms ~ 2ms，元素0~3对应定时器1的通道1到通道4，元素4~7对应定时器2的通道1到通道4
 */
extern void PWMOutput_Apply(uint16_t output[8]);

/*!
 *@}
 */

#endif
