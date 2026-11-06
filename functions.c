#include <xc.h>
#include "functions.h"
#include "interrupts.h"

// Call once at the start of main
void motors_init(void)
{
    // Pins 4 and 5 (RB0/RB1) can also be analog inputs (AN2/AN3), so make them digital
    _ANSB0 = 0;
    _ANSB1 = 0;

    _TRISB4 = 0;   // Pin 9,  Left DIR
    _TRISA4 = 0;   // Pin 10, Right DIR
    _TRISB0 = 0;   // Pin 4,  OC2 PWM -> Left STEP
    _TRISB1 = 0;   // Pin 5,  OC3 PWM -> Right STEP

    // OC2 (left) and OC3 (right): edge-aligned PWM clocked from Fcy (4 MHz).
    // Each one is self-synced, so OCxRS (L_MOTOR / R_MOTOR) sets its own period.
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC3CON1 = 0;
    OC3CON2 = 0;

    L_MOTOR = PWM_PERIOD;
    R_MOTOR = PWM_PERIOD;
    L_DC = 0;      // 0% duty = start stopped
    R_DC = 0;

    OC2CON1bits.OCTSEL = 0b111;  // Clock source = Fcy
    OC2CON2bits.SYNCSEL = 0x1F;  // Self-sync: OC2RS sets the period
    OC2CON1bits.OCM = 0b110;     // Edge-aligned PWM
    OC3CON1bits.OCTSEL = 0b111;  // Clock source = Fcy
    OC3CON2bits.SYNCSEL = 0x1F;  // Self-sync: OC3RS sets the period
    OC3CON1bits.OCM = 0b110;     // Edge-aligned PWM

    // The OC2 interrupt fires once per step and counts steps for move().
    // Only move() turns it on.
    _OC2IP = 4;
    _OC2IF = 0;
    _OC2IE = 0;
}

// The code waits in move() until the OC2 interrupt says the steps are done.
// Both wheels run at PWM_PERIOD so they take the same number of steps, which means
// counting only the left motor (OC2) is enough.
static void move(int leftDir, int rightDir, unsigned int steps)
{
    LEFT_DIR = leftDir;
    RIGHT_DIR = rightDir;

    stepsTaken = 0;
    stepsToTake = steps;
    moveDone = 0;

    // Line following may have left the wheels at different speeds, so reset both
    L_MOTOR = PWM_PERIOD;
    R_MOTOR = PWM_PERIOD;

    _OC2IF = 0;
    _OC2IE = 1;
    L_DC = PWM_PERIOD / 2;   // 50% duty = start stepping
    R_DC = PWM_PERIOD / 2;

    while (!moveDone) { }    // wait for the interrupt to finish the move (it stops both motors)
}

// Steps per inch = 200 / (pi * wheel diameter)
// For a 3 5/8 wheel: 200 / (3.14159 * 3.625) = 17.56   <-- update when wheel is measured

void accelerate(int direction) //intended to avoid misalignment from a quick start. 
// can be used as a function before changing state.
{
    LEFT_DIR = direction;
    RIGHT_DIR = direction;
    int PWM = 40000; //sets PWM to start slow
    while (PWM > 10000) {
        PWM = PWM - 10;
    }
    PWM = 10000;
}

void drive_forward(float inches)
{
    move(1, 1, (unsigned int)(inches * 17.56));
}

void drive_backward(float inches)
{
    move(0, 0, (unsigned int)(inches * 17.56));
}

// 90 deg turn: (pi * distance_between wheels / 4) *
// (3.14159 * 10.35 / 4) * 17.56 = 143 steps   <-- tune on the floor
void turn_left(void)    // left wheel back, right wheel forward
{
    move(0, 1, 143);
}

void turn_right(void)   // left wheel forward, right wheel back
{
    move(1, 0, 143);
}

void turn_180(void) {
    move(1, 0, 286);
}

void pick_up_ball(void)
{
    turn_right();
    drive_forward(5);
    drive_backward(5);
    turn_left();
}

void basic_mobility_milestone(void) {
    drive_forward(5);
    turn_right();
    drive_forward(5);
    turn_180();
    drive_forward(5);
}
