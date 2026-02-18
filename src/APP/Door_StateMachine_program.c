/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Door_StateMachine_interface.h"
#include "Events.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
static door_state_t Global_current_door_state = DOOR_CLOSED;


/* ============================================ */
/*       PUBLIC FUNCTIONS IMPLEMENTATION        */
/* ============================================ */

void Door_StateMachine_Init(void)
{
	Global_current_door_state = DOOR_CLOSED;	
	 TRACE_INFO(TRACE_DOOR, "Door SM Initialized - CLOSED");
}

void Door_StateMachine_ProcessEvent(ecu_event_t event)
{
	switch(event)
	{
		case DOOR_CLOSE_EVENT:
		Global_current_door_state = DOOR_CLOSED;	// CLOSED
		TRACE_INFO(TRACE_DOOR,"Door Closed");
		break;
		
		case DOOR_OPEN_EVENT:
		Global_current_door_state = DOOR_OPEN;		// OPENED
		TRACE_INFO(TRACE_DOOR,"Door Opened");
		break;
		
		default:
		// Ignore
		break;
	}
}

void Door_StateMachine_Update(void)
{
	// TODO: If any door ajar timeout warning if wanted
}

door_state_t Door_GetState(void)
{
	return Global_current_door_state;
}