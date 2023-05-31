#ifndef STATES_H_
#define STATES_H_

#include <stdio.h>

typedef enum {
    OFF=0,
    DRIVING,
	BACKUP,
	RIGHT,
	LEFT,
	REORIENT,
	COUNTDOWN,
} states;

typedef enum {
    LEFT_TURN,
	RIGHT_TURN,
	STRAIGHT,
} turns;

#endif /* STATES_H_ */