/*
--------------------------------------------------------------------------------------
Jidechukwu Obioha
This file contains the implemnetation of the button functions declared in button.h 
Using pin 7 for the button
--------------------------------------------------------------------------------------
*/
#include "button.h"
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include "gpio_irq.h"
#include <stdbool.h>
#include <stdint.h>


#define BUTTON_RESETS (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)
#define DEB_PERIOD 5

void configure_button( void )
{
    RESETS_RESET_CLR = BUTTON_RESETS; // RESETS_RESET_CLR is the atomic register for the regular RESETS_RESET. This register
    				      // has all bits defaulting to zero, and you write a 1 to the bits you want to pull out of reset.
    				      // if we were resetting from the RESETS_RESET, reg, you would follow its rules, where all bits in
    				      // in that reg default to 1 and you write a zero to the bits you want to reset. 
    				      
    while(! (RESETS_RESET_DONE & BUTTON_RESETS) )
	    continue;

     PADS_BANK0_GPIO7 =
	PADS_BANK0_GPIO7_OD(0) 
	| PADS_BANK0_GPIO7_IE(1) 
	| PADS_BANK0_GPIO7_DRIVE(0) 
	| PADS_BANK0_GPIO7_PUE(1) 
	| PADS_BANK0_GPIO7_PDE(0) 
	| PADS_BANK0_GPIO7_SCHMITT(0) 
	| PADS_BANK0_GPIO7_SLEWFAST(0);
    IO_BANK0_GPIO7_CTRL = 
	IO_BANK0_GPIO7_CTRL_IRQOVER(0) |
	IO_BANK0_GPIO7_CTRL_INOVER(0)  |
	IO_BANK0_GPIO7_CTRL_OEOVER(0)  |
	IO_BANK0_GPIO7_CTRL_OUTOVER(0) |
	IO_BANK0_GPIO7_CTRL_FUNCSEL(5);
	
	// don't need to enable sio for inputs, only do that for outputs
	
}

_Bool button_is_down(void){
	// button is active low -> when the button is in the active state (pressed), it outputs a zero
	_Bool button_state = READ_SIO_GPIO_IN(7);
	return !button_state; // if button is pressed (outputting 0), we return !0 (true). 
			      // if button is unpressed (outputting 1), we return !1 (false)
}

_Bool button_is_up(void){
	return !button_is_down();
}

button_press_t get_button_press(void){

	static uint8_t deb_cntr;
	static uint16_t state_cntr;
	static button_press_t state;
	button_press_t retval = NO_PRESS; // initialize retval to NO_PRESS and not state, because retval is supposed to be edge-sensitive
					  // it is only supposed to change when state changes. initializing it to state here would make it
					  // level sensitive (not what we want)
					  // if you wanted to know what current state (level) button is in, you would need another variable
	state_cntr++; //put state_cntr here becuase we want to increment this every cycle not every 5 cycles (debounce period)
	
	if (deb_cntr == DEB_PERIOD - 1){
		deb_cntr = 0;
	} else {
		deb_cntr++; 
		return retval;
	}
	
	switch(state){
	case NO_PRESS:
		if (button_is_down()){
			state = SHORT_PRESS;
			state_cntr = 0;
		}
		break;
	case SHORT_PRESS:
		
		if (button_is_up()){
			state = NO_PRESS;
			retval = SHORT_PRESS;
		}else if (button_is_down() && state_cntr >= 500 ){
			state = PRESS;
		}
		break;
	case PRESS:
		
		if (button_is_up()){
			state = NO_PRESS;
			retval = PRESS;
		}else if (button_is_down() && state_cntr >= 1000){
			state = LONG_PRESS; 
		}
		break;
	case LONG_PRESS:
		
		if (button_is_up()){
			state = NO_PRESS;
			retval = LONG_PRESS;
		}
		break;
	}
	return retval; 
}

