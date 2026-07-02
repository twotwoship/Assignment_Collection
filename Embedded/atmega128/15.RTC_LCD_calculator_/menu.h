/*
 * menu.h
 *
 * Created: 2026-07-01 오후 7:51:03
 *  Author: kccistc
 */ 


#ifndef MENU_H_
#define MENU_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등

void _RTC_LCD_calculator_control(t_ds1307 *ds1307);

#define WATCH_MODE 0
#define WATCH_SETTING_MODE 1
#define CALCULATE_MODE 2

extern volatile int func_state;


#endif /* MENU_H_ */