#ifndef NVMSTORAGE_H_
#define NVMSTORAGE_H_

#include "StdTypes.h"
#include "Faults.h"
#include "VehicleManager.h"

/*
Data saved to NVM when :
	Ignition off
	Reset
	Fault logged
*/
	

typedef struct
{
	uint8_t last_lock_state;
	fault_id_t degraded_reason;
	ecu_operational_state_t last_op_state;
}nvm_data_t;


#endif /* NVMSTORAGE_H_ */