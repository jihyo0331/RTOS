/*
 * Timer.c
 *
 *  Created on: Sep 27, 2018
 *      Author: yiman
 */

#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"
#include "Kernel.h"

extern volatile Timer_t* Timer;

static void interrupt_handler(void);

static uint32_t sInternal_1ms_counter;
static uint32_t sTick_accum;

void Hal_timer_init(void)
{
    // inerface reset
    Timer->timerxcontrol.bits.TimerEn = 0;
    Timer->timerxcontrol.bits.TimerMode = 0;
    Timer->timerxcontrol.bits.OneShot = 0;
    Timer->timerxcontrol.bits.TimerSize = 0;
    Timer->timerxcontrol.bits.TimerPre = 0;
    Timer->timerxcontrol.bits.IntEnable = 1;
    Timer->timerxload = 0;
    Timer->timerxvalue = 0xFFFFFFFF;

    // set periodic mode
    Timer->timerxcontrol.bits.TimerMode = TIMER_PERIOIC;
    Timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    Timer->timerxcontrol.bits.OneShot = 0;
    Timer->timerxcontrol.bits.TimerPre = 0;
    Timer->timerxcontrol.bits.IntEnable = 1;

    uint32_t interval_1ms = TIMER_1MZ_INTERVAL / 1000;

    Timer->timerxload = interval_1ms;
    Timer->timerxcontrol.bits.TimerEn = 1;

    sInternal_1ms_counter = 0;

    // Register Timer interrupt handler
    Hal_interrupt_enable(TIMER_INTERRUPT);
    Hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT);
}

uint32_t Hal_timer_get_1ms_counter(void)
{
    return sInternal_1ms_counter;
}

static void interrupt_handler(void)
{
    sInternal_1ms_counter++;
    sTick_accum++;

    // periodically wake tasks so they don't depend on UART input
    if (sTick_accum >= 10) // ~10ms
    {
        sTick_accum = 0;
        Kernel_send_events(KernelEventFlag_Tick);
    }

    Timer->timerxintclr = 1;
}
