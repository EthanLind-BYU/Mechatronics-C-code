/*
 * File:   Lab5.c
 * Author: Ethan Lind
 *
 * Created on October 15, 2026, 9:44 AM
 */


#include "xc.h"
#include "functions.h"

#pragma config FNOSC = FRCDIV // Selecting 8 MHz Oscillator
#pragma config POSCMOD = NONE   // No external crystal, frees RA2/RA3 for I/O
#pragma config OSCIOFNC = OFF   // RA3 is normal I/O instead of clock output
#pragma config SOSCSRC = DIG    // RA4/RB4 usable as digital I/O
#pragma config FWDTEN = OFF     // Watchdog off, otherwise the chip resets itself periodically


#define startStop _RA2

enum {
    IDLE,
    MILESTONE
    
} state;

int main(void) {
    // Post scaling by 1
    _RCDIV = 0b000;
    
    //Clear All Registers
    LATA = 0;
    LATB = 0;
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;
   
    // Initiallize all pins, pwm, and inturrupts used for the stepper motors
    motors_init();
    
    // Set Pin 7 to be the start / stop pin
    _TRISA2 = 1;

    // Set initial State
    state = IDLE;
    

    // LOOP
    while (1) {
    switch (state){

        case IDLE:
            if (startStop == 0){ // Before button is pressed
                state = IDLE;
            }
            else if (startStop == 1) {  
                basic_mobility_milestone();
            }
            break;
    }
}

    return 0;
}  