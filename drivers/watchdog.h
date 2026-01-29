/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the interface for using functions for the 
computer operating properly (COP) watchdog peripheral
-----------------------------------------------------------------------------------
*/

#ifndef WATCHDOG_H
#define WATCHDOG_H
#include <stdint.h>

void configure_watchdog( uint32_t reload);
void feed_the_watchdog();

#endif


