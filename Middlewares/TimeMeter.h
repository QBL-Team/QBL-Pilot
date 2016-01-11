/*!
 * \file TimeMeter.h
 * \author no1wudi
 */

#ifndef __TIMEMETER_H
#define __TIMEMETER_H

#include "stm32f4xx_hal.h"

/*!
 * @defgroup time_meter Time Meter
 * @{
 */

/*!
 * \brief TimeMeter_Init    Initialize the time meter module
 * \param handle    Timer instance to be used for measure
 */

extern void TimeMeter_Init(TIM_HandleTypeDef * handle);

/*!
 * \brief TimeMeter_StartMeasure Start time measure
 */

extern void TimeMeter_StartMeasure(void);

/*!
 * \brief TimeMeter_StopMeasure Stop measure and get time
 * \return  Time after TimeMeter_StartMeasure() called,unit in 0.25 us
 */
extern uint32_t TimeMeter_StopMeasure(void);

/*!
 *@}
 */


#endif
