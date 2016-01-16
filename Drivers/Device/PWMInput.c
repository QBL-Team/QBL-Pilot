/*!
 * \file PWMInput.c
 * \author no1wudi
 */

#include "PWMInput.h"


/*!
 *@ingroup pwm_input
 *@{
 */

/*!
 *@defgroup pwm_input_private PWM Input Private
 *@{
 */

/*!
 *\brief Private struct used for capture routine
 */
typedef struct
{
    bool edge_indicater;        ///< true for Rising edge,false for Falling edge
    uint16_t rising_edge;       ///< Rising start time
    uint16_t falling_edge;      ///< Falling start time
    uint16_t pulse_wide;        ///< Pulse wide
} PWMInput_TypeDef;

static PWMInput_TypeDef input_array[8];
static TIM_HandleTypeDef * input_timer1, * input_timer2;

static void PWMInput_Process(PWMInput_TypeDef * pwm, uint16_t value)
{
    if(pwm->edge_indicater)     //Current in Rising edge
    {
        pwm->rising_edge = value;
    } else {                    //Current in Falling edge
        pwm->falling_edge = value;

        //Normal mode
        if((pwm->rising_edge < 1499) && (pwm->falling_edge < 2499))
        {
            pwm->pulse_wide = pwm->falling_edge - pwm->rising_edge;
            return;
        }

        //Ring cycled

        if((pwm->rising_edge > 1499) && (pwm->falling_edge < 1000))
        {
            pwm->pulse_wide = 2499 - pwm->rising_edge + pwm->falling_edge;
        }

    }
}

static inline void PWMInput_EdgeSwitch(PWMInput_TypeDef * pwm, TIM_HandleTypeDef * htim, uint32_t TIM_CHANNEL)
{

    if(pwm->edge_indicater)
    {
        pwm->edge_indicater = false;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else
    {
        pwm->edge_indicater = true;
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

    switch(htim->Channel)
    {
    case HAL_TIM_ACTIVE_CHANNEL_1:
        if(htim == input_timer1)
        {
            PWMInput_Process(&input_array[0], htim->Instance->CCR1);
            PWMInput_EdgeSwitch(&input_array[0], htim, TIM_CHANNEL_1);

        } else {
            PWMInput_Process(&input_array[4], htim->Instance->CCR1);
            PWMInput_EdgeSwitch(&input_array[4], htim, TIM_CHANNEL_1);
        }

        break;


    case HAL_TIM_ACTIVE_CHANNEL_2:

        if(htim == input_timer1)
        {
            PWMInput_Process(&input_array[1], htim->Instance->CCR2);
            PWMInput_EdgeSwitch(&input_array[1], htim, TIM_CHANNEL_2);

        } else {
            PWMInput_Process(&input_array[5], htim->Instance->CCR2);
            PWMInput_EdgeSwitch(&input_array[5], htim, TIM_CHANNEL_2);
        }

        break;


    case HAL_TIM_ACTIVE_CHANNEL_3:

        if(htim == input_timer1)
        {
            PWMInput_Process(&input_array[2], htim->Instance->CCR3);
            PWMInput_EdgeSwitch(&input_array[2], htim, TIM_CHANNEL_3);

        } else {
            PWMInput_Process(&input_array[6], htim->Instance->CCR3);
            PWMInput_EdgeSwitch(&input_array[6], htim, TIM_CHANNEL_3);
        }

        break;


    case HAL_TIM_ACTIVE_CHANNEL_4:
        if(htim == input_timer1)
        {
            PWMInput_Process(&input_array[3], htim->Instance->CCR4);
            PWMInput_EdgeSwitch(&input_array[3], htim, TIM_CHANNEL_4);

        } else {
            PWMInput_Process(&input_array[7], htim->Instance->CCR4);
            PWMInput_EdgeSwitch(&input_array[7], htim, TIM_CHANNEL_4);
        }
        break;
    }

}

/*!
 * @}
 */


void PWMInput_Init(TIM_HandleTypeDef * timer1, TIM_HandleTypeDef * timer2)
{
    input_timer1 = timer1;
    input_timer2 = timer2;

    //Init the array for input process

    for(uint8_t i = 0; i < 8; i++)
    {
        input_array[i].edge_indicater = true;
        input_array[i].rising_edge = 0;
        input_array[i].falling_edge = 0;
        input_array[i].pulse_wide = 0;
    }

    HAL_TIM_IC_Start_IT(input_timer1, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(input_timer1, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(input_timer1, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(input_timer1, TIM_CHANNEL_4);


    HAL_TIM_IC_Start_IT(input_timer2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(input_timer2, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(input_timer2, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(input_timer2, TIM_CHANNEL_4);
    
}

void PWMInput_GetInput(uint16_t readout[8])
{
    for(uint8_t i = 0; i < 8;i++)
    {
        readout[i] = input_array[i].pulse_wide;
    }
}

/*!
 * @}
 */

