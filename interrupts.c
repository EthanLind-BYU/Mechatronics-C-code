#include <xc.h>
#include "interrupts.h"

// Step counting (shared with functions.c through interrupts.h)
volatile unsigned int stepsTaken = 0;
volatile unsigned int stepsToTake = 0;
volatile int moveDone = 1;

// The move() function enables this interupt and resets the flag
// The interrupt is called by the clock
// Runs once per step. Stops the motors and turns off interrupt once stepsTaken >= stepsToTake 
// Then finally, gives a moveDone flag that exits the while loop in the move() function
void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void)
{
    _OC1IF = 0;
    stepsTaken++;
    if (stepsTaken >= stepsToTake) {
        OC1R = 0;        // 0% duty = no more step pulses
        _OC1IE = 0;
        moveDone = 1;
    }
}
