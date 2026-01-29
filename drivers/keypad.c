/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an driver file that has the implementation functions for using
the keypad peripheral 
-----------------------------------------------------------------------------------
*/

#include "keypad.h"
#include "systick.h"
#include "interrupt.h"
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include <stdint.h>

#define ROW0 13 
#define ROW1 12
#define ROW2 11
#define ROW3 10
#define COL0 9
#define COL1 8
#define COL2 7

#define ALL 13

#define CONCAT2(X, Y) X ## Y
#define CONCAT3(X, Y, Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO, X, _CTRL)
#define PADS_BANK0_GPIO(X)    CONCAT2(PADS_BANK0_GPIO, X)
#define GPIO_RESET (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)

static const char keyPadChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'}; 
static char keypad_char; 

static void keypad_scan_row(int8_t row){
    SIO_GPIO_OUT_CLR = ((1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3)); 
    SIO_GPIO_OE_CLR = ( (1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3)); 

    switch(row){
        case -1: break;
        case 0: 
            SIO_GPIO_OE_SET = (1 << ROW0);
            break;
        case 1: 
            SIO_GPIO_OE_SET = (1 << ROW1);
            break;
        case 2: 
            SIO_GPIO_OE_SET = (1 << ROW2);
            break;
        case 3: 
            SIO_GPIO_OE_SET = (1 << ROW3);
            break;
        default: 
            SIO_GPIO_OE_SET = ( (1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3) );
            break;
    }
}

#define NOKEY_WAS_PRESSED 99 
static uint8_t keypad_scan_cols(void){

    uint8_t retval = NOKEY_WAS_PRESSED;
    
    if ( (SIO_GPIO_IN & (1 << COL0)) == 0 ){
        retval = 0;
    } else if ( (SIO_GPIO_IN & (1 << COL1)) == 0 ){
        retval = 1;
    } else if ( (SIO_GPIO_IN & (1 << COL2)) == 0 ){
        retval = 2;
    } 

    return retval;
}

void configure_keypad(){
    
    RESETS_RESET_CLR = GPIO_RESET;
    while ((RESETS_RESET_DONE & GPIO_RESET) != GPIO_RESET) { continue; }

    PADS_BANK0_GPIO(ROW0) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(0)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(0)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(ROW0) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);
    
    PADS_BANK0_GPIO(ROW1) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(0)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(0)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(ROW1) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

    PADS_BANK0_GPIO(ROW2) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(0)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(0)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(ROW2) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

    PADS_BANK0_GPIO(ROW3) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(0)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(0)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(ROW3) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);
    
    PADS_BANK0_GPIO(COL0) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(1)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(1)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(COL0) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);
    
    PADS_BANK0_GPIO(COL1) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(1)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(1)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(COL1) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

    PADS_BANK0_GPIO(COL2) = 
        PADS_BANK0_GPIO0_OD(0)      |
        PADS_BANK0_GPIO0_IE(1)       |
        PADS_BANK0_GPIO0_DRIVE(0)    |
        PADS_BANK0_GPIO0_PUE(1)     |
        PADS_BANK0_GPIO0_PDE(0)     |
        PADS_BANK0_GPIO0_SCHMITT(0) |
        PADS_BANK0_GPIO0_SLEWFAST(0) ;
    
    IO_BANK0_GPIO_CTRL(COL2) = 
        IO_BANK0_GPIO0_CTRL_IRQOVER(0)  |
        IO_BANK0_GPIO0_CTRL_INOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OEOVER(0)   |
        IO_BANK0_GPIO0_CTRL_OUTOVER(0)  |
        IO_BANK0_GPIO0_CTRL_FUNCSEL(5);
    
    keypad_scan_row(ALL); 
    systick_register_callback(&task_keypad_scan); 
}

#define DEB_PERIOD 5
void task_keypad_scan(void) { 
    static uint8_t deb_cntr;
    static keypad_scan_t state; 
    uint8_t column_scan;

    if (deb_cntr == DEB_PERIOD - 1){
        deb_cntr = 0;
    } else {
        deb_cntr++;
        return;
    }

    switch(state){
        case WAIT_PRESS:
            if (keypad_scan_cols() != NOKEY_WAS_PRESSED){ 
                state = SCAN0;
                keypad_scan_row(0); 
            }
            break;
        case SCAN0:
            column_scan = keypad_scan_cols();
            if (column_scan == NOKEY_WAS_PRESSED ) { 
                state = SCAN1;
                keypad_scan_row(1); 
            
            }else { 
                state = WAIT_RELEASE;
                keypad_scan_row(ALL);
                keypad_char = keyPadChars[0 + column_scan];                                                         
            }
            break;
        
        case SCAN1: 
            column_scan = keypad_scan_cols();
            if (column_scan == NOKEY_WAS_PRESSED) { 
                state = SCAN2; 
                keypad_scan_row(2); 

            } else {
                state = WAIT_RELEASE; 
                keypad_scan_row(ALL); 
                keypad_char = keyPadChars[3 + column_scan];
            }
            break;
        
        case SCAN2:
            column_scan = keypad_scan_cols();
            if (column_scan == NOKEY_WAS_PRESSED) { 
                state = SCAN3; 
                keypad_scan_row(3); 
            } else {
                state = WAIT_RELEASE;
                keypad_scan_row(ALL);
                keypad_char = keyPadChars[6 + column_scan];
            }
            break;
        case SCAN3:
            column_scan = keypad_scan_cols();
            if (column_scan == NOKEY_WAS_PRESSED){ 
                state = WAIT_RELEASE;
                keypad_scan_row(ALL); 

            } else {
                state = WAIT_RELEASE;
                keypad_scan_row(ALL); 
                keypad_char = keyPadChars[9 + column_scan];
            }
            break;
        
        case WAIT_RELEASE:
            if(keypad_scan_cols() == NOKEY_WAS_PRESSED) { 
                state = WAIT_PRESS;
            }
            break;
    }
}

int keypad_getchar(char * cp){ 
    uint32_t interruptStatus;
    int retval;

    __get_primask(&interruptStatus);
    __disable_irq(); 
    if (keypad_char != 0) { 
        retval = 1;
        (*cp) = keypad_char;
        keypad_char = 0; 
    } else {
        retval = 0;
    }
    __set_primask(interruptStatus);
    return retval;
}
