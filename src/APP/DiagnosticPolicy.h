#ifndef DIAGNOSTICPOLICY_H_
#define DIAGNOSTICPOLICY_H_

/*
Diagnostic Mode Policy
----------------------

Diagnostic mode is entered via TECHNICIAN_DIAG_REQUEST event.

Entry Conditions:
- Vehicle must be stopped
- ECU must be ACTIVE
- No safety-critical fault blocking diagnostics

Exit Conditions:
- Diagnostic timeout
- ECU reset
*/

/* ============================= */
/* Allowed Overrides in DIAG     */
/* ============================= */

/* Door locks may be controlled for testing */
#define DIAG_ALLOW_DOOR_LOCK_CONTROL        1

/* Blinkers may be activated for lamp testing */
#define DIAG_ALLOW_BLINKER_CONTROL          1

/* Wipers are NOT allowed without ignition ON */
#define DIAG_ALLOW_WIPER_CONTROL            0

/* Ignition state transitions cannot be forced */
#define DIAG_ALLOW_IGNITION_OVERRIDE        0


/* ============================= */
/* Forbidden Actions (Hard Rules)*/
/* ============================= */

/*
- Ignition START cannot be triggered from diagnostics
- Wipers cannot run unless ignition is ON
- Vehicle movement cannot be commanded
- Safety-critical faults cannot be overridden
- Diagnostic mode cannot force ECU out of DEGRADED if safety fault exists
*/




#endif /* DIAGNOSTICPOLICY_H_ */