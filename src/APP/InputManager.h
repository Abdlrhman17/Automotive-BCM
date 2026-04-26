#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "StdTypes.h"

/* ============================================ */
/*          SIMULATION VARIABLES                */
/* ============================================ */
// This replaces the actual hardware until you have it
typedef struct
{
	u8 ignition;	// 0=OFF, 1=ACC, 2=ON, 3=START
	u8 door;		// 0=CLOSED, 1=OPEN
	u8 lock_btn;	// 0=LOCK, 1=UNLOCK, 2=NONE
	u8 blinker;		// 0=OFF, 1=LEFT, 2=RIGHT, 3=HAZARD
	u8 wipers;		// 0=OFF, 1=INT, 2=LOW, 3=HIGH
	u8 speed;		// 0=STOPPED, 1=MOVING
}sim_inputs_t;


typedef enum
{
    INPUT_IGNITION_SWITCH = 0,
    INPUT_DRIVER_DOOR_SWITCH,
    INPUT_LOCK_BUTTON,
    INPUT_BLINKER_STALK,
    INPUT_WIPER_STALK,
    INPUT_SPEED_SIGNAL,
    INPUT_COUNT
}input_signal_t;

void InputManager_Init(void);

void InputManager_Update(void);

sim_inputs_t* INPUTMAN_GetSim(void);

#endif /* INPUTMANAGER_H_ */