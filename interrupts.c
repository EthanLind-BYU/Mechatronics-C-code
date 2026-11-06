#include <xc.h>
#include "functions.h"
#include "interrupts.h"

// Step counting (shared with functions.c through interrupts.h)
volatile unsigned int stepsTaken = 0;
volatile unsigned int stepsToTake = 0;
volatile int moveDone = 1;

// The move() function enables this interupt and resets the flag
// OC2 (left motor PWM, pin 4) sets the flag once per PWM period = once per step
// Stops BOTH motors and turns off interrupt once stepsTaken >= stepsToTake
// Then finally, gives a moveDone flag that exits the while loop in the move() function
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void)
{
    _OC2IF = 0;
    stepsTaken++;
    if (stepsTaken >= stepsToTake) {
        L_DC = 0;        // 0% duty = no more step pulses
        R_DC = 0;
        _OC2IE = 0;
        moveDone = 1;
    }
}
