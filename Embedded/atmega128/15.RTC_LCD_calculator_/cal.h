/*
 * cal.h
 *
 * Created: 2026-06-29 오후 4:14:38
 *  Author: kccistc
 */ 


#ifndef CAL_H_
#define CAL_H_

#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>
#include <string.h>
#include "uart0.h"
#include "keypad.h"
//#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void back_display_make(char* inbuff, char* temp_buff, char* operater_stack);
void back_display_carculate(char* temp_buff, int* number_stack);
char pop_c(char* operater_stack);
char push_c(char* operater_stack, char* inbuff);
char peek_c(char* operater_stack);
int pop_i(int* number_stack);
int push_i(int* number_stack, int buff);
int peek_i(int* number_stack);

extern volatile uint8_t calc_error;
extern volatile int top;
extern volatile int i_top;
extern volatile int ocb; // open_culry_bracket
extern volatile int ccb; // close_culry_bracket

#endif /* CAL_H_ */