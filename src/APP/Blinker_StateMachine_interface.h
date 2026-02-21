#ifndef BLINKER_STATEMACHINE_INTERFACE_H_
#define BLINKER_STATEMACHINE_INTERFACE_H_

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
 * Initialize Blinker State Machine
 * Set initial state to OFF
 */
void Blinker_StateMachine_Init(void);

/**
 * Process blinker events
 * Enforce ignition requirements
 */
void Blinker_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 */
void Blinker_StateMachine_Update(void);

/**
 * Get current blinker state
 */
blinkers_state_t Blinker_GetState(void);

/**
 * Get previous blinker state
 */
blinkers_state_t Blinker_GetPrevState(void)

#endif /* BLINKER_STATEMACHINE_INTERFACE_H_ */