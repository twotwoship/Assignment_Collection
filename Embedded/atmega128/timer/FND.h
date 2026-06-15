/*
 * FND.h
 *
 * Created: 2026-06-12 오전 10:46:00
 *  Author: kccistc
 */ 

#ifndef FND_H_
#define FND_H_

#define F_CPU 16000000UL
#include <avr/io.h>

#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

#define FND_DIGIT_DDR DDRB
#define FND_DIGIT_PORT PORTB
#define FND_DIGIT_D1 4
#define FND_DIGIT_D2 5
#define FND_DIGIT_D3 6
#define FND_DIGIT_D4 7

void init_fnd(void);
void fnd_display(uint32_t sec_count, uint8_t dot_display);
void fnd_main(void);
void fnd_second(void);
void fnd_display_second(uint32_t sec_count);
void stop_timer(void);
void fnd_stopwatch_(void);
void fnd_third(uint32_t sec_count, uint32_t ms_count);

#endif /* FND_H_ */