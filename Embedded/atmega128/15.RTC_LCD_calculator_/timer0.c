/*
 * timer0.c
 *
 * Created: 2026-06-19 오후 9:03:30
 *  Author: kccistc
 */ 

#include "timer0.h"
#include "button.h"
#include "keypad_cal_lcd.h"

volatile uint32_t keypad_count = 0;
volatile uint16_t debounce_count = 0;
volatile uint16_t ms_count = 0;
volatile uint16_t cal_reset_ms_count = 0;
volatile uint8_t button0_flag;
volatile uint8_t button1_flag;
volatile uint8_t button2_flag;
volatile uint8_t button3_flag;
volatile uint8_t button4_flag;
volatile uint8_t keydata = 0;
volatile uint8_t keypad_scan_flag = 0;


void init_timer0(void){
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0; // TIMER0 OVERFLOW INT 활성화
	
}

ISR(TIMER0_OVF_vect){		//4ms timer
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	if(cal_reset_ms_count > 250){
		reset_count++;
		cal_reset_ms_count = 0;
	}
		debounce_count++;
		cal_reset_ms_count++;
		ms_count++;
		if(get_button(BUTTON0,BUTTON0PIN)){
			button0_flag=1;
		}
		if(get_button(BUTTON1,BUTTON1PIN)){
			button1_flag=1;
		}
		if(get_button(BUTTON2,BUTTON2PIN)){
			button2_flag=1;
		}
		if(get_button(BUTTON3,BUTTON3PIN)){
			button3_flag=1;
		}
		if(get_button(BUTTON4,BUTTON4PIN)){
			button4_flag=1;
			reset_count = 0;
		}
		
		if(++keypad_count >= 60){
			keypad_count = 0;
			keypad_scan_flag = 1;
		}
}