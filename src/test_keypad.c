/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file is an application file that uses systick interrupts
to scan keypad presses and prints them to minicom
-----------------------------------------------------------------------------------
*/

#include "keypad.h"
#include "systick.h"
#include "usbcdc.h"
#include "interrupt.h"

int main(void){

    __disable_irq();
    configure_keypad();
    configure_systick();
    configure_usbcdc();
    __enable_irq();

    char keypad_key;
    while (1){
        __wait_for_interrupt(); 
        if ( !system_tick() ){
            continue; 
        }

        if ( keypad_getchar(&keypad_key) ){
            usbcdc_putchar(keypad_key);
        }
    }

    return (0);
}

