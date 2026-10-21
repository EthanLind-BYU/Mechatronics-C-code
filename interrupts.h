#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// These variables live in interrupts.c.
// "extern" tells other files they exist over there.
extern volatile unsigned int stepsTaken;
extern volatile unsigned int stepsToTake;
extern volatile int moveDone;

#endif
