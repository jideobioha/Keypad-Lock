/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an interface file that uses that has the driver declarations
for using the keypad peripheral
-----------------------------------------------------------------------------------
*/

#ifndef KEYPAD_H
#define KEYPAD_H

typedef enum keypad_scan_t {WAIT_PRESS, SCAN0, SCAN1, SCAN2, SCAN3, WAIT_RELEASE} keypad_scan_t; 

void configure_keypad(void);
void task_keypad_scan(void);
int keypad_getchar(char * );

#endif