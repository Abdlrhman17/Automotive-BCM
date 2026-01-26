#ifndef FAULTS_H_
#define FAULTS_H_

#include "StdTypes.h"

typedef enum
{
	FAULT_SEVERITY_NON_CRITICAL = 0,
	FAULT_SEVERITY_CRITICAL,
	FAULT_SEVERITY_SAFETY
}fault_severity_t;

typedef enum
{
	FAULT_VOLATILE = 0,
	FAULT_LATCHED
}fault_persistence_t;

typedef enum 
{
	IGNITION_SIGNAL_FAULT = 0,
	DOOR_SENSOR_FAULT,
	LOCK_ACTUATOR_FAULT,
	BLINKER_OUTPUT_FAULT,
	WIPER_OUTPUT_FAULT,
	SPEED_SIGNAL_FAULT
}fault_id_t;

typedef struct
{
	fault_id_t faultID;
	fault_severity_t faultSeverity;
	fault_persistence_t faultPersistence;
	uint8_t is_Active;
}fault_t;

#endif /* FAULTS_H_ */