/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:24
 *  Author: kccistc
 */ 
#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_right_on(void); // half_right
void led_left_on(void); // half_left

void init_led(void){
	DDRA = 0xff; // PORTA를 출력모드로 설정
	PORTA = 0x00; // PORTA에 물려있는 led를 all off
	
}

void led_all_on(void){
	//PORTA = 0xff;	// 0 1 off on setting
	*(volatile uint8_t *)0x3B = 0xFF;
}

void led_all_off(void){
	//PORTA = 0xff;	// 0 1 off on setting
	*(volatile uint8_t *)0x3B = 0x00;
}

void led_right_on(void){
		//PORTA = 0x0F;
		*(volatile uint8_t *)0x3B = 0x0F;
}

void led_left_on(void){
	//PORTA = 0xF0;
	*(volatile uint8_t *)0x3B = 0xF0;
}