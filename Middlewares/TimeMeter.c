/*!
 * \file TimeMeter.c
 * \author no1wudi
 */
#include "TimeMeter.h"
#include <stddef.h>

static TIM_HandleTypeDef * timer_handle = NULL;
static uint32_t timer_update_cycle = 0; ///< 定时器溢出的次数

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(timer_handle == htim)
    {
        timer_update_cycle++;
    }
}

void TimeMeter_Init(TIM_HandleTypeDef * handle)
{
    timer_handle = handle;
}

void TimeMeter_StartMeasure(void)
{
    timer_update_cycle = 0;
    timer_handle->Instance->CNT = 0;
    
    __HAL_TIM_CLEAR_FLAG(timer_handle,TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(timer_handle);
}

uint32_t TimeMeter_StopMeasure(void)
{
    uint32_t tmp = timer_handle->Instance->CNT;
    HAL_TIM_Base_Stop_IT(timer_handle);
    tmp += (timer_update_cycle * 65535);
    
    return tmp;
}    
