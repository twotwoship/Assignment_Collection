/*
 * bottom.h
 *
 * Created: 2026-06-10 오전 11:45:12
 *  Author: kccistc
 */ 
#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL	// 16MHz
#include <avr/io.h>			// PORTA PORTB ..... I/O reg
#include <util/delay.h>		// _delay_ms ......


#define BUTTON_DDR DDRD	
#define BUTTON_PIN PIND	// portd 읽는 register 5v:1 0v:0

#define BUTTON0PIN 3 // portd.3
#define BUTTON1PIN 4 // portd.4
#define BUTTON2PIN 5 // portd.5
#define BUTTON3PIN 6 // portd.6

#define BUTTON0	0 // portd.3의 가상 index
#define BUTTON1	1 // portd.4의 가상 index
#define BUTTON2	2 // portd.5의 가상 index
#define BUTTON3	3 // portd.6의 가상 index

#define BUTTON0_NUMBER 4 // BUTTON stock

#define BUTTON_PRESS 1	// press button state high
#define BUTTON_RELEASE 0	// release button state high

#endif