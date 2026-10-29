#include <xc.h>
#include "functions.h"
#include "interrupts.h"

static int FAST = 10000;// Max speed PWM
static int MEDIUM = 12000;
static int SLOW = 18000;

enum {STRAIGHT, GO_RIGHT, GO_LEFT};

void line_follow_check() {
  if (QRDR >= THRESHOLD) // checks if the right QRD sees the line
    {
      PATH = GO_RIGHT;
    }
  else if (QRDL >= THRESHOLD) // checks if the left qrd sees the line.
    {
      PATH = GO_LEFT;
    }
  else if (QRDL >= THRESHOLD && QRDM >= THRESHOLD && QRDR >= THRESHOLD) // checks for the end of the line 
    {
      state = CANYON;
    }
  else { //sets it to straight since none of the other sides are seen.
    PATH = STRAIGHT;
  }
}
switch (PATH) {
  case STRAIGHT:
    L_MOTOR = FAST;
    L_DC = L_MOTOR/2;
    R_MOTOR = FAST;
    R_DC = L_MOTOR/2;
    break;
  case GO_RIGHT:
    if (QRDM > THRESHOLD && QRDR > THRESHOLD){
      R_MOTOR = MEDIUM;
      R_DC = R_MOTOR/2;
      L_MOTOR = FAST;
      L_DC = L_MOTOR/2;
    }
    else if (QRDM <= THRESHOLD && QRDR > THRESHOLD){
      R_MOTOR = SLOW;
      R_DC = R_MOTOR/2;
      L_MOTOR = FAST;
      L_DC = L_MOTOR/2;
    }
    break;
  case GO_LEFT:
    if (QRDM > THRESHOLD && QRDL > THRESHOLD){
      R_MOTOR = FAST;
      R_DC = R_MOTOR/2;
      L_MOTOR = MEDIUM;
      L_DC = L_MOTOR/2;
    }
    else if (QRDM <= THRESHOLD && QRDL > THRESHOLD){
      R_MOTOR = FAST;
      R_DC = R_MOTOR/2;
      L_MOTOR = SLOW;
      L_DC = L_MOTOR/2;
    }
    break;
}
