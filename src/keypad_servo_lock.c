/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an application file that uses a servo motor, keypad lock, piezo buzzer
to implement a smart 4-pin password protected lock system
-----------------------------------------------------------------------------------
*/
#include "systick.h"
#include "keypad.h"
#include "servo.h"
#include "led.h"
#include "passive_buzzer.h"
#include "watchdog.h"
#include "timer0.h"
#include "usbcdc.h" 
#include "interrupt.h"
#include <stdint.h>

#define NOT_A_NUMBER 99
#define PASSCODE_LENGTH 4


void SmartLock_FSM(char);
int8_t char_to_int(char);

int main(void){

    __disable_irq();
    configure_systick();
    configure_keypad();
    configure_led();
    configure_servo();
    configure_buzzer();
    configure_watchdog(2000);
    configure_timer0();
    configure_usbcdc();
    __enable_irq();

    char keypad_char;
    rotate_servo(LOCKED_POSITION);
    while(1){
        __wait_for_interrupt();
        if ( !system_tick() ){
            continue;
        }

        if (keypad_getchar( &keypad_char) ){
            SmartLock_FSM(keypad_char);
            usbcdc_putchar(keypad_char);
        }

        feed_the_watchdog();
    }

    return (0);
}

void SmartLock_FSM(char keypad_key){
    static enum {IDLE, FIRST_HASH, SET_NEW_CODE, HAVE_1_DIGIT, HAVE_2_DIGITS, HAVE_3_DIGITS, HAVE_4_DIGITS} state;
    static uint32_t password;
    static uint32_t passwordAttempt;
    static uint32_t newPassword;
    static uint8_t newPasswordCntr;
    static uint8_t digitCntr;
    
    int8_t keypad_num = char_to_int(keypad_key);

    switch(state){
        case IDLE:
            newPassword = 0;
            newPasswordCntr = 0;
            digitCntr = 0;
            passwordAttempt = 0;
            turn_off_led();
            usbcdc_putchar('L');
            if (keypad_key == '#') { 
                state = FIRST_HASH;
            }else if (keypad_num != NOT_A_NUMBER) {
                passwordAttempt = ( (passwordAttempt << 8) | ((uint32_t)keypad_num) );
                digitCntr++;
                state = HAVE_1_DIGIT;
            }
            break;
        
        case FIRST_HASH:
            if (keypad_key != '#'){
                state = IDLE;
            } else {
                state = SET_NEW_CODE;
            }
            break;
        
        case SET_NEW_CODE:
            turn_on_led();
            if (keypad_num != NOT_A_NUMBER && newPasswordCntr < PASSCODE_LENGTH){
                newPassword = ( (newPassword << 8) | (uint32_t )(keypad_num) );
                newPasswordCntr++;
            } else{
                if (newPasswordCntr >= PASSCODE_LENGTH && keypad_key == '#'){
                    buzzer_play_note(BUZZER_INCORRECT_NOTE_FREQ, INCORRECT_NOTE_DURATION);
                    state = IDLE;
                } else if (newPasswordCntr >= PASSCODE_LENGTH && keypad_key == '*'){
                    buzzer_play_note(BUZZER_NEUTRAL_NOTE_FREQ, OK_NOTE_DURATION);
                    password = newPassword;
                    usbcdc_putchar('C');
                    state = IDLE;
                } else {
                    state = IDLE;
                }
            }
            break;

        case HAVE_1_DIGIT: 
            if (keypad_num != NOT_A_NUMBER){
                passwordAttempt = ( (passwordAttempt << 8) | ((uint32_t)keypad_num) );
                digitCntr++;
                state = HAVE_2_DIGITS;
            } else{
                state = IDLE;
            }
            break;
        
        case HAVE_2_DIGITS:
            if (keypad_num != NOT_A_NUMBER){
                passwordAttempt = ( (passwordAttempt << 8) | ((uint32_t)keypad_num) );
                digitCntr++;
                state = HAVE_3_DIGITS;
                
            } else{
                state = IDLE;
            }
            break;
        
        case HAVE_3_DIGITS:
            if (keypad_num != NOT_A_NUMBER){
                passwordAttempt = ( (passwordAttempt << 8) | ((uint32_t)keypad_num) );
                digitCntr++;
                state = HAVE_4_DIGITS;
                
            } else{
                state = IDLE;
            }
            break;
        
        case HAVE_4_DIGITS:
            if (keypad_num == NOT_A_NUMBER && keypad_key == '*' && digitCntr == 4){
                if (passwordAttempt == password){
                    buzzer_play_note(BUZZER_CORRECT_NOTE_FREQ, CORRECT_NOTE_DURATION);
                    servo_timed_unlock();
                    usbcdc_putchar('U');
                 
                } else{
                    buzzer_play_note(BUZZER_INCORRECT_NOTE_FREQ, INCORRECT_NOTE_DURATION);

                } 
            }

            state = IDLE;  
            break;
    }

}

int8_t char_to_int(char num){
    if (num >= '0' && num <= '9'){
        return num - '0';
    }

    return NOT_A_NUMBER;
}