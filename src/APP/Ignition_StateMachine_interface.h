#ifndef IGNITION_STATEMACHINE_INTERFACE_H_
#define IGNITION_STATEMACHINE_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "VehicleManager.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */


/**
 * Initialize Ignition State Machine
 */
void Ignition_StateMachine_Init(void);

/**
 * Process ignition events
 * Enforce sequential transitions
 */
void Ignition_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 * Handle START auto-return timeout
 */
void Ignition_StateMachine_Update(void);

/**
 * Get current ignition state
 */
ignition_state_t Ignition_GetState(void);


#endif /* IGNITION_STATEMACHINE_INTERFACE_H_ */