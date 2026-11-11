#include <xc.h>
#include "functions.h"
#include "interrupts.h"
#include "lineFollow.h"
#include "qrd.h"

static unsigned int FAST = 5000;// Max speed PWM
static unsigned int MEDIUM = 15000;
static unsigned int SLOW = 60000;
enum {STRAIGHT, SLOW_RIGHT, FAST_RIGHT, SLOW_LEFT, FAST_LEFT, STOP};
int PATH = STRAIGHT;




void line_follow_check() {
//    if (QRDM < THRESHOLD) { // checks if middle is on the line
//        PATH = STRAIGHT;
//    }
//    else if (QRDR < THRESHOLD) // checks if the right QRD is on the line
//      {
//        PATH = FAST_RIGHT;
//    }
//    else if (QRDL < THRESHOLD) // checks if the left QRD is on the line.
//    {
//        PATH = FAST_LEFT;
//    }
//    else if (QRDL < THRESHOLD && QRDR <= THRESHOLD && QRDM <= THRESHOLD) { // checks if all of them are on the line
//        PATH = STRAIGHT;
//    }
// note that if all of them get off of the line, it will be stuck in the state that it was in earlier to keep turning until it hits the line again
    

switch (PATH) {
    case STRAIGHT:
        L_MOTOR = FAST;
        L_DC = L_MOTOR/2;
        R_MOTOR = FAST;
        R_DC = R_MOTOR/2;
        if (QRDL > THRESHOLD) {PATH = SLOW_RIGHT;} // less than means it is on the white line
        if (QRDR > THRESHOLD) {PATH = SLOW_LEFT;}
      break;
    case SLOW_RIGHT:
        R_MOTOR = SLOW;
        R_DC = R_MOTOR/2;
        L_MOTOR = FAST;
        L_DC = L_MOTOR/2;
        if (QRDM > THRESHOLD) {PATH = FAST_RIGHT;}
        if (QRDL < THRESHOLD) {PATH = STRAIGHT;}
      break;
    case FAST_RIGHT:
        R_MOTOR = SLOW;
        R_DC = R_MOTOR/2;
        L_MOTOR = FAST;
        L_DC = L_MOTOR/2;
        if (QRDM < THRESHOLD) {PATH = STRAIGHT;}
      break;
    case SLOW_LEFT:
        R_MOTOR = FAST;
        R_DC = R_MOTOR/2;
        L_MOTOR = SLOW;
        L_DC = L_MOTOR/2;
        if (QRDM > THRESHOLD) {PATH = FAST_LEFT;}
        if (QRDR < THRESHOLD) {PATH = STRAIGHT;}
      break;
    case FAST_LEFT:
        R_MOTOR = FAST;
        R_DC = R_MOTOR/2;
        L_MOTOR = SLOW;
        L_DC = L_MOTOR/2;
        if (QRDM < THRESHOLD) {PATH = SLOW_LEFT;}
      break;
    case STOP:
        R_DC = 0;
        L_DC = 0;
        break;
  }

}



















//// Define sensor weights for 4-sensor array
//const int SENSOR_WEIGHTS[3] = {-1, 0, +1};
//
//int calculate_error(int sensor_values[4]) {
//    int weighted_sum = 0;
//    int total_signal = 0;
//    
//    for (int i = 0; i < 3; i++) {
//        // Invert sensor values: line = dark = high reflection difference
//        // Assuming digital read after ADC: 0 = light, 1023 = dark
//        int inverted = 1023 - sensor_values[i];
//        weighted_sum += inverted * SENSOR_WEIGHTS[i];
//        total_signal += inverted;
//    }
//    
//    // Avoid division by zero (no line detected)
//    if (total_signal == 0) {
//        return 0; // No line - maintain last known error
//    }
//    
//    return weighted_sum / total_signal;
//}
//
//typedef struct {
//    float Kp;  // Proportional gain
//    float Ki;  // Integral gain
//    float Kd;  // Derivative gain
//    float setpoint;
//    float previous_error;
//    float integral;
//} PID_Controller;
//
//void pid_init(PID_Controller *pid, float Kp, float Ki, float Kd) {
//    pid->Kp = Kp;
//    pid->Ki = Ki;
//    pid->Kd = Kd;
//    pid->setpoint = 0;
//    pid->previous_error = 0;
//    pid->integral = 0;
//}
//
//float pid_calculate(PID_Controller *pid, float current_position) {
//    // Calculate error (position relative to setpoint)
//    float error = pid->setpoint - current_position;
//    
//    // Proportional term
//    float P = pid->Kp * error;
//    
//    // Integral term (with anti-windup limit)
//    pid->integral += error;
//    // Clamp integral to prevent windup
//    if (pid->integral > 100) pid->integral = 100;
//    if (pid->integral < -100) pid->integral = -100;
//    float I = pid->Ki * pid->integral;
//    
//    // Derivative term
//    float derivative = error - pid->previous_error;
//    float D = pid->Kd * derivative;
//    
//    // Store current error for next iteration
//    pid->previous_error = error;
//    
//    // Total PID output
//    return P + I + D;
//}
//
//void calculate_wheel_speeds(int pid_output, int *left_speed, int *right_speed) {
//    int base_speed = 50000;  // Nominal forward speed (0-255 PWM)
//    int max_speed = 5000;
//    
//    // PID output positive = robot left of line = need to turn right
//    // So increase right wheel speed, decrease left wheel speed
//    *left_speed = base_speed + pid_output;
//    *right_speed = base_speed - pid_output;
//    
//    // Clamp to valid PWM range
//    if (*left_speed < max_speed) *left_speed = max_speed;
//    if (*left_speed > 65000) *left_speed = 0;
//    if (*right_speed < max_speed) *right_speed = max_speed;
//    if (*right_speed > 65000) *right_speed = 0;
//}
//
//int line_follow_check(void) {
//    // Initialize hardware (ADC, PWM, motors)
////    adc_init();
////    pwm_init();
//    
//    // Initialize PID controller
//    PID_Controller pid;
//    pid_init(&pid, 5, 0.0, 0.25);  // Start with no integral
//    
//    int sensor_values[3];
//    int left_motor, right_motor;
//
//    while (1) {
//        // Read all 4 sensors
////        for (int i = 0; i < 3; i++) {
////            sensor_values[i] = adc_read(i);
////        }
//        
//        sensor_values[0] = QRDL;
//        sensor_values[1] = QRDM;
//        sensor_values[2] = QRDR;
//        
//        // Calculate position error
//        int position_error = calculate_error(sensor_values);
//        
//        // Get PID correction
//        int correction = pid_calculate(&pid, position_error);
//        
//        // Calculate wheel speeds
//        calculate_wheel_speeds(correction, &left_motor, &right_motor);
//            
//        // Apply to motors
////        pwm_set_duty_cycle(LEFT_MOTOR, left_motor);
////        pwm_set_duty_cycle(RIGHT_MOTOR, right_motor);
//        L_MOTOR = left_motor;
//        R_MOTOR = right_motor;
//        L_DC = L_MOTOR/2;
//        R_DC = R_MOTOR/2;
//        
////        delay_ms(10);  // 100Hz control loop
//    }
//}
