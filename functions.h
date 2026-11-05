#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// ---- Pins ----
#define LEFT_DIR   _LATB4
#define RIGHT_DIR  _LATA4
#define DISABLE_LATB0     // high = motors off
#define L_MOTOR OC2RS    // Set to control speed pin 4 to the left motor
#define L_DC OC2R       
#define R_MOTOR OC3RS    // Set to control the speed pin 5 to the right motor
#define R_DC OC3R

void motors_init(void);
void turn_left(void);
void turn_right(void);
void drive_forward(float inches);
void drive_backward(float inches);
void turn_180(void);
void pick_up_ball(void);
void basic_mobility_milestone(void);
void accelerate(int direction);

#endif
