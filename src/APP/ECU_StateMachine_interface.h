#ifndef ECU_STATEMACHINE_INTERFACE_H_
#define ECU_STATEMACHINE_INTERFACE_H_

#include "StdTypes.h"
#include "VehicleManager.h"
#include "Events.h"

/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize ECU State Machine
 * - Set initial states
 * - Load NVM data if available
 */
void ECU_StateMachine_Init(void);

/**
 * Handle events relevant to this SM
 * - Update states
 * - Execute actions
 */
void ECU_StateMachine_ProcessEvent(ecu_event_t event);

/**
 * Handle periodic tasks only
 * No event dequeuing
 */
void ECU_StateMachine_Update(void);

/**
 * Get current ECU state
 * Returns: ecu_state_t (OFF/STARTUP/ACTIVE)
 */
ecu_state_t ECU_GetState(void);

/**
 * Get current operational state
 * Returns: ecu_operational_state_t (NORMAL/DIAGNOSTIC/DEGRADED)
 */
ecu_operational_state_t ECU_GetOperationalState(void);



#endif /* ECU_STATEMACHINE_INTERFACE_H_ */