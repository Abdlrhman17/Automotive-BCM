#include "StdTypes.h"
#include "Events.h"
#include "EventQueue_interface.h"
#include "InputManager.h"
#include "VehicleManager.h"
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
static u16 diagnostic_timeout_counter = 0;											// Countdown timer for diag timeout

// GLobal Events Queue
extern Events_Queue_t GlobalEventQueue;

// Global vehicle movement state Var
extern vehicle_movement_state_t Global_vehicle_movement;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void ECU_StateMachine_Init(void)
{
	Global_current_ecu_state = ECU_STATE_ACTIVE;
	if(TRUE) // TODO: Check for faults in NVM
	{
		Global_operational_state = ECU_OP_STATE_NORMAL;
	}
	else
	{
		Global_operational_state = ECU_OP_STATE_DEGRADED;
	}
	diagnostic_timeout_counter = 0;
	TRACE_INFO(TRACE_ECU,"ECU Initialized");
	
	//TODO: LOAD NVM DATA	
}

void ECU_StateMachine_ProcessEvent(ecu_event_t event)
{
	if(event == ECU_RESET_EVENT)
	{
		//TODO: ISR to Reset
	}
	switch(Global_operational_state)
	{
		case ECU_OP_STATE_NORMAL:
			switch(event)
			{
				case TECHNICIAN_DIAG_REQUEST:
				// TODO: check parameters to enter diag mode
				if(Global_vehicle_movement == STOPPED)
				{
					Global_operational_state = ECU_OP_STATE_DIAGNOSTIC;
					diagnostic_timeout_counter = DIAGNOSTIC_TIMEOUT_MS;
					TRACE_INFO(TRACE_ECU,"Entered Diag mode");
				}
				break;
				
				case FAULT_CRITICAL_EVENT:
				Global_operational_state = ECU_OP_STATE_DEGRADED;
				
				// TODO: Log fault to NVM
				
				TRACE_INFO(TRACE_ECU,"Entered Degraded mode");
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
				break;
			}
		
	    break;
		
		case ECU_OP_STATE_DEGRADED:
		switch(event)
		{
			case FAULT_CLEARED_EVENT:
			Global_operational_state = ECU_OP_STATE_NORMAL;
			
			// TODO: Clear fault from NVM
			
			TRACE_INFO(TRACE_ECU,"Fault cleared");
			break;
			
			case TECHNICIAN_DIAG_REQUEST:
			// TODO: check parameters to enter diag mode
			if(Global_vehicle_movement == STOPPED)
			{
				Global_operational_state = ECU_OP_STATE_DIAGNOSTIC;
				diagnostic_timeout_counter = DIAGNOSTIC_TIMEOUT_MS;
				TRACE_INFO(TRACE_ECU,"Entered Diag mode");
			}
			break;
		}
	    break;	
	}
}

void ECU_StateMachine_Update(void)
{
	if(Global_operational_state == ECU_OP_STATE_DIAGNOSTIC)
	{
		if(diagnostic_timeout_counter > 0)
		{
			diagnostic_timeout_counter--;
		}
		else
		{
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,DIAG_TIMEOUT_EVENT);
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