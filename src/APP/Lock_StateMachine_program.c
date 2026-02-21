/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Lock_StateMachine_interface.h"
#include "Door_StateMachine_interface.h"
#include "Trace.h"
#include "ECU_StateMachine_interface.h"
#include "DiagnosticPolicy.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// Timer for Auto lock feature when stopped
#define AUTO_LOCK_TIMEOUT_MS  10000  // 10 seconds


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Global Var to the door lock state
static door_lock_state_t Global_current_lock_state = UNLOCKED;

// Counter for the Auto Lock
static u16 auto_lock_counter = 0; 

// Ref to Global Event Queue
extern Events_Queue_t GlobalEventQueue;

// Ref to Global Vehicle state
extern vehicle_movement_state_t Global_vehicle_movement;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Lock_StateMachine_Init(void)
{
	Global_current_lock_state = UNLOCKED;
	auto_lock_counter = 0;
	
	// TODO: Load last lock state from NVM

	TRACE_INFO(TRACE_LOCK,"Lock Initialized");
}

void Lock_StateMachine_ProcessEvent(ecu_event_t event)
{
	if(ECU_GetOperationalState() == ECU_OP_STATE_DIAGNOSTIC && DIAG_ALLOW_DOOR_LOCK_CONTROL) // Diag mode override
	{
		switch(event)
		{
			case LOCK_REQUEST_EVENT:
			Global_current_lock_state = LOCKED;		// LOCKED
			TRACE_INFO(TRACE_LOCK,"Door Locked in DIAG");
			break;
					
			case UNLOCK_REQUEST_EVENT:
			Global_current_lock_state = UNLOCKED;	// UNLOCKED
			TRACE_INFO(TRACE_LOCK,"Door Unlocked in DIAG");
			break;
		}
	}

	else
	{
		switch(Global_current_lock_state)
		{
			case UNLOCKED:
			if(event == LOCK_REQUEST_EVENT)
			{
				if(Door_GetState() == DOOR_CLOSED)	// if door are locked
				{
					// Safe to lock
					Global_current_lock_state = LOCKED;		// LOCKED
					TRACE_INFO(TRACE_LOCK,"Door locked");
				}
				else
				{
					// Reject Request
					TRACE_ERROR(TRACE_LOCK,"Cannot lock - Door Open");
				}
			}
			break;
			
			case LOCKED:
			if(event == UNLOCK_REQUEST_EVENT)
			{
				// Allow manual unlock
				Global_current_lock_state = UNLOCKED;	// UNLOCKED
				TRACE_INFO(TRACE_LOCK,"Door Unlocked");
			}
			else if(event == VEHICLE_MOVING_EVENT)
			{
				if(ECU_GetOperationalState() != ECU_OP_STATE_DIAGNOSTIC)
				{
					// Safety: Auto unlock if the vehicle is moving & Lock is LOCKED 
					Global_current_lock_state = UNLOCKED;
					TRACE_INFO(TRACE_LOCK,"Auto unlocked for safety");
				}
			}
		}
	}
}

void Lock_StateMachine_Update(void)
{
	// Auto-lock feature: Lock after timeout when stopped
	if(Global_current_lock_state == UNLOCKED)
	{
		if(Global_vehicle_movement == STOPPED)
		{
			if(auto_lock_counter < AUTO_LOCK_TIMEOUT_MS)
			{
				auto_lock_counter++;
			}
			else
			{
				// Timeout reached - generate lock request
				EVENTQUEUE_u8enQueue(&GlobalEventQueue, LOCK_REQUEST_EVENT);
				auto_lock_counter = 0;
			}
		}
		else  // Vehicle is MOVING
		{
			auto_lock_counter = 0;  // Reset if vehicle moving
		}
	}
	else  // Already LOCKED
	{
		auto_lock_counter = 0;  // Reset if already locked
	}
}
door_lock_state_t Lock_GetState(void)
{
	return Global_current_lock_state;
}