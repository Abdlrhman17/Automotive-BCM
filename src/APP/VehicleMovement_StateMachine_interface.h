#ifndef VEHICLEMOVEMENT_STATEMACHINE_INTERFACE_H_
#define VEHICLEMOVEMENT_STATEMACHINE_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "VehicleManager.h"
#include "Events.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Vehicle Movement State Machine
 * Set initial state to STOPPED
 */
void VehicleMovement_StateMachine_Init(void);

/**
 * Process movement events
 * Handle VEHICLE_STOPPED_EVENT and VEHICLE_MOVING_EVENT
 */
void VehicleMovement_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Periodic update
 */
void VehicleMovement_StateMachine_Update(void);

/**
 * Get current movement state
 */
vehicle_movement_state_t VehicleMovement_GetState(void);

#endif /* VEHICLEMOVEMENT_STATEMACHINE_INTERFACE_H_ */