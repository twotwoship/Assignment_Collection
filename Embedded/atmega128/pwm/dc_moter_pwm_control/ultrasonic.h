/*
 * ultrasonic.h
 *
 * Created: 2026-06-17 오후 1:32:41
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.


#define TRIG_DDR	DDRG		//output
#define TRIG_PORT	PORTG
#define TRIG_PIN	4

#define ECHO_DDR	DDRE		//input
#define ECHO_PORT	PINE		//external INT 4
#define ECHO_PIN	4			

void init_ultrasonic(void);
void make_trigger(void);
void ultrasonic_processing(void);

#endif /* ULTRASONIC_H_ */