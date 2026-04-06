#ifndef FAULT_MONITOR_INTERFACE_H_
#define FAULT_MONITOR_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "Faults.h"

// For the macro not to be included everywhere
#ifdef _IS_LATCED_FAULTS_ARR_INC

// Latched Faults count (For nvm)
#define LATCHED_FAULTS_COUNT	3

#endif

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