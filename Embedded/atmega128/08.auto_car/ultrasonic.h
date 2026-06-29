/*
 * ultrasonic.h
 *
 * Created: 2026-06-20 오후 3:08:26
 *  Author: kccistc
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <avr/interrupt.h> // sei cli ....etc interrupt function

#define TRIG_LAFT_DDR	DDRA	//output
#define TRIG_LAFT_PORT	PORTA
#define TRIG_LAFT_PIN	0

#define TRIG_MID_DDR	DDRA		//output
#define TRIG_MID_PORT	PORTA
#define TRIG_MID_PIN	1

#define TRIG_RIGHT_DDR	DDRA		//output
#define TRIG_RIGHT_PORT	PORTA
#define TRIG_RIGHT_PIN	2

#define ECHO_LAFT_DDR	DDRE		//input
#define ECHO_LAFT_PORT	PINE		//external INT 4
#define ECHO_LAFT_PIN	4

#define ECHO_MID_DDR	DDRE		//input
#define ECHO_MID_PORT	PINE		//external INT 5
#define ECHO_MID_PIN	5

#define ECHO_RIGHT_DDR	DDRE		//input
#define ECHO_RIGHT_PORT	PINE		//external INT 6
#define ECHO_RIGHT_PIN	6

#define MS_COUNT_RESET 1000
#define _US_COUNT_RESET 50000
#define _TCNT1_RESET 1023

#define LAFT_TYPE 1
#define MID_TYPE 2
#define RIGHT_TYPE 3

#define ULTRA_INTERVAL 30   // 50ms마다 센서 하나 측정


#define TRIG_HIGH_TICK 1

void init_ultrasonic(void);

/*
void make_left_trigger(void);
void make_mid_trigger(void);
void make_right_trigger(void);
void ultrasonic_left_process(void);
void ultrasonic_mid_process(void);
void ultrasonic_right_process(void);
void make_trigger_high(int type);
void make_trigger_low(int type);
*/
//void test_ultrasonic_processing(void);
void litrasonic_processing_flag(void);
void make_trigger(int type);

void _ultrasonic_processing_flag_laft(void);
void _ultrasonic_processing_flag_mid(void);
void _ultrasonic_processing_flag_right(void);

extern volatile int ultrasonic_left_distance;
extern volatile int ultrasonic_mid_distance;
extern volatile int ultrasonic_right_distance;

#endif /* ULTRASONIC_H_ */