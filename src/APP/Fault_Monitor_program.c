/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "Fault_Monitor_interface.h"
#include "EventQueue_interface.h"
#include "Ignition_StateMachine_interface.h"
#include "Door_StateMachine_interface.h"
#include "Lock_StateMachine_interface.h"
#include "ECU_StateMachine_interface.h"
#include "Events.h"
#include "Trace.h"
#include "DIO_interface.h"
// TODO: Add includes for reading inputs/outputs

/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// Total number of fault types
#define FAULT_COUNT  6 

// Fault detection thresholds
#define IGNITION_SIGNAL_MIN    0
#define IGNITION_SIGNAL_MAX    3
#define DOOR_DEBOUNCE_FAIL_THRESHOLD  4		// Bouncing too much
#define DOOR_DEBOUNCE_FAIL_WINDOW     1000	// 1 Second
#define LOCK_DEBOUNCE_FAIL_THRESHOLD  4		// Bouncing too much
#define LOCK_DEBOUNCE_FAIL_WINDOW     300	// 300 MS
/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Fault table - tracks all faults
static fault_t fault_table[FAULT_COUNT];

// Counts changes in the door sensor in 1 Second
static u8 door_sensor_change_counter = 0;

// Var for the prev door sensor state
static door_state_t prev_door_state = DOOR_CLOSED;

// Counter for monitoring door debouncing window
static u16 door_debounce_counter = 0;

// Var for the prev door lock state
static door_lock_state_t prev_lock_state = LOCKED;

// Counts changes in the door lock in 300 MS
static u8 door_lock_change_counter = 0;

// Counter for monitoring door lock debouncing window
static u16 lock_debounce_counter = 0;

// Reference to global event queue
extern Events_Queue_t GlobalEventQueue;

/* ============================================ */
/*       PRIVATE FUNCTION PROTOTYPES            */
/* ============================================ */
static u8 CheckIgnitionSignalFault(void);
static u8 CheckDoorSensorFault(void);
static u8 CheckLockActuatorFault(void);
static u8 CheckBlinkerOutputFault(void);
static u8 CheckWiperOutputFault(void);
static u8 CheckSpeedSignalFault(void);
static void ActivateFault(fault_id_t fault_id);
static void ClearFaultInternal(fault_id_t fault_id);

u8 (*CheckFaultArr[FAULT_COUNT])(void) = {
	CheckIgnitionSignalFault,
	CheckDoorSensorFault,
	CheckLockActuatorFault,
	CheckBlinkerOutputFault,
	CheckWiperOutputFault,
	CheckSpeedSignalFault
};											
											
