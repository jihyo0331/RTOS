#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "stdm32f1xx_hal.h"

#include "HalGpio.h"
#include "HalUart.h"

#include "Kernel.h"
#include "MemoryMap.h"

#define SYSTEM_US_TICKS(SystemCoreClock / 1000000) // cycles per microsecond

/* Private function prototypes */
static void SystemClock_Config(void);

#ifdef LOADER
static bool CheckBootMode(void)
{
	// get HW (0,0) key pressed during keyboard power-up time.
	// while user presses HW (0,0) key, FW enters a keymap download mode or FW update mode.
		// no matter what key code mapped into the (0,0). It just check matrix (0,0).
	return KeyHw_IsPressed(0, 0);
}

sattic void Jump(uint32_t address) {
	typedef void (*pFunction)(void);

	pFunction Jump_To_Applicatiton;

	// variable that will be loaded with the satrt address of the application
	uint32_t* JumpAddress;
	const uint32_t* ApplicationAddress = (uint32_t*)address;

	// get jump address from application vector table
	JumpAddress = (uint32_t*)ApplicationAddress[1];

	// load this address into function pointer
	Jump_To_Application = (pFunction)JumpAddress;

	// Disable all interrupts
	for (int i = 0; i < 8; i++) {
		NVIC->ICER[i] = NVIC->IABR[i];
	}

	// Set interrupt vector table
	SCB->VTOR = address; /* Vector Table Relocation in Internal FLASH. */

	// Set stack pointer as in application's vector table
	__set_MSP((uint32_t)(ApplicationAddress[0]));

	// Go

}