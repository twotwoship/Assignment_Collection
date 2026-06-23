/*
 * fnd.h
 *
 * Created: 2026-06-19 오후 7:47:13
 *  Author: kccistc
 */ 


#ifndef FND_H_
#define FND_H_
#define F_CPU 16000000UL
#include <avr/io.h>

#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

#define FND_UP_DIGIT_DDR DDRD
#define FND_UP_DIGIT_PORT PORTD
#define FND_UP_DIGIT_D1 4
#define FND_UP_DIGIT_D2 5
#define FND_UP_DIGIT_D3 6
#define FND_UP_DIGIT_D4 7

#define FND_DOWN_DIGIT_DDR DDRB
#define FND_DOWN_DIGIT_PORT PORTB
#define FND_DOWN_DIGIT_D1 0
#define FND_DOWN_DIGIT_D2 1
#define FND_DOWN_DIGIT_D3 2
#define FND_DOWN_DIGIT_D4 3

void init_fnd(void);
void fnd_test(void);
void fnd_auto_test(void);
void fnd_auto_test_2(void);
void fnd_count(void);
void fnd_running(void);
void fnd_go(void);
void fnd_auto_check_display(void);
extern volatile int ultrasonic_left_distance;
extern volatile int ultrasonic_mid_distance;
extern volatile int ultrasonic_right_distance;

#endif /* FND_H_ */