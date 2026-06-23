/*
 * led1.h
 *
 * Created: 2026-06-19 오후 8:20:31
 *  Author: kccistc
 */ 


#ifndef LED1_H_
#define LED1_H_
#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>

#define BLUETOOTH_LED_MODE DDRG
#define BLUETOOTH_LED_CONTROL PORTG

#define BLUETOOTH_LED_1 3

#define _On 1
#define _Off 0

void init_led(void);
void led_on(void);
void _led_off(void);

#endif /* LED1_H_ */