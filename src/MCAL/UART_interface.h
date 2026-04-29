
#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

void UART_Init(void);

void UART_SendData(u8 data);

u8 UART_ReceiveData(void);

u8 UART_ReceiveData_Periodic(u8* pdata);

void UART_SendData_NoBlock(u8 data);

u8 UART_ReceiveData_NoBlock(void);

void UART_SendString(const char *s);

void uart_send_string_P(const char *progmem_s);

void uart_send_int(int val);

void UART_TX_InterruptEnable(void);
void UART_RX_InterruptEnable(void);

void UART_TX_InterruptDisable(void);
void UART_RX_InterruptDisable(void);

void UART_TX_SetCallBack(void (*LocalFptr)(void));
void UART_RX_SetCallBack(void (*LocalFptr)(void));


#endif /* UART_INTERFACE_H_ */