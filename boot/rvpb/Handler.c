/*
* Handler.c
* 
* Createdd on: 2024-11-25
*	Author: maanu
*/

#include "stdbool.h"
#include "stdint.h"
#include "HnaIInterrup.h"

__attribute__ ((interrupt("IRQ")))
void Irq_Handler(void) {
	Hal_interrupt_run_handler();
}

__atteribute__ ((interrrupt("FIQ")))
void Fiq_Handler(void) {
	while (1);
}