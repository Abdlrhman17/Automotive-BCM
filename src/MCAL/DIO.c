#include "atmega32_mem_map.h"
#include "Utils.h"
#include "StdTypes.h"
#include "DIO_interface.h"
#include "DIO_private.h"


void DIO_Init(void)
{
	for(DIO_PinNum_type i = PINA0; i < TotalPins; ++i)
	{
		DIO_InitPin(i, PinStatusArray[i]);
	}
}


void DIO_InitPin(DIO_PinNum_type pin, DIO_PinStatus_type status)
{
	DIO_Port_type port = pin / 8;
	u8 pin_num = pin % 8;
	
	switch(status)
	{
		case OUTPUT:
		switch(port)
		{
			case PA:
			SET_BIT(DDRA,pin_num);
			CLR_BIT(PORTA,pin_num);
			break;
			
			case PB:
			SET_BIT(DDRB,pin_num);
			CLR_BIT(PORTB,pin_num);
			break;
			
			case PC:
			SET_BIT(DDRC,pin_num);
			CLR_BIT(PORTC,pin_num);
			break;
			
			case PD:
			SET_BIT(DDRD,pin_num);
			CLR_BIT(PORTD,pin_num);
			break;
		}
		break;
		
		case INFREE:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA,pin_num);
			CLR_BIT(PORTA,pin_num);
			break;
			
			case PB:
			CLR_BIT(DDRB,pin_num);
			CLR_BIT(PORTB,pin_num);
			break;
			
			case PC:
			CLR_BIT(DDRC,pin_num);
			CLR_BIT(PORTC,pin_num);
			break;
			
			case PD:
			CLR_BIT(DDRD,pin_num);
			CLR_BIT(PORTD,pin_num);
			break;

		}
		break;
		
		case INPULL:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA,pin_num);
			SET_BIT(PORTA,pin_num);
			break;
			
			case PB:
			CLR_BIT(DDRB,pin_num);
			SET_BIT(PORTB,pin_num);
			break;
			
			case PC:
			CLR_BIT(DDRC,pin_num);
			SET_BIT(PORTC,pin_num);
			break;
			
			case PD:
			CLR_BIT(DDRD,pin_num);
			SET_BIT(PORTD,pin_num);
			break;
		}
		break;
	}
}



void DIO_WritePin(DIO_PinNum_type pin, DIO_PinVoltag_type volt)
{
	DIO_Port_type port = pin / 8;
	u8 pin_num = pin % 8;
	
	if(volt == HIGH)
	{
		switch(port)
		{
			case PA:
			SET_BIT(PORTA,pin_num);
			break;
			
			case PB:
			SET_BIT(PORTB,pin_num);
			break;
			
			case PC:
			SET_BIT(PORTC,pin_num);
			break;
			
			case PD:
			SET_BIT(PORTD,pin_num);
			break;
		}
	}
	else if(volt == LOW)
	{
		switch(port)
		{
			case PA:
			CLR_BIT(PORTA,pin_num);
			break;
			
			case PB:
			CLR_BIT(PORTB,pin_num);
			break;
			
			case PC:
			CLR_BIT(PORTC,pin_num);
			break;
			
			case PD:
			CLR_BIT(PORTD,pin_num);
			break;
		}
	}
}



DIO_PinVoltag_type DIO_ReadPin(DIO_PinNum_type pin)
{
	DIO_Port_type port = pin / 8;
	u8 pin_num = pin % 8;
	DIO_PinVoltag_type volt = LOW;
	
	switch(port)
	{
		case PA:
		volt = GET_BIT(PORTA,pin_num);
		break;
		
		case PB:
		volt = GET_BIT(PINB,pin_num);
		break;
		
		case PC:
		volt = GET_BIT(PINC,pin_num);
		break;
		
		case PD:
		volt = GET_BIT(PIND,pin_num);
		break;
	}
	return volt;
}



void DIO_TogglePin(DIO_PinNum_type pin)
{
	DIO_Port_type port = pin / 8;
	u8 pin_num = pin % 8;
	switch(port)
	{
		case PA:
		TOG_BIT(PORTA,pin_num);
		break;
			
		case PB:
		TOG_BIT(PORTB,pin_num);
		break;
		
		case PC:
		TOG_BIT(PORTC,pin_num);
		break;
			
		case PD:
		TOG_BIT(PORTD,pin_num);
		break;
	}
}



void DIO_WritePort(DIO_Port_type port, u8 value)
{
	switch(port)
	{
		case PA:
		PORTA = value;
		break;
		
		case PB:
		PORTB = value;
		break;
		
		case PC:
		PORTC = value;
		break;
		
		case PD:
		PORTD = value;
		break;
	}
}


u8 DIO_ReadPort(DIO_Port_type port)
{
	u8 value = 0;
	switch(port)
	{
		case PA:
		value = PORTA;
		break;
		
		case PB:
		value = PORTB;
		break;
		
		case PC:
		value = PORTC;
		break;
		
		case PD:
		value = PORTD;
		break;
	}
	return value;
}

void DIO_WritePin_Port(DIO_Port_type port, DIO_PinNum_type pin, DIO_PinVoltag_type volt)
{
	switch (volt)
	{
		case HIGH:
		switch (port)
		{
			case PA:
			SET_BIT(PORTA,(pin % 8));
			break;
				
			case PB:
			SET_BIT(PORTB,(pin % 8));
			break;
				
			case PC:
			SET_BIT(PORTC,(pin % 8));
			break;
				
			case PD:
			SET_BIT(PORTD,(pin % 8));
			break;
		}
		break;
		
		case LOW:
		switch (port)
		{
			case PA:
			CLR_BIT(PORTA,(pin % 8));
			break;
					
			case PB:
			CLR_BIT(PORTB,(pin % 8));
			break;
						
			case PC:
			CLR_BIT(PORTC,(pin % 8));
			break;
				
			case PD:
			CLR_BIT(PORTD,(pin % 8));
			break;
			}
		break;
	}
}



/*
                                     OPTIMIZED VERSION

void DIO_WritePin_Port(DIO_Port_type port, DIO_PinNum_type pin, DIO_PinVoltag_type volt)
{
	volatile uint8_t* ports[] = { &PORTA, &PORTB, &PORTC, &PORTD };

	if (volt == HIGH)
	SET_BIT(*ports[port], pin % 8);
	else
	CLR_BIT(*ports[port], pin % 8);
}


*/