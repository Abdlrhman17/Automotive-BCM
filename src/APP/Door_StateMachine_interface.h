#ifndef DOOR_STATEMACHINE_INTERFACE_H_
#define DOOR_STATEMACHINE_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "VehicleManager.h"
#include "Events.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Door State Machine
 * Set initial state (assume closed at startup)
 */
void Door_StateMachine_Init(void);

/**
 * Process door events
 * Handle DOOR_OPEN_EVENT and DOOR_CLOSE_EVENT
 */
void Door_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 */
void Door_StateMachine_Update(void);

/**
 * Get current door state
 */
door_state_t Door_GetState(void);


#endif /* DOOR_STATEMACHINE_INTERFACE_H_ */