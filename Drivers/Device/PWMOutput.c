/*!
 * \file PWMOutput.c
 * \author no1wudi
 */

#include "PWMOutput.h"


/*!
 *@ingroup pwm_output
 *@{
 */
#define PWMOUTPUT_BASE  20999   ///< Base value of output

static TIM_HandleTypeDef * pwm_out_handle1 = NULL,* pwm_out_handle2 = NULL;

void PWMOutput_Init(TIM_HandleTypeDef * tim1,TIM_HandleTypeDef * tim2)
{
    pwm_out_handle1 = tim1;
    pwm_out_handle2 = tim2;
    
    HAL_TIM_OC_Start(pwm_out_handle1,TIM_CHANNEL_1);
    HAL_TIM_OC_Start(pwm_out_handle1,TIM_CHANNEL_2);
    HAL_TIM_OC_Start(pwm_out_handle1,TIM_CHANNEL_3);
    HAL_TIM_OC_Start(pwm_out_handle1,TIM_CHANNEL_4);
    
    
    HAL_TIM_OC_Start(pwm_out_handle2,TIM_CHANNEL_1);
    HAL_TIM_OC_Start(pwm_out_handle2,TIM_CHANNEL_2);
    HAL_TIM_OC_Start(pwm_out_handle2,TIM_CHANNEL_3);
    HAL_TIM_OC_Start(pwm_out_handle2,TIM_CHANNEL_4);
}

void PWMOutput_Apply(uint16_t output[8])
{
    pwm_out_handle1->Instance->CCR1 = output[0] + PWMOUTPUT_BASE;
    pwm_out_handle1->Instance->CCR2 = output[1] + PWMOUTPUT_BASE;
    pwm_out_handle1->Instance->CCR3 = output[2] + PWMOUTPUT_BASE;
    pwm_out_handle1->Instance->CCR4 = output[3] + PWMOUTPUT_BASE;
    
    pwm_out_handle2->Instance->CCR1 = output[4] + PWMOUTPUT_BASE;
    pwm_out_handle2->Instance->CCR2 = output[5] + PWMOUTPUT_BASE;
    pwm_out_handle2->Instance->CCR3 = output[6] + PWMOUTPUT_BASE;
    pwm_out_handle2->Instance->CCR4 = output[7] + PWMOUTPUT_BASE;
    
    
}

/*!
 *
 * @}
 */
