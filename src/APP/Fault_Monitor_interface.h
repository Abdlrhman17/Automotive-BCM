#ifndef FAULT_MONITOR_INTERFACE_H_
#define FAULT_MONITOR_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Faults.h"


/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

/**
 * Initialize Fault Monitor
 */
void Fault_Monitor_Init(void);

/**
 * Periodic fault monitoring
 * Check all inputs/outputs for faults
 * Called periodically
 */
void Fault_Monitor_Update(void);

/**
 * Get fault status
 * Return fault structure for specific fault ID
 */
fault_t Fault_Monitor_GetFault(fault_id_t fault_id);

/**
 * Check if any SAFETY faults are active
 */
u8 Fault_Monitor_HasSafetyFaults(void);

/**
 * Clear a specific fault (if allowed)
 * Clear latched faults after repair
 */
void Fault_Monitor_ClearFault(fault_id_t fault_id);


#endif /* FAULT_MONITOR_INTERFACE_H_ */