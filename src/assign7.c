/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an application file that uses the keypad to control the movements of 
a servo motor
-----------------------------------------------------------------------------------
*/
#include "usbcdc.h"
#include "servo.h"
#include "systick.h"
#include "interrupt.h"
#include <stdint.h>
#define STEP_SIZE 100

void servo_FSM(char key);

int main(void){

    __disable_irq();
    configure_usbcdc();
    configure_systick();
    configure_servo();
    __enable_irq();

    char usbcdc_char;
    while (1){
        __wait_for_interrupt();
        if ( !system_tick() ){
            continue;
        }

        if ( usbcdc_getchar(&usbcdc_char) ){ // asynchronous (event-driven) FSM
            
            servo_FSM(usbcdc_char);
        }
    }
    return (0);
}

void servo_FSM(char key){
    int16_t currServoPos = get_servo_position();
    int16_t desiredPos;

    switch(key){
        case 'l':
        desiredPos = currServoPos - (1 * STEP_SIZE);
        rotate_servo(desiredPos);
        break;

        case 'L':
        desiredPos = currServoPos - (2 * STEP_SIZE);
        rotate_servo(desiredPos);
        break;

        case 'r':
        desiredPos = currServoPos + (1 * STEP_SIZE);
        rotate_servo(desiredPos);
        break;

        case 'R':
        desiredPos = currServoPos + (2 * STEP_SIZE);
        rotate_servo(desiredPos);
        break;

        case 'c':
        rotate_servo(0);
        break;
    }

}
