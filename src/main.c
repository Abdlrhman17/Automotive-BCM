/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
// Foundation
#include "Scheduler_interface.h"
#include "EventQueue_interface.h"
#include "TIMERS.h"

// Configuration
#include "config.h"

// Input Layer
#include "InputManager.h"

// State machines
#include "EventDispatcher_interface.h"
#include "ECU_StateMachine_interface.h"
#include "Ignition_StateMachine_interface.h"
#include "Door_StateMachine_interface.h"
#include "Lock_StateMachine_interface.h"
#include "Blinker_StateMachine_interface.h"
#include "Wiper_StateMachine_interface.h"
#include "VehicleMovement_StateMachine_interface.h"

// Output Controllers
#include "Lock_Output_interface.h"
#include "Lighting_Output_interface.h"
#include "Wiper_Output_interface.h"

// Fault & NVM
#include "Fault_Monitor_interface.h"
#include "NVM_Manager_interface.h"

// Drivers
#include "UART_interface.h"
#include "DIO_interface.h"

// Testing
#include "UART_Test_interface.h"

// Trace
#include "Trace.h"


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
// flag to check if the previous scheduler function is done 
volatile u8 tick_flag = 0;


/* ============================================ */
/*          PRIVATE FUNCTION PROTOTYPES         */
/* ============================================ */
static void setTickFlag(void);
static void System_Init(void);
static void Scheduler_Setup(void);
void Combined_StateMachine_Update(void);
void Combined_Output_Update(void);


/* ============================================ */
/*                  MAIN                        */
/* ============================================ */
int main(void)
{
	// UART Initialization for testing
	UART_Init();
	UART_TestInit();
	
	// Initialize all subsystems
	System_Init();
	
	// Configure scheduler with all tasks
	Scheduler_Setup();
	
	TRACE_INFO(TRACE_ECU, "System initialization complete");
	TRACE_INFO(TRACE_ECU, "Starting scheduler...");
	
	
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE, TIMER1_SCALER_64);
	OCR1A = 249;
	Timer1_OCA_SetCallBack(setTickFlag);
	Timer1_OCA_InterruptEnable();
	sei();
	
	
	while(1)
	{
		if(tick_flag)
		{
			tick_flag = 0;
			SCHEDULER_voidStart();
			UART_Task();
			NVM_MainFunction();
		}
	}
	
	return 0;
}

/* ============================================ */
/*      PRIVATE FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

static void System_Init(void)
{
    TRACE_INFO(TRACE_ECU, "System initialization started");
    
	/* ===== PHASE 0: Hardware ===== */
	// configure all pins
	DIO_Init();
	
	
    /* ===== PHASE 1: Foundation ===== */
    
    // Initialize event queue
    EVENTQUEUE_voidInit(EventQueue_Get());
    TRACE_INFO(TRACE_ECU, "Event queue initialized");
    
    
    /* ===== PHASE 2: NVM & Fault Management ===== */
    
    // Initialize NVM (must be first to load saved state)
    NVM_Manager_Init();
    
    // Initialize fault monitor
    Fault_Monitor_Init();
    TRACE_INFO(TRACE_FAULT, "Fault monitor initialized");
    
    
    /* ===== PHASE 3: State Machines ===== */
    
    // Initialize ECU state machine
    ECU_StateMachine_Init();
    
    // Initialize ignition state machine
    Ignition_StateMachine_Init();
    
    // Initialize door state machine
    Door_StateMachine_Init();
    
    // Initialize lock state machine
    Lock_StateMachine_Init();
    
    // Initialize blinker state machine
    Blinker_StateMachine_Init();
    
    // Initialize wiper state machine
    Wiper_StateMachine_Init();
    
    // Initialize vehicle movement state machine
    VehicleMovement_StateMachine_Init();
    
    TRACE_INFO(TRACE_ECU, "All state machines initialized");
    
    
    /* ===== PHASE 4: Input Layer ===== */
    
    // Initialize input manager (Reads hardware)
    InputManager_Init();
    TRACE_INFO(TRACE_ECU, "Input manager initialized");
    
    
    /* ===== PHASE 5: Output Controllers ===== */
    
    // Initialize lock output
    Lock_Output_Init();
    
    // Initialize lighting output
    Lighting_Output_Init();
    
    // Initialize wiper output
    Wiper_Output_Init();
    
    TRACE_INFO(TRACE_ECU, "All output controllers initialized");
}

static void Scheduler_Setup(void)
{
    TRACE_INFO(TRACE_ECU, "Configuring scheduler tasks...");
    
    /* ============================================ */
    /* TASK PRIORITY ASSIGNMENT                     */
    /* ============================================ */
    /*
     * Priority 0 (Highest): Input sampling
     * Priority 1: Event processing
     * Priority 2: State machine updates
     * Priority 3: Output control
     * Priority 4: Fault monitoring
     */
    
    // Priority 0: Input Manager - Sample hardware inputs
    // Runs every 10ms for responsive input detection
    SCHEDULER_taskfeedbackCreateTask(
        0,                          // Priority (highest)
        InputManager_Update,        // Function
        10,                         // Period: 10ms
        0                           // First delay: 0ms (start immediately)
    );
    
    // Priority 1: Event Dispatcher - Process queued events
    // Runs every 20ms to dispatch events to state machines
    SCHEDULER_taskfeedbackCreateTask(
        1,                          // Priority
        EventDispatcher_ProcessEvents,  // Function
        20,                         // Period: 20ms
        5                           // First delay: 5ms (after inputs)
    );
    
    // Priority 2: State Machine Updates - Periodic checks
    // Combined update for all state machines that need periodic execution
    // Runs every 50ms for timeouts, auto-transitions, etc
    SCHEDULER_taskfeedbackCreateTask(
        2,                          // Priority
        Combined_StateMachine_Update,  // Function (see below)
        50,                         // Period: 50ms
        10                          // First delay: 10ms
    );
    
    // Priority 3: Output Controllers - Drive hardware
    // Runs every 10ms for smooth output control
    SCHEDULER_taskfeedbackCreateTask(
        3,                          // Priority
        Combined_Output_Update,     // Function (see below)
        10,                         // Period: 10ms
        15                          // First delay: 15ms
    );
    
    // Priority 4: Fault Monitor - Check for faults
    // Runs every 100ms
    SCHEDULER_taskfeedbackCreateTask(
        4,                          // Priority
        Fault_Monitor_Update,       // Function
        100,                        // Period: 100ms
        20                          // First delay: 20ms
    );
    
    TRACE_INFO(TRACE_ECU, "Scheduler configured with 5 tasks");
}

/* ============================================ */
/*          COMBINED UPDATE FUNCTIONS           */
/* ============================================ */

// Combined state machine update function
void Combined_StateMachine_Update(void)
{
	// Update state machines that need periodic execution
	ECU_StateMachine_Update(SM_UPDATE_PERIOD_MS);
	Ignition_StateMachine_Update(SM_UPDATE_PERIOD_MS);
	Lock_StateMachine_Update(SM_UPDATE_PERIOD_MS);
	Wiper_StateMachine_Update(SM_UPDATE_PERIOD_MS);
	VehicleMovement_StateMachine_Update(SM_UPDATE_PERIOD_MS);
}

// Combined output update function
void Combined_Output_Update(void)
{
	// Update all output controllers
	Lock_Output_Update(OUTPUT_UPDATE_PERIOD_MS);
	Lighting_Output_Update(OUTPUT_UPDATE_PERIOD_MS);
	Wiper_Output_Update(OUTPUT_UPDATE_PERIOD_MS);
}


static void setTickFlag(void)
{
	tick_flag = 1;
}