/*
 * uart.h
 *
 * Created: 2026-06-13 오후 4:28:57
 *  Author: kccistc
 */ 

//uart 0임
#ifndef UART_H_
#define UART_H_
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / 16 / BAUD) - 1)

#include <avr/io.h>

void UART0_Init(void);
void UART0_TxChar(char data);
char UART0_RxChar(void);
void UART0_print_string(char *str);
void UART0_print_1_byte_number(unsigned char n);

#endif /* UART_H_ */


/*


#define U2X1	(*(volatile uint8_t *)0x9B) // bit 1
#define RXEN1	(*(volatile uint8_t *)0x9A) // bit 4
#define TXEN1	(*(volatile uint8_t *)0x9A) // bit 3
void UART1_transmit(char data);
unsigned char UART1_receive(void);


*/