/*
 * timer0.h
 *
 * Created: 2026-06-19 오후 9:05:01
 *  Author: kccistc
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t button0_flag;
extern volatile uint8_t button1_flag;
extern volatile uint8_t button2_flag;
extern volatile uint8_t button3_flag;
extern volatile uint8_t button4_flag;

extern volatile uint8_t keydata;
extern volatile uint16_t cal_reset_ms_count;
extern volatile int reset_count;

void init_timer0(void);


#endif /* TIMER0_H_ */