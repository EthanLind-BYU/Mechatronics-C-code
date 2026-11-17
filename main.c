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
    _TRISA2 = 1;  //set-up input for pin 7, AN13 (Laser photodiode)
    _TRISB14 = 1; //set-up input for pin 17, AN10, ping-pong ball photodiode

    // Initiallize motor pins: 
    // pwm (OC2 pin 4, OC3 pin 5)
    // interrupts used for the stepper motors.
    motors_init();

    // Initiallize canyon pins:
    // Digital input on RB7 (pin 11), RB8 (pin 12), RB9 (pin 13)
    canyon_init();
    
    // Setup of the QRDs
    qrd_init();


    // CONFIGURE TIMERS
    // Timer 1
    _TON = 1; // Timer ON
    _TCS = 0; // Timer Clock Source = internal clock
    _TCKPS = 0b10; // Pre scale the timer by a factor of 64

    // Stepper PWM (OC2, OC3) and the OC2 step-counting interrupt are configured in motors_init()

    // Turn on PWM for pin14 (OC1 = servo, not set up yet)
    // OC1CON1 = 0x1C06;
    // OC1CON2 = 0x001F;

    // Initialize the state
    state = LOOKING_FOR_LINE;
    accelerate(1);
    static int ball_dropped = 0;

    while (1) {
        switch (state){

            case LOOKING_FOR_LINE:
                L_MOTOR = 3000;
                R_MOTOR = 3000;
                L_DC = L_MOTOR/2;
                R_DC = R_MOTOR/2;
                if (QRDL >= THRESHOLD && QRDR >= THRESHOLD && QRDM >= THRESHOLD){
                    drive_forward(1.5); // align the line from the qrd sensors to wheel "axle"
                    turn_left();
                    accelerate(1); // set both wheels forward again
                    state = LINE_FOLLOWING;
                }
                break;

            
            case LINE_FOLLOWING:
               line_follow_check();
               if (QRDL >= THRESHOLD && QRDM >= THRESHOLD && QRDR >= THRESHOLD) // checks for the end of the line 
                   {
                     state = CANYON;
                   }
               if (BALL_SENSOR > 1) {
                   pick_up_ball();
               }
               if (RIGHT_WALL && LEFT_WALL && !ball_dropped){
                   ball_drop();
                   ball_dropped = 1;
               }
               if (QRDE > THRESHOLD)//an idea we could add if necessary, have it report when each task is complete, so that if it misreads it knows if the ball has been dropped and the canyon has happened.
               {
                   drive_forward(2);
                   turn_right();
                   accelerate(0);
                   drive_backward(21);
                   servo_angle(0);
                   state = LASER_POINTING;
                   
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
                if (!LASER_SENSOR){
                    servo_angle(ANGLE+1);
                }
                else if (LASER_SENSOR){
                    //set laser to be on (might have to have it repeat in the IDLE state)
                    state = IDLE;
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