/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Fault_Monitor_Init(void)
{
	// TODO: Load faults from NVM if there is any
	
    /* IGNITION SIGNAL FAULT */
    fault_table[IGNITION_SIGNAL_FAULT].faultID = IGNITION_SIGNAL_FAULT;
    fault_table[IGNITION_SIGNAL_FAULT].faultSeverity = FAULT_SEVERITY_CRITICAL;
    fault_table[IGNITION_SIGNAL_FAULT].faultPersistence = FAULT_LATCHED;
    fault_table[IGNITION_SIGNAL_FAULT].is_Active = FALSE;

    /* DOOR SENSOR FAULT */
    fault_table[DOOR_SENSOR_FAULT].faultID = DOOR_SENSOR_FAULT;
    fault_table[DOOR_SENSOR_FAULT].faultSeverity = FAULT_SEVERITY_NON_CRITICAL;
    fault_table[DOOR_SENSOR_FAULT].faultPersistence = FAULT_VOLATILE;
    fault_table[DOOR_SENSOR_FAULT].is_Active = FALSE;

    /* LOCK ACTUATOR FAULT */
    fault_table[LOCK_ACTUATOR_FAULT].faultID = LOCK_ACTUATOR_FAULT;
    fault_table[LOCK_ACTUATOR_FAULT].faultSeverity = FAULT_SEVERITY_CRITICAL;
    fault_table[LOCK_ACTUATOR_FAULT].faultPersistence = FAULT_LATCHED;
    fault_table[LOCK_ACTUATOR_FAULT].is_Active = FALSE;

    /* BLINKER OUTPUT FAULT */
    fault_table[BLINKER_OUTPUT_FAULT].faultID = BLINKER_OUTPUT_FAULT;
    fault_table[BLINKER_OUTPUT_FAULT].faultSeverity = FAULT_SEVERITY_NON_CRITICAL;
    fault_table[BLINKER_OUTPUT_FAULT].faultPersistence = FAULT_VOLATILE;
    fault_table[BLINKER_OUTPUT_FAULT].is_Active = FALSE;

    /* WIPER OUTPUT FAULT */
    fault_table[WIPER_OUTPUT_FAULT].faultID = WIPER_OUTPUT_FAULT;
    fault_table[WIPER_OUTPUT_FAULT].faultSeverity = FAULT_SEVERITY_NON_CRITICAL;
    fault_table[WIPER_OUTPUT_FAULT].faultPersistence = FAULT_VOLATILE;
    fault_table[WIPER_OUTPUT_FAULT].is_Active = FALSE;

    /* SPEED SIGNAL FAULT */
    fault_table[SPEED_SIGNAL_FAULT].faultID = SPEED_SIGNAL_FAULT;
    fault_table[SPEED_SIGNAL_FAULT].faultSeverity = FAULT_SEVERITY_SAFETY;
    fault_table[SPEED_SIGNAL_FAULT].faultPersistence = FAULT_LATCHED;
    fault_table[SPEED_SIGNAL_FAULT].is_Active = FALSE;
}

void Fault_Monitor_Update(void)
{	
	CheckIgnitionSignalFault();
	CheckDoorSensorFault();
	CheckLockActuatorFault();
	CheckBlinkerOutputFault();
	CheckWiperOutputFault();
	CheckSpeedSignalFault();
}

fault_t Fault_Monitor_GetFault(fault_id_t fault_id)
{
	return fault_table[fault_id];
}

