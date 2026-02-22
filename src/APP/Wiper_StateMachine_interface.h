#ifndef WIPER_STATEMACHINE_INTERFACE_H_
#define WIPER_STATEMACHINE_INTERFACE_H_

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
 * Initialize Wiper State Machine
 * Set initial state to OFF
 */
void Wiper_StateMachine_Init(void);

/**
 * Process wiper events
 * Enforce ignition ON requirement
 * Force OFF if ignition not ON
 */
void Wiper_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 * Monitor ignition state, force OFF if not ON
 */
void Wiper_StateMachine_Update(void);

/**
 * Get current wiper state
 */
wiper_control_state_t Wiper_GetState(void);

#endif /* WIPER_STATEMACHINE_INTERFACE_H_ */