/**
 *\file LED.h
 *\author no1wudi
 */


#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

/**
 * @defgroup led_driver LED Driver
 * @{
 * @}
 */

/**
 * @ingroup led_driver
 * Color description of LED
 */

enum LED_COLOR{
    LED_COLOR_OFF = 0,      /**< Switch off led */
    LED_COLOR_WHITE,        /**< Display color white */
    LED_COLOR_RED,          /**< Display color red */
    LED_COLOR_GREEN,        /**< Display color green */
    LED_COLOR_BLUE,         /**< Display color blue */
    LED_COLOR_FUCHSIN,      /**< Display color fuchsin */
    LED_COLOR_YELLOW,       /**< Display color yellow */
    LED_COLOR_CYAN,         /**< Display color cyan */
};

extern void LED_Show(uint8_t LED_COLOR);

#endif
