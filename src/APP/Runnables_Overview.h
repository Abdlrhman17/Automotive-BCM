#ifndef RUNNABLES_OVERVIEW_H_
#define RUNNABLES_OVERVIEW_H_

/*
===============================================================================
ECU RUNNABLES & EXECUTION MODEL OVERVIEW
===============================================================================

This file documents the logical runnables executed by the bare-metal scheduler.
It is NOT implementation code. It defines responsibilities, ownership, and
execution intent only.

The scheduler is time-triggered and does not contain application logic.
Each runnable is responsible for a single concern.

-------------------------------------------------------------------------------
GENERAL EXECUTION RULES
-------------------------------------------------------------------------------
- Runnables must be deterministic and non-blocking
- No runnable may directly modify another module’s state
- State Machines own states and are the only entities allowed to change them
- Inputs generate events, they do not apply logic
- Outputs reflect state, they do not decide behavior
- Faults explain abnormal behavior, they do not fix it

-------------------------------------------------------------------------------
RUNNABLE GROUP 1: INPUT SAMPLING
-------------------------------------------------------------------------------

Runnable: Ignition_Input_Task
- Purpose:
  Read ignition switch hardware state and detect changes.
- Trigger:
  Periodic (e.g. 10 ms)
- Reads:
  Ignition GPIO
- Produces:
  Ignition-related events (OFF / ACC / ON / START)
- Must NOT:
  - Change ignition state
  - Check system rules
  - Drive outputs

Runnable: Door_Input_Task
- Purpose:
  Detect door open/close changes.
- Trigger:
  Periodic
- Reads:
  Door sensor GPIOs
- Produces:
  Door open / close events
- Must NOT:
  - Lock/unlock doors
  - Apply vehicle rules

Runnable: Vehicle_Movement_Input_Task
- Purpose:
  Detect vehicle movement state.
- Trigger:
  Periodic
- Reads:
  Speed signal or movement indicator
- Produces:
  Vehicle moving / stopped events

-------------------------------------------------------------------------------
RUNNABLE GROUP 2: STATE MACHINES
-------------------------------------------------------------------------------

Runnable: ECU_StateMachine_Task
- Purpose:
  Control ECU lifecycle (OFF, STARTUP, ACTIVE)
- Trigger:
  Event-driven / periodic
- Consumes:
  ECU power events, fault events, reset events
- Owns:
  ecu_state_t
  ecu_operational_state_t

Runnable: Ignition_StateMachine_Task
- Purpose:
  Control ignition sequencing (OFF ? ACC ? ON ? START)
- Trigger:
  Event-driven
- Consumes:
  Ignition events, vehicle movement state, fault state
- Owns:
  ignition_state_t
- Enforces:
  No jumps, no illegal transitions

Runnable: Door_StateMachine_Task
- Purpose:
  Track door open/close state
- Trigger:
  Event-driven
- Owns:
  door_state_t

Runnable: Lock_StateMachine_Task
- Purpose:
  Control lock/unlock behavior
- Trigger:
  Event-driven
- Consumes:
  Door state, vehicle movement, ECU state
- Owns:
  Lock state

Runnable: Blinker_StateMachine_Task
- Purpose:
  Control blinker modes (left/right/hazard)
- Trigger:
  Event-driven
- Consumes:
  Blinker requests, ignition state
- Owns:
  blinker_state_t

Runnable: Wiper_StateMachine_Task
- Purpose:
  Control wiper modes
- Trigger:
  Event-driven
- Consumes:
  Wiper requests, ignition state
- Owns:
  wiper_control_state_t

-------------------------------------------------------------------------------
RUNNABLE GROUP 3: OUTPUT CONTROL
-------------------------------------------------------------------------------

Runnable: Lighting_Output_Task
- Purpose:
  Drive physical blinker outputs based on blinker state
- Trigger:
  Periodic
- Reads:
  Blinker state
- Must NOT:
  - Validate ignition
  - Change blinker state

Runnable: Wiper_Output_Task
- Purpose:
  Drive wiper motor outputs
- Trigger:
  Periodic
- Reads:
  Wiper control state

Runnable: Lock_Output_Task
- Purpose:
  Drive door lock actuators
- Trigger:
  Periodic
- Reads:
  Lock state

-------------------------------------------------------------------------------
RUNNABLE GROUP 4: FAULT MONITORING
-------------------------------------------------------------------------------

Runnable: Fault_Monitor_Task
- Purpose:
  Detect and manage faults
- Trigger:
  Periodic
- Reads:
  Inputs, states, output feedback
- Produces:
  Fault events
- Owns:
  Fault activation / clearance logic

-------------------------------------------------------------------------------
RUNNABLE GROUP 5: DIAGNOSTIC & NVM
-------------------------------------------------------------------------------

Runnable: Diagnostic_Task
- Purpose:
  Handle technician diagnostic requests
- Trigger:
  Event-driven
- Consumes:
  Diagnostic events, timeout events

Runnable: NVM_Manager_Task
- Purpose:
  Persist critical system data
- Trigger:
  On ignition OFF, reset, fault logging
- Saves:
  Last lock state
  Last operational state
  Degraded reason

===============================================================================
END OF DOCUMENT
===============================================================================
*/



#endif /* RUNNABLES_OVERVIEW_H_ */