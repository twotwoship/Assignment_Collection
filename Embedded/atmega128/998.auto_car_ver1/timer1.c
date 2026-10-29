/*
 * timer1.c
 *
 * Created: 2026-06-19 오후 9:19:47
 *  Author: kccistc
 */ 
#include "timer1.h"


void init_timer1(void){ // pwm
	TCNT1 = 0;
	TCCR1B |= 1 << CS11 | 1 << CS10;	// 분주비 64
	TCCR1A |= (1 << COM1A1 | 1 << COM1B1);			// 비반전모드
	/*	mode 5 : 8bit fast PWM mode using.
	TCCR1A |= 1 << WGM10;	
	TCCR1B |= 1 << WGM12;
	*/
	// mode 14 : 16bit fast PWM mode using. 348page
	TCCR1A |= 1 << WGM11;	//icr1을 top으로 설정
	TCCR1B |= 1 << WGM12;
	TCCR1B |= 1 << WGM13;
	ICR1 = 0x3ff; // 1023 * 4us = 4ms top 시간 정하기

	//8 bit 고속 모드니까 0 ~255 사이 값.
	//PWM 파형 비율 조절
	OCR1A = 0;	// OCR(output compare regishter) : PWM 값 총 8비트의 표시(256)중 어느정도 전압을 뽑아줄것이냐.
	OCR1B = 0;	// OCR(output compare regishter) : PWM 값 총 8비트의 표시(256)중 어느정도 전압을 뽑아줄것이냐.
	TIMSK |= 1 << TOIE1; // TIMER2 OVERFLOW INT 활성화


}
	// 16000000hz / 64 = 250000hz (250KHz)
	// T = 1/f 1/250000 --> 0.000004sec 4us
