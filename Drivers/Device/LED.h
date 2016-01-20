/**
 *\file LED.h
 *\author no1wudi
 */


#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

/**
 * @defgroup led_driver LED 驱动
 * @{
 * @}
 */

/**
 * @ingroup led_driver
 * 颜色描述符
 */

enum LED_COLOR{
    LED_COLOR_OFF = 0,          /**< 关闭显示 */
    LED_COLOR_WHITE = 1,        /**< 显示白色 */
    LED_COLOR_RED = 2,          /**< 显示红色 */
    LED_COLOR_GREEN = 3,        /**< 显示绿色 */
    LED_COLOR_BLUE = 4,         /**< 显示蓝色 */
    LED_COLOR_FUCHSIN = 5,      /**< 显示品红 */
    LED_COLOR_YELLOW = 6,       /**< 显示黄色 */
    LED_COLOR_CYAN = 7,         /**< 显示青色 */
};

extern void LED_Show(uint8_t LED_COLOR);

#endif
