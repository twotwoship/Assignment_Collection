/*
 * led1.c
 *
 * Created: 2026-06-19 오후 8:20:42
 *  Author: kccistc
 */ 
#include "led1.h"

void init_led(void){
	BLUETOOTH_LED_MODE |= _On << BLUETOOTH_LED_1;
}

void _led_off(void){
	BLUETOOTH_LED_CONTROL &= ~(_On << BLUETOOTH_LED_1);
}

void led_on(void){
	BLUETOOTH_LED_CONTROL |= _On << BLUETOOTH_LED_1;
}
