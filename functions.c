#include <xc.h>
#include "functions.h"
#include "interrupts.h"
#include "qrd.h"


// Call once at the start of main
void motors_init(void)
{
    // Pins 4 and 5 (RB0/RB1) can also be analog inputs (AN2/AN3), so make them digital
    _ANSB0 = 0;
    _ANSB1 = 0;

    _TRISB4 = 0;   // Pin 9,  Left DIR
    _TRISA4 = 0;   // Pin 10, Right DIR
    _TRISB0 = 0;   // Pin 4,  OC2 PWM  Left STEP
    _TRISB1 = 0;   // Pin 5,  OC3 PWM  Right STEP
    _TRISA6 = 0;   // Pin 14, OC1 PWM  Servo PWM

    // OC2 (left) and OC3 (right): edge-aligned PWM clocked from Fcy (4 MHz).
    // Each one is self-synced, so OCxRS (L_MOTOR / R_MOTOR) sets its own period.
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC1CON1 = 0;
    OC1CON2 = 0;

    L_MOTOR = PWM_PERIOD;
    R_MOTOR = PWM_PERIOD;
    L_DC = 0;      // 0% duty = start stopped
    R_DC = 0;
    
    SERVO = 65000;
    ANGLE = 7250;
    
    _TRISB15 = 1;  // pin 18, final lander sensor

    

    OC2CON1bits.OCTSEL = 0b111;  // Clock source = Fcy
    OC2CON2bits.SYNCSEL = 0x1F;  // Self-sync: OC2RS sets the period
    OC2CON1bits.OCM = 0b110;     // Edge-aligned PWM
    OC3CON1bits.OCTSEL = 0b111;  // Clock source = Fcy
    OC3CON2bits.SYNCSEL = 0x1F;  // Self-sync: OC3RS sets the period
    OC3CON1bits.OCM = 0b110;     // Edge-aligned PWM
    OC1CON1bits.OCTSEL = 0b111;  // Clock source = Fcy
    OC1CON2bits.SYNCSEL = 0x1F;  // Self-sync: OC2RS sets the period
    OC1CON1bits.OCM = 0b110;     // Edge-aligned PWM

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

void accelerate(int left, int right, int pwm_input) //intended to avoid misalignment from a quick start. 
// can be used as a function before changing state.
{
//    L_DC = 0;
//    R_DC = 0;
//    for (volatile long i = 0; i < 20000; i++);
    LEFT_DIR = left;
    RIGHT_DIR = right;
//    unsigned int PWM = 65000; //sets PWM to start slow
//    while (PWM > pwm_input) {
//        PWM = PWM - 1;
//        L_MOTOR = PWM;
//        R_MOTOR = PWM;
//        L_DC = PWM/2;
//        R_DC = PWM/2;
//        for (volatile long i = 0; i < 4000; i++);
//    }
////    PWM = 10000;
}

void drive_forward(float inches)
{
    move(1, 1, (unsigned int)(inches * 17.56));
}

void drive_backward(float inches)
{
    move(0, 0, (unsigned int)(inches * 17.56));
}

void servo_angle(int angle) 
{
    while (ANGLE != angle){
        if (ANGLE > angle){
            ANGLE--;
        }
        if (ANGLE < angle){
            ANGLE++;
        }
        int DELAY = 0;
        while (DELAY != 75){
            DELAY++;
        }
    }
}



//void laser_search(void) {
//    int angle = ANGLE+1;
//    if (LASER_SENSOR){
//        servo_angle(angle);
//        angle +=1;
//    }
//}

// 90 deg turn: (pi * distance_between wheels / 4) *
// (3.14159 * 10.35 / 4) * 17.56 = 143 steps   <-- tune on the floor
void turn_left(void)    // left wheel back, right wheel forward
{
    accelerate(0,1,10000);
    move(0, 1, 350);
}

void turn_right(void)   // left wheel forward, right wheel back
{
    accelerate(1,0,10000);
    move(1, 0, 605);
}

void turn_180(void) 
{
    move(1, 0, 286);
}

void pick_up_ball(void)
{
    int delay = 0;
    turn_right();
    drive_forward(5);
    while (delay<65000)// wait for the ball to drop
    {
        delay++;
    }
    drive_backward(5);
    turn_left();
}

//void basic_mobility_milestone(void) {
//    drive_forward(5);
//    turn_right();
//    drive_forward(5);
//    turn_180();
//    drive_forward(5);
//}

void ball_drop(int qrdb){
    //we might need to move forward or back a bit first to get it to turn to the right spot
    if (!qrdb){
        turn_left(); // this will put the back over the right basket
        servo_angle(4000); // set angle to open the ball
        int delay = 0;
        while (delay != 300){ //this is to give the ball time to fall. we might have to make it longer.
        for (volatile long i = 0; i < 1000; i++);
            delay ++;
        }
        servo_angle(6250);    

//        servo_angle = 6000 //in case we need to set it to be lower again for the purpose of the laser.
        turn_right();
    }
    else{
        turn_right();// this will set the back over the left basket
        servo_angle(4000); // set angle to open the ball
        int delay = 0; 
        while (delay != 300){ //this is to give the ball time to fall. we might have to make it longer.
        for (volatile long i = 0; i < 1000; i++);

            delay ++;
        }
        servo_angle(6250);    
        turn_left();
    }
    accelerate(1,1,0);
}
