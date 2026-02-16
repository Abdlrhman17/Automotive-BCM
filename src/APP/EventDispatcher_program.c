#include "StdTypes.h"
#include "EventDispatcher_interface.h"
#include "EventQueue_interface.h"
#include "ECU_StateMachine_interface.h"
#include "Trace.h"

// GLobal Events Queue
extern Events_Queue_t GlobalEventQueue;


void EventDispatcher_ProcessEvents(void)
{
	 ecu_event_t event;
	 
	 while(EVENTQUEUE_u8deQueue(&GlobalEventQueue, &event) == OK)
	 {
		 switch(event)
		 {
			// ECU Events
			case TECHNICIAN_DIAG_REQUEST:
			case DIAG_TIMEOUT_EVENT:
			case ECU_RESET_EVENT:
			case ECU_EXIT_DIAG_REQUEST:
			case FAULT_CRITICAL_EVENT:
			case FAULT_CLEARED_EVENT:
				ECU_StateMachine_ProcessEvent(event);
				break;
			
			// Ignition Events
			case IGNITION_OFF_EVENT:
			case IGNITION_ACC_EVENT:
			case IGNITION_ON_EVENT:
			case IGNITION_START_EVENT:
				Ignition_StateMachine_ProcessEvent(event);
				break;
			
			// Door Events
			case DOOR_OPEN_EVENT:
			case DOOR_CLOSE_EVENT:
				Door_StateMachine_ProcessEvent(event);
				break;
			
			// Lock Events
			case LOCK_REQUEST_EVENT:
			case UNLOCK_REQUEST_EVENT:
				Lock_StateMachine_ProcessEvent(event);
				break;
			
			// Blinker Events
			case LEFT_BLINKER_REQUEST:
			case RIGHT_BLINKER_REQUEST:
			case HAZARD_REQUEST:
			case BLINKER_OFF_REQUEST:
				Blinker_StateMachine_ProcessEvent(event);
				break;
			
			// Wiper Events
			case WIPER_OFF_REQUEST:
			case WIPER_INT_REQUEST:
			case WIPER_LOW_REQUEST:
			case WIPER_HIGH_REQUEST:
				Wiper_StateMachine_ProcessEvent(event);
				break;
			
			// Vehicle Movement Events
			case VEHICLE_STOPPED_EVENT:
			case VEHICLE_MOVING_EVENT:
				ECU_StateMachine_ProcessEvent(event);
				Lock_StateMachine_ProcessEvent(event);
				break;
			
			default:
				TRACE_ERROR(TRACE_ECU, "Unknown event received");
				break;
		 }
	 }
}