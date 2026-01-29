/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an interface file that uses that has the driver declarations
for using systick with interrupts
-----------------------------------------------------------------------------------
*/

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>
#include <stdint.h>

void configure_systick();
_Bool system_tick();
void systick_register_callback( void (*fp)(void) ); 
uint64_t get_system_time(void);

#endif

