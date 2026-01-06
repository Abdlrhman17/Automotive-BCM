
#include "EX_INT.h"
#include "DIO_interface.h"

/****************************Pointer to functions to be assigned to ISR*********************************/

static void (*INT0_Fptr) (void) = NULL_PTR;
static void (*INT1_Fptr) (void) = NULL_PTR;
static void (*INT2_Fptr) (void) = NULL_PTR;

/********************************External Inturrupt Enable & Disable************************************/

void EXI_Enable(ExInterruptSource_type interrupt)
{
	switch(interrupt)
	{
		case EX_INT0 :
		SET_BIT(GICR,INT0);
		break;
		
		case EX_INT1 :
		SET_BIT(GICR,INT1);
		break;
		
		case EX_INT2 :
		SET_BIT(GICR,INT2);
		break;
	}
}



void EXI_Disable(ExInterruptSource_type interrupt)
{
	switch(interrupt)
	{
		case EX_INT0 :
		CLR_BIT(GICR,INT0);
		break;
		
		case EX_INT1 :
		CLR_BIT(GICR,INT1);
		break;
		
		case EX_INT2 :
		CLR_BIT(GICR,INT2);
		break;
	}
}



/************************************Interrupt Control function*****************************/

void EXI_TriggerEdge(ExInterruptSource_type interrupt, TriggerEdge_type edge)
{
	if (interrupt == EX_INT0)
	{
		switch(edge)
		{
			case LOW_LEVEL:         CLR_BIT(MCUCR, ISC00); CLR_BIT(MCUCR, ISC01); break;
			case ANY_LOGIC_CHANGE:  SET_BIT(MCUCR, ISC00); CLR_BIT(MCUCR, ISC01); break;
			case FALLING_EDGE:      CLR_BIT(MCUCR, ISC00); SET_BIT(MCUCR, ISC01); break;
			case RISING_EDGE:       SET_BIT(MCUCR, ISC00); SET_BIT(MCUCR, ISC01); break;
		}
	}
	else if (interrupt == EX_INT1)
	{
		switch(edge)
		{
			case LOW_LEVEL:         CLR_BIT(MCUCR, ISC10); CLR_BIT(MCUCR, ISC11); break;
			case ANY_LOGIC_CHANGE:  SET_BIT(MCUCR, ISC10); CLR_BIT(MCUCR, ISC11); break;
			case FALLING_EDGE:      CLR_BIT(MCUCR, ISC10); SET_BIT(MCUCR, ISC11); break;
			case RISING_EDGE:       SET_BIT(MCUCR, ISC10); SET_BIT(MCUCR, ISC11); break;
		}
	}
	else if (interrupt == EX_INT2)
	{
		if(edge == RISING_EDGE)
		SET_BIT(MCUCSR, ISC2);
		
		else
		CLR_BIT(MCUCSR, ISC2);
	}
}


/************************************Call back functions*********************************************/

void EXI_SetCallBack(ExInterruptSource_type interrupt,void(*localPtr)(void))
{
	switch(interrupt)
	{
		case EX_INT0:
		INT0_Fptr = localPtr;
		break;
		
		case EX_INT1:
		INT1_Fptr = localPtr;
		break;
		
		case EX_INT2:
		INT2_Fptr = localPtr;
		break;
	}
}

/*****************************************ISR************************************************/

ISR(INT0_vect)
{
	if(INT0_Fptr != NULL_PTR)
	{
		INT0_Fptr();
	}
}

ISR(INT1_vect)
{
	if(INT1_Fptr != NULL_PTR)
	{
		INT1_Fptr();
	}
}

ISR(INT2_vect)
{
	if(INT2_Fptr != NULL_PTR)
	{
		INT2_Fptr();
	}
}