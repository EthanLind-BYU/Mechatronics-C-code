#ifndef CANYON_H
#define CANYON_H

#include <xc.h>

// Prox IR sensors read 1 when they see a wall (active high)
#define FRONT_WALL (_RB7 == 1)   // pin 11
#define LEFT_WALL  (_RB8 == 1)   // pin 12
#define RIGHT_WALL (_RB9 == 1)   // pin 13

void canyon_init(void);
void canyon_check(void);
#endif
