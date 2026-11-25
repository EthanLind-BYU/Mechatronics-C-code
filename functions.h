#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// ---- Stepper pins ----
// Pin 9  (RB4)      : Left stepper DIR
// Pin 10 (RA4)      : Right stepper DIR
// Pin 4  (RB0, OC2) : Left stepper STEP (PWM)
// Pin 5  (RB1, OC3) : Right stepper STEP (PWM)
// There is no driver disable pin: a motor stops when its duty cycle (L_DC / R_DC) is 0.
#define LEFT_DIR   _LATB4
#define RIGHT_DIR  _LATA4
#define L_MOTOR    OC2RS   // Left motor period (sets speed, bigger = slower)
#define L_DC       OC2R    // Left motor duty cycle (0 = stopped, L_MOTOR/2 = running)
#define R_MOTOR    OC3RS   // Right motor period (sets speed, bigger = slower)
#define R_DC       OC3R    // Right motor duty cycle (0 = stopped, R_MOTOR/2 = running)
#define SERVO      OC1RS   // Servo Motor PWM on pin 14
#define ANGLE      OC1R    // Servo Motor Angle
#define LAND_SENSOR (_RB15 == 1)   // pin 18 for the outer sensor.


// for the servo motors and angles, to get the 20ms suggestion for PWM, we would need a postscalor
// so we can try to figure out how to get the postscaler to apply to just this, or make all of our other numbers smaller as well.

//Speed: 2 rev/s = 400 steps/s -> 4,000,000 / 400 
#define PWM_PERIOD  15000 // controls speed in canyon,and any other move functions
#define BALL_SENSOR PORTBbits.RB14 //setup for pin 17 
#define LASER_SENSOR PORTAbits.RA2 //setup for pin 7
#define LASER LATAbits.LATA3 //pin 8 setup to power laser


void motors_init(void);
void turn_left(void);
void turn_right(void);
void drive_forward(float inches);
void drive_backward(float inches);
void turn_180(void);
void pick_up_ball(void);
void basic_mobility_milestone(void);
void accelerate(int left, int right, int pwm_input);
void servo_angle(int angle);
//void laser_search(void);
void ball_drop(int qrdb);

#endif
