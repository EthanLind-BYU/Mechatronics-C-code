//TEST


#include <xc.h>
#include "driving.h"
#include "qrd.h"
#include "functions.c"

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
    
    // CONFIGURE TIMERS
    // Timer 1
    _TON = 1; // Timer ON
    _TCS = 0; // Timer Clock Source = internal clock
    _TCKPS = 0b10; // Pre scale the timer by a factor of 64
    // Timer 2

    // Timer 3

    // Timer 4

    // Timer 5

   
    // Turn on PWM for pin14
    // OC1CON1 = 0x1C06; 
    // OC1CON2 = 0x001F;

    // TURN ON PWM
    // OC1 as edge-aligned PWM, clocked from Fcy (4 MHz), period set by OC1RS
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111; // Select main oscillator (Fcy) as clock source
    OC1CON2bits.SYNCSEL = 0x1F; // Self-sync: OC1RS sets the period
    OC1CON1bits.OCM = 0b110;    // Edge-aligned PWM

    // CONFIGURE INTERUPTS
    // Output Compare 1: (Pin 14)
    _OC1IP = 4; // Select the interrupt priority
    _OC1IE = 1; // Enable the OC2 interrupt
    _OC1IF = 0; // Clear the interrupt flag
    //  Output Compare 2: 
    _OC2IP = 4; // Select the interrupt priority
    _OC2IE = 1; // Enable the OC2 interrupt
    _OC2IF = 0; // Clear the interrupt flag



    // Initialize the state
    state = LOOKING_FOR_LINE;

    while (1) {
        switch (state){

            case LOOKING_FOR_LINE:
                static bool runACC = 1; //set to accelerate only at the beginning.
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
                if (0){

                }
                else if (0){

                }
                else {

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
