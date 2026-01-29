/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the implementation functions for using
the servo motor
-----------------------------------------------------------------------------------
*/

#include "servo.h"
#include <rp2350/resets.h>
#include <rp2350/pads_bank0.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pwm.h> 
#include <rp2350/sio.h>
#include <stdint.h> 
#include "timer0.h"

#define CONCAT2(X, Y) X ## Y
#define CONCAT3(X, Y, Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO, X, _CTRL)
#define PADS_BANK0_GPIO(X) CONCAT2(PADS_BANK0_GPIO, X)

#define SERVO_RESET (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK | RESETS_RESET_PWM_MASK)

#define SERVO_OFFSET -100
#define SERVO_MAX (2500 + SERVO_OFFSET) // would this break if (2500 + SERVO_OFFSET) >= 50,000? no, you would just have a 100% duty
#define SERVO_ZERO (1500 + SERVO_OFFSET)
#define SERVO_MIN (500 + SERVO_OFFSET)
#define MOTOR_PIN 20 

static int16_t currentServoPosition;

void configure_servo(void){

    RESETS_RESET_CLR = SERVO_RESET;
    while ((RESETS_RESET_DONE & SERVO_RESET) != SERVO_RESET) { continue; }

    PADS_BANK0_GPIO(MOTOR_PIN) = 
        PADS_BANK0_GPIO0_OD(0) |
        PADS_BANK0_GPIO0_IE(0) |
        PADS_BANK0_GPIO0_DRIVE(0) |
        PADS_BANK0_GPIO0_PUE(0) |
        PADS_BANK0_GPIO0_PDE(0) |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0);
    
    IO_BANK0_GPIO_CTRL(MOTOR_PIN) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0) |
        IO_BANK0_GPIO0_CTRL_INOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)  |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0) |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(4); 
    
    PWM_CH2_DIV = PWM_CH2_DIV_INT(144) | PWM_CH2_DIV_FRAC(0);
    PWM_CH2_TOP = 49999;
    rotate_servo(0); 
    PWM_CH2_CSR = 
        PWM_CH2_CSR_PH_ADV(0)  |
        PWM_CH2_CSR_PH_RET(0)  |
        PWM_CH2_CSR_DIVMODE(0) |
        PWM_CH2_CSR_B_INV(0)   |
        PWM_CH2_CSR_A_INV(0)   |
        PWM_CH2_CSR_PH_CORRECT(0) |
        PWM_CH2_CSR_EN(1);

}

void rotate_servo(int16_t rotate){ 
    int32_t duty = SERVO_ZERO + rotate; 

    if (duty > SERVO_MAX) {
        duty = SERVO_MAX;
    } else if (duty < SERVO_MIN){
        duty = SERVO_MIN;
    }
    currentServoPosition = duty;
    PWM_CH2_CC = PWM_CH2_CC_A(currentServoPosition);
}

int16_t get_servo_position(void){
    return currentServoPosition - SERVO_ZERO; 
                                              
}

void servo_timed_unlock(){
    rotate_servo(UNLOCKED_POSITION);
    set_alarm(1, 5000000, &lock_servo, 1);
}

void lock_servo(){
    rotate_servo(LOCKED_POSITION);
}