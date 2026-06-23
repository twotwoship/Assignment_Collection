/*
 * timer1.h
 *
 * Created: 2026-06-19 오후 9:19:54
 *  Author: kccistc
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_
#include "motor.h"
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <avr/interrupt.h>



void init_timer1(void); // PWM MOTOR

#endif /* TIMER1_H_ */