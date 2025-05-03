#ifndef __UART_IT_H
#define __UART_IT_H

extern uint8_t uart1_buf[255];
extern uint8_t uart2_buf[255];
extern uint8_t uart3_buf[255];
extern uint8_t uart4_buf[255];

void init_uart_idle_it();

#endif
