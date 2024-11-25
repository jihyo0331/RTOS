#include "armcpu.h"

void enable_irq(void)
{
	__asm__ ("cpsie i");
}

void enable_fiq(void)
{

}

void disable_irq(void)
{
	__asm__ ("cpsid i");
}

void disable_fiq(void)
{

}
