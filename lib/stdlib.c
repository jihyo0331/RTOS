/*
 * stdlib.c
 *
 *  Created on: Sep 27, 2018
 *      Author: yiman
 */

#include "stdint.h"
#include "stdbool.h"
#include "HalTimer.h"

void* memset(void* dst, int value, uint32_t count)
{
    uint8_t* d = (uint8_t*)dst;
    while (count--)
    {
        *d++ = (uint8_t)value;
    }
    return dst;
}

void delay(uint32_t ms)
{
    uint32_t goal = Hal_timer_get_1ms_counter() + ms;

    // cooperative wait: spin until timer reaches goal
    while(Hal_timer_get_1ms_counter() - goal != 0);
}

void memclr(void* dst, uint32_t count)
{
    uint8_t* d = (uint8_t*)dst;

    while(count--)
    {
        *d++ = 0;
    }
}
