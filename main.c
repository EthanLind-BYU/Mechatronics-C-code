//TEST


#include <xc.h>
#include "qrd.h"
#include "functions.h"
#include "lineFollow.h"
#include "interrupts.h"
#include "canyon.h"
// Configuration bits for the PIC24F16KA301.
#pragma config FNOSC = FRC      // 8 MHz internal oscillator
#pragma config POSCMOD = NONE   // No external crystal, frees RA2/RA3 for I/O
#pragma config OSCIOFNC = OFF   // RA3 is normal I/O instead of clock output
#pragma config SOSCSRC = DIG    // RA4/RB4 usable as digital I/O
#pragma config FWDTEN = OFF     // Watchdog off, otherwise the chip resets itself periodically

//Create a list of all possible states the car can be in
enum {
    LOOKING_FOR_LINE,
    LINE_FOLLOWING,
    TURNING_LEFT,
    TURNING_RIGHT,
    BALL_PICKUP,
    BALL_DROPOFF,
    CANYON,
    LASER_POINTING,
    IDLE
} state;

int main(void)
{

    // Post scaling by 1
    _RCDIV = 0b000;
    
    // CLEAR REGISTERS
    LATA = 0;
    LATB = 0;
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;

    // Initiallize motor pins: 
    // pwm (OC2 pin 4, OC3 pin 5)
    // interrupts used for the stepper motors.
    motors_init();

    // Initiallize canyon pins:
    // Digital input on RB7 (pin 11), RB8 (pin 12), RB9 (pin 13)
    canyon_init();
    
    // Setup of the QRDs
    qrd_int();


    // CONFIGURE TIMERS
    // Timer 1
    _TON = 1; // Timer ON
    _TCS = 0; // Timer Clock Source = internal clock
    _TCKPS = 0b10; // Pre scale the timer by a factor of 64
    // Timer 2

    // Timer 3

    // Timer 4

    // Timer 5

   
    // Stepper PWM (OC2, OC3) and the OC2 step-counting interrupt are configured in motors_init()

    // Turn on PWM for pin14 (OC1 = servo, not set up yet)
    // OC1CON1 = 0x1C06;
    // OC1CON2 = 0x001F;

    // Initialize the state
    state = LOOKING_FOR_LINE;
    int runACC = 1; //set to accelerate only at the beginning.
    L_MOTOR = 10000;
    L_DC = L_MOTOR/2;
    R_MOTOR = 10000;
    R_DC = R_MOTOR/2;
    while (1) {
        switch (state){

            case LOOKING_FOR_LINE:
                //distance needs to be measured to assure we won't miss anything in the acceleration
                if (runACC){
                    accelerate(1);
                    runACC = 0;
                }                
                
                // // set it to move straight at max speed.
                // L_MOTOR = 10000;
                // static LEFT_DIR = 1;//only runs once, if ran again direction must be set as it changes statess (if needed)
                // L_DC = L_MOTOR/2; // set duty cycle to 50
                // R_MOTOR = 10000;
                // static RIGHT_DIR = 1; // ditto from above
                // R_DC = R_MOTOR/2; //set duty cycle to 50


                // the past code might be unnecessary because of the accelerate function
                    
                else if (QRDL >= THRESHOLD && QRDR >= THRESHOLD){
                    turn_left();
                    accelerate(1); // speed up to begin line following
                    state = LINE_FOLLOWING; //change state
                }
                break;

            
            case LINE_FOLLOWING:
               line_follow_check();
                 if (QRDL >= THRESHOLD && QRDM >= THRESHOLD && QRDR >= THRESHOLD) // checks for the end of the line 
                   {
                     state = CANYON;
                   }
                break;

            case TURNING_LEFT:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

            case TURNING_RIGHT:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

            case BALL_PICKUP:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

            case BALL_DROPOFF:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

            case CANYON:
                if (QRDL <= THRESHOLD && QRDM <= THRESHOLD && QRDR <= THRESHOLD && LEFT_WALL == 1) {// checks for the start of the line 
                    drive_forward(1.5); // align the line from the qrd sensors to wheel "axle"
                    turn_right();
                    accelerate(1); // set both wheels forward again
                    state = LINE_FOLLOWING;
                }
                else if (QRDL <= THRESHOLD && QRDM <= THRESHOLD && QRDR <= THRESHOLD && RIGHT_WALL == 1) {// checks for the start of the line 
                    drive_forward(1.5); // align the line from the qrd sensors to wheel "axle"
                    turn_left();
                    accelerate(1); // set both wheels forward again
                    state = LINE_FOLLOWING;
                }
                else {
                    canyon_check();
                }
                break;

            case LASER_POINTING:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

            case IDLE:
                if (0){

                }
                else if (0){

                }
                else {

                }
                break;

        
    }

    }
}
