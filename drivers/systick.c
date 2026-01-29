/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the implementation functions for using
systick with interrupts
-----------------------------------------------------------------------------------
*/

#include <rp2350/m33.h>
#include <rp2350/ticks.h>
#include "interrupt.h"
#include <stdbool.h>
#include <systick.h>
#include "keypad.h"
#include <stdint.h>

#ifndef SYSTICK_FREQ
#define SYSTICK_FREQ_HZ 1000
#endif

#ifndef EXT_CLK_FREQ_HZ
#define EXT_CLK_FREQ_HZ 1000000
#endif

#define SYSTICK_TOP (EXT_CLK_FREQ_HZ/SYSTICK_FREQ_HZ - 1)

static void (* volatile CallBack)(void);
static _Bool tick_has_fired; 
static uint64_t tickCounter; 

void configure_systick() { 
	TICKS_PROC0_CYCLES = 1;
	TICKS_PROC0_CTRL = TICKS_PROC0_CTRL_ENABLE(1);
	M33_SYST_RVR = SYSTICK_TOP;
	M33_SYST_CVR = 0;
	M33_SYST_CSR = 
		M33_SYST_CSR_CLKSOURCE(0) 
		| M33_SYST_CSR_TICKINT(1) 
		| M33_SYST_CSR_ENABLE(1);
	
	
	M33_SHPR3 = (M33_SHPR3 & ~M33_SHPR3_PRI_15_3_MASK ) | M33_SHPR3_PRI_15_3(0);
}

void systick_register_callback( void (*fp)(void) ){
	CallBack = fp;
	
}

_Bool system_tick() {
	uint32_t interruptStatus;
	__get_primask(&interruptStatus);
	__disable_irq();
	_Bool retval = tick_has_fired;
	tick_has_fired = false;
	__set_primask(interruptStatus);

	return retval;
}

uint64_t get_system_time(){
	uint64_t currSystemTime; 
	uint32_t interruptStatus;

	__get_primask(&interruptStatus);
	__disable_irq();
	currSystemTime = tickCounter;
	__set_primask(interruptStatus);

	return currSystemTime;
}

void __attribute__((interrupt)) SYSTICK_Handler(){
	tick_has_fired = true; 
	tickCounter++;
	
	if (CallBack != 0){ 
		(*CallBack)(); 
	}
}
