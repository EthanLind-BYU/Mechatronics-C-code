#include <xc.h>
#include "driving.h"

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void)
{
    static unsigned int numberOfStepsTaken = 0;
    static unsigned int dir = 0;
    unsigned int numberOfStepsToTake = 200;
    
    _OC2IF = 0; // Clear the interrupt flag
    
    numberOfStepsTaken++;
}

void turn_left() {
    
}


void turn_right() {

}
