/*!
 * \file TimeMeter.h
 * \author no1wudi
 */

#ifndef __TIMEMETER_H
#define __TIMEMETER_H

#include "stm32f4xx_hal.h"


/*!
 *@defgroup time_meter Timer Meter
 *@{
 */

/*!
 * \brief TimeMeter_Init    初始化定时器模块
 * \param handle    用于测量时间的定时器句柄
 */

extern void TimeMeter_Init(TIM_HandleTypeDef * handle);

/*!
 * \brief TimeMeter_StartMeasure 启动测量
 */

extern void TimeMeter_StartMeasure(void);

/*!
 * \brief TimeMeter_StopMeasure 停止测量并返回测量值
 * \return 函数 TimeMeter_StartMeasure() 被执行后到此刻为止经历的时间,数值的单位为0.25us,如果返回1000，那么真实事件为1000 * 0.25us = 250us
 */
extern uint32_t TimeMeter_StopMeasure(void);

/*!
 *@}
 */


#endif
