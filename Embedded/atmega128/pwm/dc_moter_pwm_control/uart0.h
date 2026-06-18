/*
 * uart0.h
 *
 * Created: 2026-06-16 오전 9:57:41
 *  Author: kccistc
 */ 


#ifndef UART0_H_
#define UART0_H_
#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>
#include <string.h>

#define QUEUE_SIZE 10
#define QUEUE_LENGTH 80
volatile unsigned char rx_buff[QUEUE_SIZE][QUEUE_LENGTH];
void pc_command_processing(void);


#endif /* UART0_H_ */