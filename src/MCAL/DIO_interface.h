#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

#include "StdTypes.h"

typedef enum
{
	OUTPUT,
	INFREE,
	INPULL
}DIO_PinStatus_type;

typedef enum
{
	PA,
	PB,
	PC,
	PD
}DIO_Port_type;


typedef enum
{
	LOW = 0,
	HIGH
}DIO_PinVoltag_type;


typedef enum
{
	DIO_PINA0,
	DIO_PINA1,
	DIO_PINA2,
	DIO_PINA3,
	DIO_PINA4,
	DIO_PINA5,
	DIO_PINA6,
	DIO_PINA7,

	DIO_PINB0,
	DIO_PINB1,
	DIO_PINB2,
	DIO_PINB3,
	DIO_PINB4,
	DIO_PINB5,
	DIO_PINB6,
	DIO_PINB7,

	DIO_PINC0,
	DIO_PINC1,
	DIO_PINC2,
	DIO_PINC3,
	DIO_PINC4,
	DIO_PINC5,
	DIO_PINC6,
	DIO_PINC7,

	DIO_PIND0,
	DIO_PIND1,
	DIO_PIND2,
	DIO_PIND3,
	DIO_PIND4,
	DIO_PIND5,
	DIO_PIND6,
	DIO_PIND7,

	DIO_TotalPins
} DIO_PinNum_type;

void DIO_Init(void);

void DIO_WritePin(DIO_PinNum_type pin, DIO_PinVoltag_type volt);

DIO_PinVoltag_type DIO_ReadPin(DIO_PinNum_type pin);

void DIO_TogglePin(DIO_PinNum_type pin);

void DIO_WritePort(DIO_Port_type port, u8 value);

u8 DIO_ReadPort(DIO_Port_type port);

void DIO_WritePin_Port(DIO_Port_type port, DIO_PinNum_type pin, DIO_PinVoltag_type volt);

#endif /* DIO_INTERFACE_H_ */