#include <xc.h>
#include "functions.h"
#include "canyon.h"

void canyon_init(void)
{
    _TRISB7 = 1;  // pin 11, front sensor
    _TRISB8 = 1;  // pin 12, left sensor
    _TRISB9 = 1;  // pin 13, right sensor
}

void canyon_check(void)
{

    // Check the state of the sensors
    if (FRONT_WALL == 0 && LEFT_WALL == 0) { // Front and left sensors triggered
        turn_right();
    }
    if (FRONT_WALL == 0 && RIGHT_WALL == 0) { // Front and right sensors triggered
        turn_left();
    }
    if (RIGHT_WALL == 0) { // Right sensor triggered
        drive_forward(1); // Move forward along right wall
    }
    if (LEFT_WALL == 0) { // Left sensor triggered
        drive_forward(1); // Move forward along left wall
    }
    if (RIGHT_WALL == 0 && LEFT_WALL == 0) { // Both side sensors triggered
        drive_forward(1); // Move forward in the canyon
    }
    if (RIGHT_WALL == 1 && LEFT_WALL == 1) { // Both side sensors triggered
        drive_forward(4); // Move forward in the canyon
    }


    // if (FRONT_WALL == 0) {        // nothing ahead: drive straight
    //     LEFT_DIR = 1;            // turn_left/right leave these reversed
    //     RIGHT_DIR = 1;
    //     L_MOTOR = PWM_PERIOD;
    //     R_MOTOR = PWM_PERIOD;
    //     L_DC = PWM_PERIOD / 2;
    //     R_DC = PWM_PERIOD / 2;
    // }
    // else if (LEFT_WALL == 0) {       // wall ahead, left open
    //     // drive_forward(5); // Move forward along left wall
    //     turn_left();
    // }
    // else if (RIGHT_WALL == 0) {      // wall ahead, right open
    //     // drive_forward(5); // Move forward along right wall
    //     turn_right();
    // }
}
