/*
 * led.h
 *
 * Created: 2026-06-10 오후 3:09:57
 *  Author: kccistc
 */ 


#ifndef LED_H_
#define LED_H_
#define F_CPU 16000000UL	// 16MHz Because int type 2Byte in atmega128, long type
#include <avr/io.h>			// PORTA PORTD for I/O register
#include <util/delay.h>

void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_right_on(void); // half_right
void led_left_on(void); // half_left




#endif /* LED_H_ */