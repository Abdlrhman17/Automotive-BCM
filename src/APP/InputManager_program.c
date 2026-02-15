/*
 * InputManager_program.c
 *
 * Purpose: Sample hardware inputs and generate events on state changes
 * 
 * Responsibilities:
 *   - Initialize input states
 *   - Periodically sample all inputs
 *   - Detect changes (edge detection)
 *   - Generate appropriate events
 *   - Handle debouncing for mechanical switches
 *
 * RULES:
 *   - This module ONLY reads and detects changes
 *   - NO validation logic
 *   - NO business rules
 *   - Just sensing and event generation
 */

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "InputManager.h"
#include "Events.h"
#include "EventQueue_interface.h"
#include "StdTypes.h"
#include "DIO_interface.h"
#include "ADC_interface.h"


/* ============================================ */
/*          PRIVATE DEFINES                     */
/* ============================================ */
#define DEBOUNCE_THRESHOLD_MS      50    // Mechanical switches need debouncing
#define IGNITION_STABLE_TIME_MS    20    // How long ignition must be stable


/* ============================================ */
/*          PRIVATE TYPES                       */
/* ============================================ */

// Structure to track previous state of each input
typedef struct
{
    u8 previous_value;      // Last stable value
    u8 current_value;       // Current raw reading
    u16 stable_counter;     // Debounce counter
}input_state_t;


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */

// Previous states for edge detection
static input_state_t ignition_state;
static input_state_t door_state;
static input_state_t lock_button_state;
static input_state_t blinker_stalk_state;
static input_state_t wiper_stalk_state;
static input_state_t speed_signal_state;

// Reference to the global event queue (extern or passed in init)
extern Events_Queue_t GlobalEventQueue;  // Or however you manage this


/* ============================================ */
/*          SIMULATION VARIABLES                */
/* ============================================ */
// These replace actual hardware until you have it
static u8 sim_ignition_position = 0;     // 0=OFF, 1=ACC, 2=ON, 3=START
static u8 sim_door_sensor = 0;           // 0=CLOSED, 1=OPEN
static u8 sim_lock_button = 2;           // 0=LOCK, 1=UNLOCK, 2=NONE
static u8 sim_blinker_stalk = 0;         // 0=OFF, 1=LEFT, 2=RIGHT, 3=HAZARD
static u8 sim_wiper_stalk = 0;           // 0=OFF, 1=INT, 2=LOW, 3=HIGH
static u8 sim_speed = 0;                 // 0=STOPPED, 1=MOVING


/* ============================================ */
/*       PRIVATE FUNCTION PROTOTYPES            */
/* ============================================ */

/**
 * Read raw ignition switch position from hardware
 * Returns: 0=OFF, 1=ACC, 2=ON, 3=START
 */
static u8 ReadIgnitionSwitch_Hardware(void);

/**
 * Read raw door sensor state from hardware
 * Returns: 0=CLOSED, 1=OPEN
 */
static u8 ReadDoorSensor_Hardware(void);
/**
 * Read lock button state from hardware
 * Returns: 0=LOCK_PRESSED, 1=UNLOCK_PRESSED, 2=NONE
 */
static u8 ReadLockButton_Hardware(void);

/**
 * Read blinker stalk position from hardware
 * Returns: 0=OFF, 1=LEFT, 2=RIGHT, 3=HAZARD
 */
static u8 ReadBlinkerStalk_Hardware(void);

/**
 * Read wiper stalk position from hardware
 * Returns: 0=OFF, 1=INT, 2=LOW, 3=HIGH
 */
static u8 ReadWiperStalk_Hardware(void);

/**
 * Read vehicle speed signal from hardware
 * Returns: 0=STOPPED, 1=MOVING
 */
static u8 ReadSpeedSignal_Hardware(void);

/**
 * Debounce a mechanical input
 * Returns: TRUE if stable, FALSE if still bouncing
 */
static u8 DebounceInput(input_state_t* input, u8 raw_value);

/**
 * Process ignition switch changes and generate events
 */
static void ProcessIgnitionInput(void);

/**
 * Process door sensor changes and generate events
 */
static void ProcessDoorInput(void);

/**
 * Process lock button presses and generate events
 */
static void ProcessLockButtonInput(void);

/**
 * Process blinker stalk changes and generate events
 */
static void ProcessBlinkerInput(void);

/**
 * Process wiper stalk changes and generate events
 */
static void ProcessWiperInput(void);

/**
 * Process speed signal changes and generate events
 */
static void ProcessSpeedInput(void);


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void InputManager_Init(void)
{
    //Initialize all input states to a known value
	
    ignition_state.previous_value = ReadIgnitionSwitch_Hardware();
    ignition_state.current_value = ignition_state.previous_value;
    ignition_state.stable_counter = 0;
	
	door_state.previous_value = ReadDoorSensor_Hardware();
	door_state.current_value = door_state.previous_value;
	door_state.stable_counter = 0;
	
	lock_button_state.previous_value = ReadLockButton_Hardware();
	lock_button_state.current_value = lock_button_state.previous_value;
	lock_button_state.stable_counter = 0;
	
	blinker_stalk_state.previous_value = ReadBlinkerStalk_Hardware();
	blinker_stalk_state.current_value = blinker_stalk_state.previous_value;
	blinker_stalk_state.stable_counter = 0;
	
	wiper_stalk_state.previous_value = ReadWiperStalk_Hardware();
	wiper_stalk_state.current_value = wiper_stalk_state.previous_value;
	wiper_stalk_state.stable_counter = 0;
	
	speed_signal_state.previous_value = ReadSpeedSignal_Hardware();
	speed_signal_state.current_value = speed_signal_state.previous_value;
	speed_signal_state.stable_counter = 0;
	
}


