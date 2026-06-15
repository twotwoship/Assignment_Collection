/*
 * interrupt.h
 *
 * Created: 2026-06-15 오후 4:20:57
 *  Author: kccistc
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#define F_CPU 16000000UL
#include <avr/interrupt.h> // sei cli ....etc interrupt function

void init_timer2(void);




#endif /* INTERRUPT_H_ */