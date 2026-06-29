/*
 * timer2.c
 *
 * Created: 2026-06-20 오후 2:59:11
 *  Author: kccistc
 */ 
#include "timer2.h"

void init_timer2(void){	// ultrasonic timer
	TCNT2 = 0;
	//분주하지 않고 그대로 쓰기
	TCCR2 &= ~(0 << CS22 | 0 << CS21 | 1 << CS20);
	TCCR2 |= (0 << CS22 | 0 << CS21 | 1 << CS20);
	//분주를 하지 않으면 16Mhz를 나눠서 그대로 쓰면 됨. 
	//tcnt2는 256/16Mhz 라서 1이 0.016ms임
	TIMSK |= 1 << TOIE2; // TIMER2 OVERFLOW INT 활성화

}