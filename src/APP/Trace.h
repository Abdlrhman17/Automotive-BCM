#ifndef TRACE_H_
#define TRACE_H_

typedef enum
{
	TRACE_LEVEL_ERROR = 0,
	TRACE_LEVEL_INFO,
	TRACE_LEVEL_DEBUG
} trace_level_t;


typedef enum
{
	TRACE_ECU,
	TRACE_IGNITION,
	TRACE_DOOR,
	TRACE_LOCK,
	TRACE_BLINKER,
	TRACE_WIPER,
	TRACE_DIAGNOSTIC,
	TRACE_FAULT
} trace_category_t;


#define TRACE_ERROR(tarceCategory, msg)
#define TRACE_INFO(tarceCategory, msg)
#define TRACE_DEBUG(tarceCategory, msg)


#endif /* TRACE_H_ */