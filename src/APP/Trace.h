#ifndef TRACE_H_
#define TRACE_H_

#include "UART_interface.h"
#include <avr/pgmspace.h>

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
	TRACE_MOVEMENT,
	TRACE_DIAGNOSTIC,
	TRACE_FAULT
} trace_category_t;


#define TRACE_ERROR(cat, msg)  uart_send_string_P(PSTR("[ERR][" #cat "] " msg "\r\n"))
#define TRACE_INFO(cat, msg)   uart_send_string_P(PSTR("[INF][" #cat "] " msg "\r\n"))
#define TRACE_DEBUG(cat, msg)  uart_send_string_P(PSTR("[DBG][" #cat "] " msg "\r\n"))

#endif /* TRACE_H_ */