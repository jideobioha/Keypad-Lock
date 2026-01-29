/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an interface file that uses that has the driver declarations
for using the servo motor
-----------------------------------------------------------------------------------
*/
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define LOCKED_POSITION -1000
#define UNLOCKED_POSITION 1000

void configure_servo(void);
void rotate_servo(int16_t);
int16_t get_servo_position(void);

void servo_timed_unlock(void);
void lock_servo(void);

#endif
