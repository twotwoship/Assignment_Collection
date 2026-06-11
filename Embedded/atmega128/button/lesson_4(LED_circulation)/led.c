/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 
//#define direct_access_port_A *(volatile uint8_t *)0x1B;
#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
int led_main(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keepon(void);
void led_shift_right_keepon(void);
void led_flower_on(void);
void led_flower_off(void);
#define test *(volatile unsigned char *)0x3B

int led_main(void){		// LED 순차적 다루기. led_shift_left_on / led_shift_right_on
	led_all_off();	
	while(1){
		led_shift_left_on();
		//led_shift_left_keepon();
		//led_flower_on();
		//led_all_off();
		_delay_ms(300);
		led_shift_right_on();
		//led_shift_right_keepon();
		//led_flower_off();
	}
	return 0;
}

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

void led_shift_left_on(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 1<< i;
	_delay_ms(30);
	i = (i + 1) % 8;	
#else
	for(int i = 0; i < 8; i++){
		;PORTA = 1 << i;	// 0x1B
		*(unsigned char *) 0x3B = 1<< i;
		_delay_ms(30);
	}
#endif
}

void led_shift_right_on(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 0x80 >> i;
	_delay_ms(30);
	i = (i + 1) % 8;
	
#else
	for(int i = 7; i >= 0; i--){
		//PORTA = 1 << i;	// 0x1B
		*(unsigned char *) 0x3B = 1 << i;
		_delay_ms(30);
	}
#endif
}

void led_shift_left_keepon(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B |= 1 << i;
	_delay_ms(100);
	i = (i + 1) % 8;
	if(i == 0){
		*(unsigned char *) 0x3B = 0x00;
	}
#else
	for(int i = 0; i < 8; i++){
		PORTA += 1 << i;	// 0x1B
		_delay_ms(30);
	}
#endif	
}

void led_shift_right_keepon(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B |= 1 << (7 - i);
	_delay_ms(100);
	i = (i + 1) % 8;
	if(i == 0){
		*(unsigned char *) 0x3B = 0x00;
	}
#else
	for(int i = 7; i > 0; i--){
		PORTA += 1 << i;	// 0x1B
		_delay_ms(300);
	}
#endif
}

void led_flower_on(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 0x18 | (0x18 << i | 0x18 >> i);
	i = (i + 1) % 4;
		_delay_ms(70);

#else
	_delay_ms(1000);
	PORTA = 1 << 3;
	PORTA |= PORTA << 1;
	_delay_ms(1000);
	for(int i = 1; i < 4; i++){
		PORTA |= PORTA << 1;	// 0x1B
		PORTA |= PORTA >> 1;
		_delay_ms(600);
#endif
}

void led_flower_off(void){
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 0x81 | (0x81 >> i | 0x81 << i);
	i = (i + 1) % 4;
		_delay_ms(70);

#else
	led_all_on();
	_delay_ms(600);
	for(int i = 0; i < 3; i++){
		PORTA &= PORTA >> 1;	// 0x1B
		PORTA &= PORTA << 1;
		_delay_ms(600);
	}
	led_all_off();
#endif
}