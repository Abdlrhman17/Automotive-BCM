#ifndef VEHICLEMANAGER_H_
#define VEHICLEMANAGER_H_

typedef enum
{
	ECU_STATE_OFF = 0,
	ECU_STATE_STARTUP,
	ECU_STATE_ACTIVE,
	ECU_STATE_INVALID
}ecu_state_t;

typedef enum
{
	ECU_OP_STATE_NORMAL = 0,
	ECU_OP_STATE_DIAGNOSTIC,
	ECU_OP_STATE_DEGRADED,
	ECU_OP_STATE_INVALID
}ecu_operational_state_t;

typedef enum
{
	IGNITION_OFF = 0,
	IGNITION_ACC,
	IGNITION_ON,
	IGNITION_START
}ignition_state_t;

typedef enum
{
	DOOR_CLOSED = 0,
	DOOR_OPEN
}door_state_t;

typedef enum
{
	WIPERS_OFF = 0,
	WIPERS_INTERMITTENT,
	WIPERS_LOW,
	WIPERS_HIGH
}wiper_control_state_t;

typedef enum
{
	BLINKERS_OFF = 0,
	BLINKERS_HAZARD,
	BLINKERS_RIGHT,
	BLINKERS_LEFT
}blinkers_state_t;

typedef enum
{
	LOCKED = 0,
	UNLOCKED
}door_lock_state_t;

typedef enum
{
	STOPPED = 0,
	MOVING
}vehicle_movement_state_t;


#endif /* VEHICLEMANAGER_H_ */