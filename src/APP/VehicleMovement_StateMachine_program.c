/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "VehicleMovement_StateMachine_interface.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Global Var to the state of the Car movement
static vehicle_movement_state_t Global_current_movement_state = STOPPED;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void VehicleMovement_StateMachine_Init(void)
{
	Global_current_movement_state = STOPPED;
	TRACE_INFO(TRACE_MOVEMENT,"Movement state Initialized to STOPPED");
}

void VehicleMovement_StateMachine_ProcessEvent(ecu_event_t event)
{
	if(event == VEHICLE_STOPPED_EVENT)
	{
		Global_current_movement_state = STOPPED;
		TRACE_INFO(TRACE_MOVEMENT,"Movement state Set to STOPPED");
	}
	else if (event == VEHICLE_MOVING_EVENT)
	{
		Global_current_movement_state = MOVING;
		TRACE_INFO(TRACE_MOVEMENT,"Movement state Set to MOVING");
	}
}

void VehicleMovement_StateMachine_Update(void)
{
	// No periodic tasks needed
}

vehicle_movement_state_t VehicleMovement_GetState(void)
{
	return Global_current_movement_state;
}