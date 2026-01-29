/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an application file that uses a system tick to create a periodic
main loop where each time the systick fires, an interrupt fires that runs an isr
that flashes the LED at a 1Hz frequency
-----------------------------------------------------------------------------------
*/

#include "systick.h"
#include "led.h"
#include "interrupt.h"
#include "usbcdc.h"
#include <stdint.h>

#define FLASH_LED_TOP 1000
#define CHAR_CNTR_TOP 4

void LED_callback(void);
const char charsToPrint[] = {'+', '-', '*', '/'};

int main(void){

    // need to disable interrupts while configuring drivers
    __disable_irq();
    configure_systick();
    configure_led();
    configure_usbcdc();
    __enable_irq(); 

    systick_register_callback(&LED_callback);
    uint8_t charCntr = 0;
    char printThisChar;
    uint64_t lastTime = get_system_time();

    while(1){

        __wait_for_interrupt();
        if ( !system_tick() ){  continue;  }

        if (get_system_time() - lastTime >= 1000){

            if (charCntr == CHAR_CNTR_TOP - 1){
                charCntr = 0;
            } else {
                charCntr++;
            }

            printThisChar = charsToPrint[charCntr];
            usbcdc_putchar(printThisChar);
            lastTime += 1000;
        }
    }

    return (0);
}


// this function's address will be passed to another function so that it will be registered
// as a callback for when the timer interrupt fires. So this function will run when the
// the timer interrupt fires
void LED_callback(){
    static uint16_t LEDcntr;
    
    if (LEDcntr == FLASH_LED_TOP - 1){
        LEDcntr = 0;
    } else {
        LEDcntr++;
    }

    switch(LEDcntr){
        case 0:
        turn_on_led();
        break;

        case 500:
        turn_off_led();
        break;
    }

}