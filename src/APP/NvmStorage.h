#ifndef NVMSTORAGE_H_
#define NVMSTORAGE_H_

// Define for latched faults number
#define _IS_LATCED_FAULTS_ARR_INC	1


#include "StdTypes.h"
#include "Faults.h"
#include "Fault_Monitor_interface.h"
#include "VehicleManager.h"

/*
Data saved to NVM when :
	Ignition off
	Reset
	Fault logged
*/
	

typedef struct
{
	fault_id_t degraded_fault_id;
	u8 occurance_counter;
	u8 is_active;
	door_lock_state_t last_lock_state;
	ecu_operational_state_t last_op_state;
}nvm_data_t;

//NVM Structure
typedef struct
{
	u16 magic_number;									// Validity marker
	nvm_data_t faultsArray[LATCHED_FAULTS_COUNT];      // Nvm Data layout (Faults Array)
	u16 checksum;									  // Simple checksum
}nvm_storage_t;

#endif /* NVMSTORAGE_H_ */