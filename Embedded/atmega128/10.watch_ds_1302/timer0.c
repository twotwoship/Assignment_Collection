/*
 * timer0.c
 *
 * Created: 2026-06-19 오후 9:03:30
 *  Author: kccistc
 */ 

#include "timer0.h"

void init_timer0(void){
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0; // TIMER0 OVERFLOW INT 활성화
	
}
