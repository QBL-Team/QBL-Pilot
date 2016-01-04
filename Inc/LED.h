#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

enum LED_COLOR{
    LED_COLOR_OFF = 0,
    LED_COLOR_WHITE,
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_FUCHSIN,
    LED_COLOR_YELLOW,
    LED_COLOR_CYAN,
};

extern void LED_Show(uint8_t LED_COLOR);

#endif
