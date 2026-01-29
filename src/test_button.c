/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an application file that uses a system tick to create a periodic
main loop where an FSM is executed can detect button presses and the duration of 
the presses
-----------------------------------------------------------------------------------
*/

#include "button.h"
#include "systick.h"
#include "led.h"
#include <stdint.h>

void LED_FSM(button_press_t press_type);

int main(void){
	
	configure_systick();
	configure_button();
	configure_led();
	
	button_press_t press_type; 
	
	while(1){
		if ( !system_tick() ){ continue; }
		
		press_type = get_button_press();
		LED_FSM(press_type);
	}

	return (0);
}

void LED_FSM(button_press_t press_type){
	static uint16_t ledCntr;
	static enum {IDLE, BUTTON_WAS_SHORT_PRESSED, BUTTON_WAS_PRESSED, BUTTON_WAS_LONG_PRESSED} state;
	
	switch(state){
	case IDLE: // technically don't even need idle because off state (short_press) is the idle state
		if (press_type == SHORT_PRESS){
			turn_off_led();
			state = BUTTON_WAS_SHORT_PRESSED;
		} else if (press_type == PRESS){
			turn_on_led();
			state = BUTTON_WAS_PRESSED;
		} else if (press_type == LONG_PRESS){
			ledCntr = 0;
			state = BUTTON_WAS_LONG_PRESSED;
		}
		break;
	case BUTTON_WAS_SHORT_PRESSED:
		state = IDLE;
		break;
	case BUTTON_WAS_PRESSED:
		state = IDLE;
		break;
	case BUTTON_WAS_LONG_PRESSED:
		ledCntr++;
		if (ledCntr >= 1000){
			state = IDLE;
		}
		switch(ledCntr){
		case 0:
			turn_off_led();
			break;
		case 250: 
			turn_on_led();
			break;
		case 750: 
			turn_off_led();
			break;
		
		}
		break;
	}
}