u8 Fault_Monitor_HasSafetyFaults(void)
{
	u8 i;
	for(i = 0; i < FAULT_COUNT; ++i)
	{
		if(fault_table[i].is_Active)
		{
			if(fault_table[i].faultSeverity == FAULT_SEVERITY_SAFETY)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void Fault_Monitor_ClearFault(fault_id_t fault_id)
{
	if(fault_table[fault_id].faultPersistence == FAULT_LATCHED && 
	ECU_GetOperationalState() == ECU_OP_STATE_DIAGNOSTIC)
	{
		if(CheckFaultArr[fault_id]() == TRUE) //Fault still present
		{
			TRACE_ERROR(TRACE_FAULT,"Cannot Clear Fault: Fault is still active");		
		}
		else	// Fault is not active
		{
			fault_table[fault_id].is_Active = FALSE;
			TRACE_INFO(TRACE_FAULT, "Latched fault manually cleared");
		}
	}
}


/* ============================================ */
/*      PRIVATE FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

static u8 CheckIgnitionSignalFault(void)
{
	ignition_state_t LocalIgnition = Ignition_GetState();
	
	// Check if signal is invalid
	u8 fault_present = (LocalIgnition > IGNITION_START);
	
	if(fault_present)
	{
		if(!fault_table[IGNITION_SIGNAL_FAULT].is_Active)
		{
			ActivateFault(IGNITION_SIGNAL_FAULT);
		}
		return TRUE;  // Fault is present
	}
	else
	{
		if(fault_table[IGNITION_SIGNAL_FAULT].is_Active)
		{
			ClearFaultInternal(IGNITION_SIGNAL_FAULT);
		}
		return FALSE;  // Fault is not present
	}
}

static u8 CheckDoorSensorFault(void)
{	
	door_state_t LocalcurrentDoorState = Door_GetState();
	
	/* Count state changes */
	if(LocalcurrentDoorState != prev_door_state)
	{
		door_sensor_change_counter++;
		prev_door_state = LocalcurrentDoorState;
	}

	/* Time window counting */
	door_debounce_counter++;

	if(door_debounce_counter >= DOOR_DEBOUNCE_FAIL_WINDOW)
	{
		if(door_sensor_change_counter >= DOOR_DEBOUNCE_FAIL_THRESHOLD)
		{
			if(!fault_table[DOOR_SENSOR_FAULT].is_Active)
			{
				ActivateFault(DOOR_SENSOR_FAULT);
			}
			return TRUE;
		}
		
		else
		{
			// No fault: Clear if was active
			if(fault_table[DOOR_SENSOR_FAULT].is_Active)
			{
				ClearFaultInternal(DOOR_SENSOR_FAULT);
			}
		}

		/* Reset window */
		door_debounce_counter = 0;
		door_sensor_change_counter = 0;
	}
	return (door_sensor_change_counter >= DOOR_DEBOUNCE_FAIL_THRESHOLD);
}

static u8 CheckLockActuatorFault(void)
{
	door_lock_state_t LocalCurrentLockState = Lock_GetState();
	
	/* Count state changes */
	if(LocalCurrentLockState != prev_lock_state)
	{
		door_lock_change_counter++;
		prev_lock_state = LocalCurrentLockState;
	}

	/* Time window counting */
	lock_debounce_counter++;

	if(lock_debounce_counter >= LOCK_DEBOUNCE_FAIL_WINDOW)
	{
		if(door_lock_change_counter >= LOCK_DEBOUNCE_FAIL_THRESHOLD)
		{
			if(!fault_table[LOCK_ACTUATOR_FAULT].is_Active)
			{
				ActivateFault(LOCK_ACTUATOR_FAULT);
			}
			return TRUE;
		}
		
		else
		{
			// No fault: Clear if was active
			if(fault_table[LOCK_ACTUATOR_FAULT].is_Active)
			{
				ClearFaultInternal(LOCK_ACTUATOR_FAULT);
			}
		}

		/* Reset window */
		lock_debounce_counter = 0;
		door_lock_change_counter = 0;
	}
	
	// TODO: Add time-out checking to see if the change applied to the state after command
	
	return (door_lock_change_counter >= LOCK_DEBOUNCE_FAIL_THRESHOLD);
}

static u8 CheckBlinkerOutputFault(void)
{
	// TODO: Add feedback & Current sensing (if available)
	return FALSE;
}

static u8 CheckWiperOutputFault(void)
{
	// TODO: Implement wiper motor overcurrent detection
	return FALSE;
}

static u8 CheckSpeedSignalFault(void)
{
	// - Timeout counter
	// - Reset counter on each speed event
	// - If counter exceeds threshold: signal lost
	
	// For now:
	// TRACE_DEBUG(TRACE_FAULT, "Speed signal check - OK");
	return FALSE;
}

static void ActivateFault(fault_id_t fault_id)
{
	fault_table[fault_id].is_Active = TRUE;
	
	TRACE_ERROR(TRACE_FAULT, "Fault activated");
	
	if(fault_table[fault_id].faultSeverity == FAULT_SEVERITY_SAFETY)
	{
		EVENTQUEUE_u8enQueue(&GlobalEventQueue, FAULT_CRITICAL_EVENT);
		TRACE_ERROR(TRACE_FAULT, "SAFETY fault - ECU entering DEGRADED");
	}
	
	// TODO: Log to NVM for latched faults
}

static void ClearFaultInternal(fault_id_t fault_id)
{
	if(fault_table[fault_id].faultPersistence == FAULT_VOLATILE)
	{
		fault_table[fault_id].is_Active = FALSE;
		TRACE_INFO(TRACE_FAULT, "Volatile fault cleared");
		
		// Check if no more SAFETY faults active
		if(!Fault_Monitor_HasSafetyFaults())
		{
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, FAULT_CLEARED_EVENT);
			TRACE_INFO(TRACE_FAULT, "All safety faults cleared");
		}
	}
	// LATCHED faults must be manually cleared via service
}
