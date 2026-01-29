/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the application file for testing
a passive buzzer
-----------------------------------------------------------------------------------
*/

#include "systick.h"
#include "timer0.h"
#include "passive_buzzer.h"
#include "interrupt.h"
#include <stdint.h>

void Buzzer_FSM(void);

int main(void){

    __disable_irq();
    configure_systick();
    configure_buzzer();
    configure_timer0();
    __enable_irq();

    while (1){

        if ( !system_tick() ){
            continue;
        }

        Buzzer_FSM();
    }

    return (0);
}

void Buzzer_FSM(void){
    static uint16_t cntr;

    cntr = cntr == ( 3000 - 1 ) ? 0 : cntr + 1;

    switch (cntr){
        case 0:
        buzzer_play_note(285, 400000);
        break;

        case 1000: 
        buzzer_play_note(332, 400000);
        break;

        case 2000:
        buzzer_play_note(400, 400000); 
        break;
    }
}