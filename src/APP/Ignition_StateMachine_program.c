/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Ignition_StateMachine_interface.h"
#include "EventQueue_interface.h"
#include "Trace.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// Counter to the duration for IGNITION to return from IGNITION_START to IGNITION_ON
#define START_CRANK_DURATION_MS 3000


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */

//	Global Var to the ignition state
static ignition_state_t Global_current_ignition_state = IGNITION_OFF;

//	Counter to get back from IGNITION_START to IGNITION_ON
static u16 start_position_counter = 0;

//	Ref to the Global event Queue
extern Events_Queue_t GlobalEventQueue;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Ignition_StateMachine_Init(void)
{
	Global_current_ignition_state = IGNITION_OFF;
	start_position_counter = 0;
	TRACE_INFO(TRACE_IGNITION,"Ignition Initialized");
}

void Ignition_StateMachine_ProcessEvent(ecu_event_t event)
{
	switch(Global_current_ignition_state)
	{
		case IGNITION_OFF:
		if(event == IGNITION_ACC_EVENT)
		{
			Global_current_ignition_state = IGNITION_ACC;	// OFF -> ACC
			TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_ACC");
		}
		else
		{
			TRACE_ERROR(TRACE_IGNITION, "Invalid ignition transition");
		}
		break;
		
		case IGNITION_ACC:
		if(event == IGNITION_ON_EVENT)
		{
			Global_current_ignition_state = IGNITION_ON;	// ACC -> ON
			TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_ON");
		}
		else if(event == IGNITION_OFF_EVENT)
		{
			Global_current_ignition_state = IGNITION_OFF;	//ACC -> OFF
			TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_OFF");
		}
		else
		{
			TRACE_ERROR(TRACE_IGNITION, "Invalid ignition transition");
		}
		break;
		
		case IGNITION_ON:
		if(event == IGNITION_START_EVENT)
		{
			if(ECU_GetOperationalState() == ECU_OP_STATE_DIAGNOSTIC)
			{
				TRACE_ERROR(TRACE_IGNITION, "Cannot START in diagnostic mode");
			}
			else
			{
				Global_current_ignition_state = IGNITION_START;	// ON -> START
				start_position_counter = START_CRANK_DURATION_MS;
				TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_START");
			}
		}
		
		else if(event == IGNITION_ACC_EVENT)
		{
			Global_current_ignition_state = IGNITION_ACC;	// ON -> ACC
			TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_ACC");
		}
		else
		{
			TRACE_ERROR(TRACE_IGNITION, "Invalid ignition transition");
		}
		break;
		
		case IGNITION_START:
		if(event == IGNITION_ON_EVENT)
		{
			Global_current_ignition_state = IGNITION_ON;	// START -> ON
			start_position_counter = 0;
			TRACE_INFO(TRACE_IGNITION,"Ignition to IGNITION_ON");
		}
		else
		{
			TRACE_ERROR(TRACE_IGNITION, "Invalid ignition transition");
		}
	}
}

void Ignition_StateMachine_Update(void)
{
	if(Global_current_ignition_state == IGNITION_START)
	{
		if(start_position_counter > 0)
		{
			start_position_counter--;
		}
		else
		{
			start_position_counter = START_CRANK_DURATION_MS; 
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,IGNITION_ON_EVENT);
			TRACE_INFO(TRACE_IGNITION, "START auto-returned to ON");
		}
	}
}

ignition_state_t Ignition_GetState(void)
{
	return Global_current_ignition_state;
}

// TODO:
// If there's an ignition signal fault:
// 
// Reject all transitions
// Lock current state
// Wait for fault clear