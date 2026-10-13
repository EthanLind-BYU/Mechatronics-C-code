#ifndef DRIVING_H
#define DRIVING_H

// Public interface for the driving module.
// Anything declared here can be called from main.c (or any other file)
// after #include "driving.h". Everything else in driving.c stays private.

typedef enum {
    TURN_LEFT,
    TURN_RIGHT
} TurnDirection;


// Turn the vehicle in the given direction.
void turn_left();

void turn_right();

#endif // DRIVING_H
