#ifndef UART_TEST_INTERFACE_H_
#define UART_TEST_INTERFACE_H_

/* ============================================ */
/*              INCLUDES                        */
/* ============================================ */

	
/* ============================================ */
/*          PUBLIC FUNCTION PROTOTYPES          */
/* ============================================ */

void UART_TestInit(void);

// Runnable
void UART_Task(void);


/* ============================================ */
/*          PRIVATE FUNCTION PROTOTYPES         */
/* ============================================ */
// Determines what action to take based on command
void UART_CommandHandler(char* cmd);

// RX Interrupt function
void UART_RX_Interrupt_func(void);

#endif /* UART_TEST_INTERFACE_H_ */