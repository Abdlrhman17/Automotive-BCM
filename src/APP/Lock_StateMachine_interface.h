#ifndef LOCK_STATEMACHINE_INTERFACE_H_
#define LOCK_STATEMACHINE_INTERFACE_H_
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
 * Initialize Lock State Machine
 * Set initial state, load from NVM if saved
 */
void Lock_StateMachine_Init(void);

/**
 * Process lock-related events
 * Enforce safety rules (door closed)
 */
void Lock_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 */
void Lock_StateMachine_Update(void);

/**
 * Get current lock state
 */
door_lock_state_t Lock_GetState(void);

#endif /* LOCK_STATEMACHINE_INTERFACE_H_ */