/*
-----------------------------------------------------------------------------------
Jidechukwu Obioha
This file is an application file that uses SPI to communicate with an accelerometer 
(LIS3DH)
-----------------------------------------------------------------------------------
*/

#include "usbcdc.h"
#include "systick.h"
#include "spi0.h"
#include "interrupt.h"
#include <stdint.h>
#include <stdio.h>

#define READ_CMD (1<<7)
#define WRITE_CMD 0
#define RW_MULTIPLE (1<<6)
#define DUMMY 0

int (* _stdin_getchar)( char *c) = usbcdc_getchar;
int (* _stdout_putchar)( char c) = usbcdc_putchar;

void Accelerometer_FSM(char);
void Clear_SPI_FIFO(void);
int8_t Convert_Char_to_Hex(char);

int main(void){

    __disable_irq();
    configure_spi0();
    configure_systick();
    configure_usbcdc();
    __enable_irq();

    char usbcdc_char;
    while (1){
        __wait_for_interrupt();
        if ( !system_tick() ) {
            continue;
        }

        if ( usbcdc_getchar(&usbcdc_char) ){ 
            Accelerometer_FSM(usbcdc_char);
        }

    }

    return (0);
}


void Accelerometer_FSM(char key){

    static enum {IDLE, GET_UPPER_ADDR, GET_LOWER_ADDR, GET_UPPER_DATA, GET_LOWER_DATA, WAIT_ENTER} state;
    static char cmd; 
    static uint8_t addr, data;
    int8_t hexit;

    switch(state){

    case IDLE:
        cmd = 0;
        addr = 0;
        data = 0;
        if (key == 'C'){
            cmd = 'C';
            state = WAIT_ENTER;
        } else if (key == 'R'){
            cmd = 'R';
            state = GET_UPPER_ADDR;
        }else if (key == 'W'){
            cmd = 'W';
            state = GET_UPPER_ADDR;
        }
        break;

    case GET_UPPER_ADDR:
        hexit = Convert_Char_to_Hex(key);
        if (hexit < 0) {
            state = IDLE;
        } else {
            addr = hexit << 4;
            state = GET_LOWER_ADDR; 
        }
        break;

    case GET_LOWER_ADDR:
        hexit = Convert_Char_to_Hex(key);
        if (hexit < 0){
            state = IDLE;
        } else {
            addr |= hexit; 
            if (cmd == 'R'){
                spi0_write(READ_CMD | addr); 
                spi0_write(DUMMY); 
                state = WAIT_ENTER;
            }else if (cmd == 'W'){
                state = GET_UPPER_DATA;
            }
        }
        break;

    case GET_UPPER_DATA:
        hexit = Convert_Char_to_Hex(key);
        if (hexit < 0){
            state = IDLE; 
        }else {
            data = hexit << 4;
            state = GET_LOWER_DATA;
        }
        break;

    case GET_LOWER_DATA:
        hexit = Convert_Char_to_Hex(key);
        if (hexit < 0){
            state = IDLE;
        } else {
            data |= hexit; 
            state = WAIT_ENTER;
        }
        break;

    case WAIT_ENTER:
        if (key == '\r' && cmd == 'C'){
            Clear_SPI_FIFO();
        } else if (key == '\r' && cmd == 'R'){
            uint8_t dummy_data, real_data;
            spi0_read(&dummy_data); 
            spi0_read(&real_data); 
            printf("The value of register %02X is %02X.\n", addr, real_data);
            
        } else if (key == '\r' && cmd == 'W'){
            spi0_write(WRITE_CMD | addr); 
            spi0_write(data); 
	        Clear_SPI_FIFO(); 
            printf("The value: %02X was written to register at address: %02X.\n", data, addr);
        }
        state = IDLE;
        break;
    }
}

void Clear_SPI_FIFO(void){
    uint8_t x; 
    for (uint8_t i = 0; i < 7; i++){
        spi0_read(&x);
    }
}

int8_t Convert_Char_to_Hex(char hexit){ 
    if (hexit >= '0' && hexit <= '9'){
        return hexit - '0';
    }

    if (hexit >= 'A' && hexit <= 'F'){
        return 10 + (hexit - 'A');
    }

    if (hexit >= 'a' && hexit <= 'f'){
        return 10 + (hexit - 'a');
    }

    return -1;
}

