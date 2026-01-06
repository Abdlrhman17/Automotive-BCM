#include "StdTypes.h"
#include "atmega32_mem_map.h"
#include "Utils.h"
#include "UART_interface.h"


static void (*UART_TX_Fptr)(void) = NULL_PTR;
static void (*UART_RX_Fptr)(void) = NULL_PTR;


void UART_Init(void)
{
	/*BAUD RATE*/	
	UBRRL = 103;
	/*FRAME*/
	
	/*ENABLE*/
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN);
}


void UART_SendData(u8 data)
{
	while(!GET_BIT(UCSRA,UDRE));
	
	UDR = data;
}


u8 UART_ReceiveData(void)
{
	while(!GET_BIT(UCSRA,RXC));
	
	return UDR;
}

u8 UART_ReceiveData_Periodic(u8* pdata)
{
	if(GET_BIT(UCSRA,RXC))
	{
		*pdata = UDR;
		return 1;
	}
	else
	{
		return 0;
	}
}


void UART_SendData_NoBlock(u8 data)
{
	UDR = data;
}


u8 UART_ReceiveData_NoBlock(void)
{
	return UDR;
}


void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}


void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}


void UART_TX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}


void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}

void UART_TX_SetCallBack(void (*LocalFptr)(void))
{
	UART_TX_Fptr = LocalFptr;
}
void UART_RX_SetCallBack(void (*LocalFptr)(void))
{
	UART_RX_Fptr = LocalFptr;
}


ISR(USART_TXC_vect)
{
	if(UART_TX_Fptr != NULL_PTR)
	{
		UART_TX_Fptr();
	}
}

ISR(USART_RXC_vect)
{
	if(UART_RX_Fptr != NULL_PTR)
	{
		UART_RX_Fptr();
	}
}
