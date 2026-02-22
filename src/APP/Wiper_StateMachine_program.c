/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "Wiper_StateMachine_interface.h"
#include "ECU_StateMachine_interface.h"
#include "Ignition_StateMachine_interface.h"
#include "EventQueue_interface.h"
#include "DiagnosticPolicy.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// Timer for Auto Wiper off if ignition changed
#define AUTO_WIPER_OFF_TIMEOUT_MS  5000  // 5 seconds


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Global Var to the state of the Wipers
static wiper_control_state_t Global_current_wiper_state = WIPERS_OFF;

//	Ref to the Global event Queue
extern Events_Queue_t GlobalEventQueue;

// Counter for the Auto Wipers off if ignition changed
static u16 auto_wiper_off_counter = 0;

/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Wiper_StateMachine_Init(void)
{
	Global_current_wiper_state = WIPERS_OFF;
	auto_wiper_off_counter = 0;
	TRACE_INFO(TRACE_WIPER,"Wipers Initialized to WIPERS_OFF");
}

void Wiper_StateMachine_ProcessEvent(ecu_event_t event)
{
	if(ECU_GetOperationalState() == ECU_OP_STATE_DIAGNOSTIC)	// Diag Mode
	{
		if(DIAG_ALLOW_WIPER_CONTROL)
		{
			switch(event)
			{
				case WIPER_INT_REQUEST:
				Global_current_wiper_state = WIPERS_INTERMITTENT;
				TRACE_INFO(TRACE_WIPER,"Wipers Set to INTERMITTENT (Diag)");
				break;
				
				case WIPER_LOW_REQUEST:
				Global_current_wiper_state = WIPERS_LOW;
				TRACE_INFO(TRACE_WIPER,"Wipers Set to LOW (Diag)");
				break;
				
				case WIPER_HIGH_REQUEST:
				Global_current_wiper_state = WIPERS_HIGH;
				TRACE_INFO(TRACE_WIPER,"Wipers Set to HIGH (Diag)");
				break;
				
				case WIPER_OFF_REQUEST:
				Global_current_wiper_state = WIPERS_OFF;
				TRACE_INFO(TRACE_WIPER,"Wipers Set to OFF (Diag)");
				break;
			}
		}
		else
		{
			TRACE_ERROR(TRACE_WIPER,"Wipers Are not allowed in DIAG");
		}
	}
	else		// Normal OP Mode
	{
		switch(event)
		{
			case WIPER_INT_REQUEST:
			case WIPER_LOW_REQUEST:
			case WIPER_HIGH_REQUEST:
			if(Ignition_GetState() == IGNITION_ON)
			{
				switch(event)
				{
					case WIPER_INT_REQUEST:
					Global_current_wiper_state = WIPERS_INTERMITTENT;
					TRACE_INFO(TRACE_WIPER,"Wipers Set to INTERMITTENT");
					break;
					
					case WIPER_LOW_REQUEST:
					Global_current_wiper_state = WIPERS_LOW;
					TRACE_INFO(TRACE_WIPER,"Wipers Set to LOW");
					break;
					
					case WIPER_HIGH_REQUEST:
					Global_current_wiper_state = WIPERS_HIGH;
					TRACE_INFO(TRACE_WIPER,"Wipers Set to HIGH");
					break;
				}
			}
			else
			{
				TRACE_ERROR(TRACE_WIPER,"Wipers Are only allowed in IGNITION_ON");
			}
		}
		break;
		
		case WIPER_OFF_REQUEST:
		Global_current_wiper_state = WIPERS_OFF;
		TRACE_INFO(TRACE_WIPER,"Wipers Set to OFF");
		break;
	}
}

void Wiper_StateMachine_Update(void)
{
	if(Ignition_GetState() != IGNITION_ON)
	{
		if(Global_current_wiper_state > WIPERS_OFF)	// if wipers are ON
		{
			if(auto_wiper_off_counter < AUTO_WIPER_OFF_TIMEOUT_MS)		// if counter isn't reached
			{
				auto_wiper_off_counter++;
			}
			else	// Auto off counter is reached
			{
				EVENTQUEUE_u8enQueue(&GlobalEventQueue,WIPER_OFF_REQUEST);
				auto_wiper_off_counter = 0;
				TRACE_INFO(TRACE_WIPER,"Wipers Set to off (Auto-off)");
			}
		}
	}
	else		// Ignition Back to ON
	{
		auto_wiper_off_counter = 0;
	}
}

wiper_control_state_t Wiper_GetState(void)
{
	return Global_current_wiper_state;
}