/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Events.h"
#include "EventQueue_interface.h"
#include "InputManager.h"
#include "VehicleManager.h"
#include "NVM_Manager_interface.h"
#include "VehicleMovement_StateMachine_interface.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
#define DIAGNOSTIC_TIMEOUT_MS    600000    //	10 Minutes


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
static ecu_state_t Global_current_ecu_state = ECU_STATE_STARTUP;					// STARTUP/ACTIVE
static ecu_operational_state_t Global_operational_state = ECU_OP_STATE_NORMAL;		// NORMAL/DIAGNOSTIC/DEGRADED
static s32 diagnostic_timeout_counter = 0;											// Countdown timer for diag timeout

//Reset Function
void(*resetFunc)(void) = 0;

/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void ECU_StateMachine_Init(void)
{
	Global_current_ecu_state = ECU_STATE_ACTIVE;
	if(!Fault_Monitor_HasLatchedFaults())
	{
		Global_operational_state = ECU_OP_STATE_NORMAL;
	}
	else
	{
		Global_operational_state = ECU_OP_STATE_DEGRADED;
	}
	diagnostic_timeout_counter = 0;
	TRACE_INFO(TRACE_ECU,"ECU Initialized");
	
}

void ECU_StateMachine_ProcessEvent(ecu_event_t event)
{
	if(event == ECU_RESET_EVENT)
	{
		// Save to Nvm
		NVM_Manager_Save();
		TRACE_INFO(TRACE_ECU, "Reseting ECU...");
		
		resetFunc();				
	}
	switch(Global_operational_state)
	{
		case ECU_OP_STATE_NORMAL:
			switch(event)
			{
				case TECHNICIAN_DIAG_REQUEST:
				// TODO: check parameters to enter diag mode
				if(VehicleMovement_GetState() == STOPPED)
				{
					Global_operational_state = ECU_OP_STATE_DIAGNOSTIC;
					diagnostic_timeout_counter = DIAGNOSTIC_TIMEOUT_MS;
					TRACE_INFO(TRACE_ECU,"Entered Diag mode");
				}
				break;
				
				case FAULT_CRITICAL_EVENT:
				Global_operational_state = ECU_OP_STATE_DEGRADED;
				
				// Log fault to NVM
				NVM_Manager_Save();
				
				TRACE_INFO(TRACE_ECU,"Entered Degraded mode");
				break;
				
				default:
				// Do Nothing
				break;
			}
			
		break;
			
		case ECU_OP_STATE_DIAGNOSTIC:
			switch(event)
			{
				case ECU_EXIT_DIAG_REQUEST:
				case DIAG_TIMEOUT_EVENT:
				Global_operational_state = ECU_OP_STATE_NORMAL;
				diagnostic_timeout_counter = 0;
				break;
			
				case FAULT_CRITICAL_EVENT:
				Global_operational_state = ECU_OP_STATE_DEGRADED;
				TRACE_ERROR(TRACE_ECU,"Entered Degraded mode");
				
				// Log fault to NVM
				NVM_Manager_Save();
				break;
				
				default:
				// Do Nothing
				break;
			}
		
	    break;
		
		case ECU_OP_STATE_DEGRADED:
		switch(event)
		{
			case FAULT_CLEARED_EVENT:
			Global_operational_state = ECU_OP_STATE_NORMAL;
			TRACE_INFO(TRACE_ECU,"Fault cleared");
			break;
			
			case TECHNICIAN_DIAG_REQUEST:
			// TODO: check parameters to enter diag mode
			if(VehicleMovement_GetState() == STOPPED)
			{
				Global_operational_state = ECU_OP_STATE_DIAGNOSTIC;
				diagnostic_timeout_counter = DIAGNOSTIC_TIMEOUT_MS;
				TRACE_INFO(TRACE_ECU,"Entered Diag mode");
			}
			break;
			
			default:
			// Do Nothing
			break;
		}
	    break;	
		
		default:
		// Do Nothing
		break;
	}
}

void ECU_StateMachine_Update(u16 elapsedTime_ms)
{
	if(Global_operational_state == ECU_OP_STATE_DIAGNOSTIC)
	{
		if(diagnostic_timeout_counter > 0)
		{
			diagnostic_timeout_counter -= elapsedTime_ms;
		}
		else
		{
			EVENTQUEUE_u8enQueue(EventQueue_Get(),DIAG_TIMEOUT_EVENT);
		}
	}
}

ecu_state_t ECU_GetState(void)
{
	return Global_current_ecu_state;
}

ecu_operational_state_t ECU_GetOperationalState(void)
{
	return Global_operational_state;
}
