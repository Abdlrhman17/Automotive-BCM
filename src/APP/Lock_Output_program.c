/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "Lock_Output_interface.h"
#include "Lock_StateMachine_interface.h"
#include "DIO_interface.h"
#include "EventQueue_interface.h"
#include "Events.h"
#include "Trace.h"
#include "StdTypes.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
// GPIO pins for lock actuators
#define LOCK_ACTUATOR_PIN    DIO_PINB1
#define UNLOCK_ACTUATOR_PIN  DIO_PINB2

// Duration for the actuators to delay
#define ACTUATORS_DELAY_TIMEOUT_MS		300		// 300 MS


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// Counter to monitor actuators delay
static u16 actuators_delay_counter = 0;

// Local Var to track the last Lock state
static door_lock_state_t prev_Lock_state = LOCKED;

// Local Var to determine motor activity (Essential for control)
static u8 motor_active = 0;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void Lock_Output_Init(void)
{	
	// Initialized to Locked
	EVENTQUEUE_u8enQueue(EventQueue_Get(), LOCK_REQUEST_EVENT);
	actuators_delay_counter = 0;
	motor_active = 0;
	TRACE_INFO(TRACE_LOCK,"Lock output initialized");
}

void Lock_Output_Update(u8 elapsedTime_ms)
{
	door_lock_state_t local_DoorLockState = Lock_GetState();
	
	//	Detect changes
	if(local_DoorLockState != prev_Lock_state)
	{
		prev_Lock_state = local_DoorLockState;
		motor_active = 1;
		actuators_delay_counter = 0;
	}
	
	if(motor_active)	// Evaluate the change
	{
		if(local_DoorLockState == UNLOCKED)
		{
			DIO_WritePin(LOCK_ACTUATOR_PIN,LOW);
			DIO_WritePin(UNLOCK_ACTUATOR_PIN,HIGH);
		}
		else
		{
			DIO_WritePin(LOCK_ACTUATOR_PIN,HIGH);
			DIO_WritePin(UNLOCK_ACTUATOR_PIN,LOW);
		}
		
		if(actuators_delay_counter < ACTUATORS_DELAY_TIMEOUT_MS)	// Check for time-out
		{
			actuators_delay_counter += elapsedTime_ms;
		}
		else	//	Time-out Reached
		{
			//	Stop the motor
			DIO_WritePin(LOCK_ACTUATOR_PIN,LOW);
			DIO_WritePin(UNLOCK_ACTUATOR_PIN,LOW);
			
			motor_active = 0;
		}
	}
}

