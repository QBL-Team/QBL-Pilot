/*!
 * \file PWMInput.h
 * \author no1wudi
 */
#ifndef __PWMINPUT_H
#define __PWMINPUT_H


#include "stm32f4xx_hal.h"
#include <stdbool.h>
/*!
 *@defgroup pwm_input  PWM Input 驱动
 * @{
 */
 
/*!
 * \brief PWMInput_Init 初始化定时器用于捕获脉冲输入
 * \param timer1    用于捕获的定时器1
 * \param timer2    用于捕获的定时器2
 */
extern void PWMInput_Init(TIM_HandleTypeDef * timer1,TIM_HandleTypeDef * timer2);

/*!
 * \brief PWMInput_GetInput 获取捕获到的脉冲宽度
 * \param readout 读取到值的缓冲数组，数值的单位为us，元素0~3为定时器1通道1~4的输入脉宽，元素4~7为定时器2通道1~4的输入脉宽
 */

extern void PWMInput_GetInput(uint16_t readout[8]);
 
/*!
 *@}
 */

#endif