void InputManager_Update(void)
{
	//Check for any change for all inputs
 
	ProcessIgnitionInput();
	ProcessDoorInput();
	ProcessLockButtonInput();
	ProcessBlinkerInput();
    ProcessWiperInput();
    ProcessSpeedInput();
}


/* ============================================ */
/*      PRIVATE FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

static u8 ReadIgnitionSwitch_Hardware(void)
{
	return sim_ignition_position;
}


static u8 ReadDoorSensor_Hardware(void)
{
	return sim_door_sensor;
}



static u8 ReadLockButton_Hardware(void)
{
	return sim_lock_button;
}


static u8 ReadBlinkerStalk_Hardware(void)
{
	return sim_blinker_stalk;
}


static u8 ReadWiperStalk_Hardware(void)
{
	return sim_wiper_stalk;
}


static u8 ReadSpeedSignal_Hardware(void)
{
	return sim_speed;
}


static u8 DebounceInput(input_state_t* input, u8 raw_value)
{
	if(raw_value == input->current_value)
	{
		if(input->stable_counter < DEBOUNCE_THRESHOLD_MS)
		{
			input->stable_counter++;
		}
	}
	else
	{
		input->current_value = raw_value;
		input->stable_counter = 0;
	}
	
	if(input->stable_counter >= DEBOUNCE_THRESHOLD_MS)
	{
		if(input->current_value != input->previous_value)
		{
			input->previous_value = input->current_value;
			return TRUE;	//if a change happened
		}
	}
    return FALSE;
}


static void ProcessIgnitionInput(void)
{    
	u8 raw_ignition = ReadIgnitionSwitch_Hardware();	
	
	if(DebounceInput(&ignition_state, raw_ignition))
	{
		switch(ignition_state.previous_value)
		{
			case 0:	//OFF
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, IGNITION_OFF_EVENT);
			break;
			
			case 1:	//ACC
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, IGNITION_ACC_EVENT);
			break;
			
			case 2:	//ON
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, IGNITION_ON_EVENT);
			break;
			
			case 3:	//START
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, IGNITION_START_EVENT);
			break;
		}
	}
}


static void ProcessDoorInput(void)
{
    u8 raw_door = ReadDoorSensor_Hardware();
	
	if(DebounceInput(&door_state,raw_door))
	{
		switch(door_state.previous_value)
		{
			case 0:		//DOORS CLOSED
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, DOOR_CLOSE_EVENT);
			break;
			
			case 1:		//DOORS OPEN
			EVENTQUEUE_u8enQueue(&GlobalEventQueue, DOOR_OPEN_EVENT);
			break;
		}
	}
}


static void ProcessLockButtonInput(void)
{
   	u8 raw_lock = ReadLockButton_Hardware();
	
	if(DebounceInput(&lock_button_state,raw_lock))
	{
		 if(lock_button_state.previous_value == 0)  // LOCK
		 {
			 EVENTQUEUE_u8enQueue(&GlobalEventQueue, LOCK_REQUEST_EVENT);
		 }
		 else if(lock_button_state.previous_value == 1)  // UNLOCK
		 {
			 EVENTQUEUE_u8enQueue(&GlobalEventQueue, UNLOCK_REQUEST_EVENT);
		 }
	}
}


static void ProcessBlinkerInput(void)
{
    u8 raw_blinker = ReadBlinkerStalk_Hardware();
	
	if(DebounceInput(&blinker_stalk_state,raw_blinker))
	{
		switch(blinker_stalk_state.previous_value)
		{
			case 0:		//NEUTRAL
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,BLINKER_OFF_REQUEST);
			break;
			
			case 1:		//LEFT
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,LEFT_BLINKER_REQUEST);
			break;
			
			case 2:		//RIGHT
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,RIGHT_BLINKER_REQUEST);
			break;
			
			case 3:		//HAZARD
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,HAZARD_REQUEST);
			break;
		}
	}
}


static void ProcessWiperInput(void)
{
    u8 raw_wiper = ReadWiperStalk_Hardware();
	
	if(DebounceInput(&wiper_stalk_state,raw_wiper))
	{
		switch(wiper_stalk_state.previous_value)
		{
			case 0:		//OFF
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,WIPER_OFF_REQUEST);
			break;
			
			case 1:		//INT
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,WIPER_INT_REQUEST);
			break;
			
			case 2:		//LOW
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,WIPER_LOW_REQUEST);
			break;
			
			case 3:		//HIGH
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,WIPER_HIGH_REQUEST);
			break;
		}
	}
}


static void ProcessSpeedInput(void)
{
    u8 raw_speed = ReadSpeedSignal_Hardware();
	
	if(DebounceInput(&speed_signal_state,raw_speed))
	{
		switch(speed_signal_state.previous_value)
		{
			case 0:		//STOPPED
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,VEHICLE_STOPPED_EVENT);
			break;
			
			case 1:
			EVENTQUEUE_u8enQueue(&GlobalEventQueue,VEHICLE_MOVING_EVENT);
			break;
		}
	}
}