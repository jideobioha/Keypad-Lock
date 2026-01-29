/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an interface file that uses that has the driver declarations
for using a passive buzzer
-----------------------------------------------------------------------------------
*/

#ifndef PASSIVE_BUZZER_H
#define PASSIVE_BUZZER_H

#include <stdint.h>

#define BUZZER_NEUTRAL_NOTE_FREQ 332
#define BUZZER_INCORRECT_NOTE_FREQ 400
#define BUZZER_CORRECT_NOTE_FREQ 285
#define OK_NOTE_DURATION 200000
#define INCORRECT_NOTE_DURATION 200000
#define CORRECT_NOTE_DURATION 600000

void configure_buzzer(void);
void buzzer_play_note(uint16_t freq, uint32_t time_us);
void turn_off_buzzer(void);

#endif

