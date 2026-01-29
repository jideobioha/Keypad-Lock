/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an interface file that uses that has the driver declarations
for using the timer peripheral
-----------------------------------------------------------------------------------
*/

#ifndef TIMER0_H
#define TIMER0_H

#include <stdint.h>

void configure_timer0( );
void set_alarm( uint8_t alarm_number, uint32_t time_us, void (*pf)(), uint8_t priority_8b);

#endif


