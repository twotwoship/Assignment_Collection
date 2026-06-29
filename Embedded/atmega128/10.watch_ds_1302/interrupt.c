/*
 * interrupt.c
 *
 * Created: 2026-06-20 오후 2:30:46
 *  Author: kccistc
 */ 
#include "interrupt.h"
#include "ds1302.h"

volatile uint16_t msec_count = 0;
extern volatile uint8_t watch;

ISR(TIMER0_OVF_vect){		//1sec timer
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	msec_count++; // 1ms count
	if(msec_count >= 1000){
		msec_count = 0;
		watch = 1;
	}
}
