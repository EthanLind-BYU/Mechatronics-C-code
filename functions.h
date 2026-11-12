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

// ---- Speed: 2 rev/s = 400 steps/s -> 4,000,000 / 400 ----
#define PWM_PERIOD  15000

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
