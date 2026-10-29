#include <xc.h>
#include "functions.h"
#include "interrupts.h"

// ---- Speed: 2 rev/s = 400 steps/s -> 4,000,000 / 400 ----
#define PWM_PERIOD  10000

// ---- Pins ----
#define LEFT_DIR   _LATB4
#define RIGHT_DIR  _LATA4
#define DISABLE    _LATB0     // high = motors off
#DEFINE L_MOTOR OC2RS    // Set to control speed of the left motor
#DEFINE R_MOTOR OC3RS    // Set to control the speed of the right motor

// Call once at the start of main
void motors_init(void)
{
    ANSA = 0;
    _TRISA0 = 0;   // Left DIR
    _TRISA1 = 0;   // Right DIR
    _TRISB0 = 0;   // Disable (both motors)
    _TRISA6 = 0;   // Pin 14, PWM to both STEP pins

    OC1CON1 = 0x1C06;
    OC1CON2 = 0x001F;
    OC1RS = PWM_PERIOD;
    OC1R = 0;          // start stopped             ----------------------------CHECK HERE-------------

    _OC1IP = 4;
    _OC1IF = 0;
    _OC1IE = 0;

    DISABLE = 0;       // motors on (but Duty Cycle is still 0% until move() changes that)
}

// The code cycles through the move function until the OC1 interrupt says the steps are done.
static void move(int leftDir, int rightDir, unsigned int steps)
{
    LEFT_DIR = leftDir;
    RIGHT_DIR = rightDir;

    stepsTaken = 0;
    stepsToTake = steps;
    moveDone = 0;

    _OC1IF = 0;
    _OC1IE = 1;
    OC1R = PWM_PERIOD / 2;   // 50% duty = start stepping

    while (!moveDone) { }    // wait for the interrupt to finish the move
}

// Steps per inch = 200 / (pi * wheel diameter)
// For a 3 5/8 wheel: 200 / (3.14159 * 3.625) = 17.56   <-- update when wheel is measured

void accellerate(direction) //intended to avoid missalignement from a quick start. 
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
