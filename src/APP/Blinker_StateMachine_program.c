/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "Blinker_StateMachine_interface.h"
#include "Ignition_StateMachine_interface.h"
#include "ECU_StateMachine_interface.h"
#include "DiagnosticPolicy.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Global Var for the state of the blinkers (OFF/RIGHT/LEFT/HAZARD)
static blinkers_state_t Global_current_blinker_state = BLINKERS_OFF;

// Global Var for the previous state of the blinkers (OFF/RIGHT/LEFT/HAZARD)
static blinkers_state_t Global_previous_blinker_state = BLINKERS_OFF;


/* ============================================ */
/*       PUBLIC FUNCTIONS IMPLEMENTATION        */
/* ============================================ */

void Blinker_StateMachine_Init(void)
{
	Global_current_blinker_state = BLINKERS_OFF;
	Global_previous_blinker_state = BLINKERS_OFF;
	
	TRACE_INFO(TRACE_BLINKER,"Blinkers Initialized");
}

void Blinker_StateMachine_ProcessEvent(ecu_event_t event)
{
	//	Check if in diag mode: Allow all blinkers No Restrictions
	if(ECU_GetOperationalState() == ECU_OP_STATE_DIAGNOSTIC && DIAG_ALLOW_BLINKER_CONTROL)
	{
		switch(event)
		{
			case HAZARD_REQUEST:	//HAZARD Request
			Global_previous_blinker_state = Global_current_blinker_state;
			Global_current_blinker_state = BLINKERS_HAZARD;
			TRACE_INFO(TRACE_BLINKER,"HAZARD on (Diag)");
			break;
			
			case LEFT_BLINKER_REQUEST:	// Left blinker Request
			if(Global_current_blinker_state == BLINKERS_HAZARD)	// HAZARD override blinkers
			{
				TRACE_ERROR(TRACE_BLINKER,"LEFT blinker overriden (Diag)");
			}
			else
			{
				Global_previous_blinker_state = Global_current_blinker_state;
				Global_current_blinker_state = BLINKERS_LEFT;
				TRACE_INFO(TRACE_BLINKER,"LEFT Blinkers on (Diag)");
			}
			break;
			
			case RIGHT_BLINKER_REQUEST:		// Left blinker Request
			if(Global_current_blinker_state == BLINKERS_HAZARD)		// HAZARD override blinkers
			{
				TRACE_ERROR(TRACE_BLINKER,"RIGHT blinker overriden (Diag)");
			}
			else
			{
				Global_previous_blinker_state = Global_current_blinker_state;
				Global_current_blinker_state = BLINKERS_RIGHT;
				TRACE_INFO(TRACE_BLINKER,"RIGHT Blinkers on (Diag)");
			}
			break;
			
			case BLINKER_OFF_REQUEST:		// Blinkers OFF
			Global_previous_blinker_state = Global_current_blinker_state;
			Global_current_blinker_state = BLINKERS_OFF;
			TRACE_INFO(TRACE_BLINKER,"Blinkers OFF (Diag)");
			break;
		}
	}
	else
	{
		switch(event)
		{
			case HAZARD_REQUEST:	// Allow HAZARD Regardless of ignition mode
			Global_previous_blinker_state = Global_current_blinker_state;
			Global_current_blinker_state = BLINKERS_HAZARD;
			TRACE_INFO(TRACE_BLINKER,"HAZARD on");
			break;
			
			case LEFT_BLINKER_REQUEST:
			if(Ignition_GetState() == IGNITION_OFF)		// Ignition OFF: Request rejected
			{
				TRACE_ERROR(TRACE_BLINKER,"Blinker Request Rejected (IGNITION_OFF)");
			}
			else
			{
				Global_previous_blinker_state = Global_current_blinker_state;
				Global_current_blinker_state = BLINKERS_LEFT;
				TRACE_INFO(TRACE_BLINKER,"LEFT Blinkers on");
			}
			break;
			
			case RIGHT_BLINKER_REQUEST:
			if(Ignition_GetState() == IGNITION_OFF)		// Ignition OFF: Request rejected
			{
				TRACE_ERROR(TRACE_BLINKER,"Blinker Request Rejected (IGNITION_OFF)");
			}
			else
			{
				Global_previous_blinker_state = Global_current_blinker_state;
				Global_current_blinker_state = BLINKERS_RIGHT;
				TRACE_INFO(TRACE_BLINKER,"RIGHT Blinkers on");
			}
			break;
			
			case BLINKER_OFF_REQUEST:		// Blinkers OFF
			Global_previous_blinker_state = Global_current_blinker_state;
			Global_current_blinker_state = BLINKERS_OFF;
			TRACE_INFO(TRACE_BLINKER,"Blinkers OFF");
			break;
		}
	}
}

void Blinker_StateMachine_Update(void)
{
	// No periodic tasks needed for basic state tracking
	
	// TODO: if physical output simulation
}

blinkers_state_t Blinker_GetState(void)
{
	return Global_current_blinker_state;
}

blinkers_state_t Blinker_GetPrevState(void)
{
	return Global_previous_blinker_state;
}