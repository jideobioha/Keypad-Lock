/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the implementation functions for using
the a passive buzzer (e.g a passive piezoelectric buzzer)
-----------------------------------------------------------------------------------
*/

#include "passive_buzzer.h"
#include "timer0.h"
#include <rp2350/resets.h>
#include <rp2350/pads_bank0.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pwm.h> 
#include <rp2350/sio.h>
#include <stdint.h> 
#include <stdbool.h>

#define CONCAT2(X, Y) X ## Y
#define CONCAT3(X, Y, Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO, X, _CTRL)
#define PADS_BANK0_GPIO(X) CONCAT2(PADS_BANK0_GPIO, X)
#define BUZZER_RESET (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK | RESETS_RESET_PWM_MASK)
#define BUZZER_PIN 22 

void configure_buzzer(void){
    RESETS_RESET_CLR = BUZZER_RESET;
    while ((RESETS_RESET_DONE & BUZZER_RESET) != BUZZER_RESET) { continue; }

    PADS_BANK0_GPIO(BUZZER_PIN) = 
        PADS_BANK0_GPIO0_OD(0) |
        PADS_BANK0_GPIO0_IE(0) |
        PADS_BANK0_GPIO0_DRIVE(0) |
        PADS_BANK0_GPIO0_PUE(0) |
        PADS_BANK0_GPIO0_PDE(0) |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0);
    
    IO_BANK0_GPIO_CTRL(BUZZER_PIN) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0) |
        IO_BANK0_GPIO0_CTRL_INOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0) |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(4); 
    
    PWM_CH3_DIV = PWM_CH3_DIV_INT(144) | PWM_CH2_DIV_FRAC(0);
    // PWM_CH3_TOP = 0; dont have to write anything to this register since it get sets when play_note is called

    PWM_CH3_CSR = 
        PWM_CH3_CSR_PH_ADV(0)  |
        PWM_CH3_CSR_PH_RET(0)  |
        PWM_CH3_CSR_DIVMODE(0) |
        PWM_CH3_CSR_B_INV(0)   |
        PWM_CH3_CSR_A_INV(0)   |
        PWM_CH3_CSR_PH_CORRECT(0) |
        PWM_CH3_CSR_EN(1);
}

void buzzer_play_note(uint16_t freq, uint32_t time_us){

    PWM_CH3_TOP = freq;
    PWM_CH3_CC = PWM_CH3_CC_A(freq / 2);
    set_alarm(0, time_us, &turn_off_buzzer, 1);
}

void turn_off_buzzer(void){
    PWM_CH3_CC = 0;
}




