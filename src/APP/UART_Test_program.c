/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */
#include "StdTypes.h"
#include "UART_Test_interface.h"
#include "UART_interface.h"
#include "InputManager.h"
#include "atmega32_mem_map.h"
#include <string.h>
#include <stdlib.h>


/* ============================================ */
/*          PRIVATE VARIABLES                   */
/* ============================================ */
volatile u8 rx_char;
volatile u8 rx_flag;


/* ============================================ */
/*       PUBLIC FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void UART_TestInit(void)
{
	// Set up interrupt
	UART_RX_SetCallBack(UART_RX_Interrupt_func);
	UART_RX_InterruptEnable();
}


void UART_Task(void)
{
	static char buf[32];
	static u8 idx = 0;

	if (rx_flag)
	{
		rx_flag = 0;

		if (rx_char == '\n' || rx_char == '\r')
		{
			buf[idx] = '\0';
			UART_CommandHandler(buf);
			idx = 0;
		}
		else if (idx < sizeof(buf) - 1)
		{
			buf[idx++] = rx_char;
		}
	}
}


/* ============================================ */
/*      PRIVATE FUNCTION IMPLEMENTATIONS        */
/* ============================================ */

void UART_CommandHandler(char* cmd)
{
	if (!strncmp(cmd, "IGN ", 4))
	INPUTMAN_GetSim()->ignition = atoi(&cmd[4]);

	else if (!strncmp(cmd, "DOOR ", 5))
	INPUTMAN_GetSim()->door = atoi(&cmd[5]);

	else if (!strncmp(cmd, "LOCK ", 5))
	INPUTMAN_GetSim()->lock_btn = atoi(&cmd[5]);

	else if (!strncmp(cmd, "BLINK ", 6))
	INPUTMAN_GetSim()->blinker = atoi(&cmd[6]);

	else if (!strncmp(cmd, "WIP ", 4))
	INPUTMAN_GetSim()->wipers = atoi(&cmd[4]);
	
	else if (!strncmp(cmd, "SPD ", 4))
	INPUTMAN_GetSim()->speed = atoi(&cmd[4]);
	
}

void UART_RX_Interrupt_func(void)
{
	rx_char = UDR;
	rx_flag = 1;
}