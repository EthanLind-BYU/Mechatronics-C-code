#include <xc.h>
#include "functions.h"
#include "interrupts.h"
#include "lineFollow.h"
#include "qrd.h"

static unsigned int FAST = 8000;// Max speed PWM
static unsigned int MEDIUM = 35000;
static unsigned int SLOW = 65000;
static unsigned int SPEED = 15000;

enum {STRAIGHT, SLOW_RIGHT, FAST_RIGHT, SLOW_LEFT, FAST_LEFT};
int PATH = STRAIGHT;




void line_follow_check() {
     if (QRDR >= THRESHOLD) // checks if the right QRD is off of the line
    {
      if QRDM >= THRESHOLD) // checks if the middle is off the line
      {
        PATH = FAST_LEFT;
      }
      else {
        PATH = SLOW_LEFT;
      }
    }
    else if (QRDL >= THRESHOLD) // checks if the left qrd is off of the line.
    {
      if QRDM >= THRESHOLD) // checks if the middle is off the line
      {
        PATH = FAST_LEFT;
      }
      else {
        PATH = SLOW_LEFT;
      }
    }
    else if (QRDL <= THRESHOLD && QRDR <= THRESHOLD && QRDM <= THRESHOLD) { // checks if all of them are on the line
        PATH = STRAIGHT
    }
  // note that if all of them get off of the line, it will be stuck in the state that it was in earlier to keep turning until it hits the line again
    
    
switch (PATH) {
    case STRAIGHT:
      L_MOTOR = FAST;
      L_DC = L_MOTOR/2;
      R_MOTOR = FAST;
      R_DC = L_MOTOR/2;
      break;
    case SLOW_RIGHT:
        R_MOTOR = MEDIUM;
        R_DC = R_MOTOR/2;
        L_MOTOR = FAST;
        L_DC = L_MOTOR/2;
      break;
    case FAST_RIGHT:
      R_MOTOR = SLOW;
      R_DC = R_MOTOR/2;
      L_MOTOR = FAST;
      L_DC = L_MOTOR/2;
      break;
    case SLOW_LEFT:
      R_MOTOR = FAST;
      R_DC = R_MOTOR/2;
      L_MOTOR = MEDIUM;
      L_DC = L_MOTOR/2;
      break;
    case FAST_LEFT:
      R_MOTOR = FAST;
      R_DC = R_MOTOR/2;
      L_MOTOR = SLOW;
      L_DC = L_MOTOR/2;
      break;
  }

}
