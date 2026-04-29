#ifndef TRACE_H_
#define TRACE_H_

#include "UART_interface.h"

#ifndef PROGMEM
#define PROGMEM __attribute__((__progmem__))
#endif

#ifndef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
#endif


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


/*Trace with the possibility of sending variables */
#define TRACE_INFO_I(cat, msg, val) do {                     \
	uart_send_string_P(PSTR("[INF][" #cat "] " msg));        \
	uart_send_int(val);                                      \
	uart_send_string_P(PSTR("\r\n"));                        \
} while (0)

#define TRACE_ERROR_I(cat, msg, val) do {                    \
	uart_send_string_P(PSTR("[ERR][" #cat "] " msg));        \
	uart_send_int(val);                                      \
	uart_send_string_P(PSTR("\r\n"));                        \
} while (0)

#define TRACE_DEBUG_I(cat, msg, val) do {                    \
	uart_send_string_P(PSTR("[DBG][" #cat "] " msg));        \
	uart_send_int(val);                                      \
	uart_send_string_P(PSTR("\r\n"));                        \
} while (0)

#endif /* TRACE_H_ */