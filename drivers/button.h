/*
----------------------------------------------------------------------------------------
 Jidechukwu Obioha
 This file contains the declarations for the functions that will be used in button.c
---------------------------------------------------------------------------------------
*/

#ifndef BUTTON_H
#define BUTTON_H

typedef enum button_press_t {NO_PRESS, SHORT_PRESS, PRESS, LONG_PRESS} button_press_t;

void configure_button(void);
_Bool button_is_down(void);
_Bool button_is_up(void);
button_press_t get_button_press(void); // this function has the implementation for an FSM that debounces and detects button press 
				       // durations

#endif
