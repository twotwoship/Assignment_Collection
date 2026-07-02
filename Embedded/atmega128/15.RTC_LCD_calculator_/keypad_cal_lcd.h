/*
 * keypad_cal_lcd.h
 *
 * Created: 2026-07-02 오전 10:39:54
 *  Author: kccistc
 */ 


#ifndef KEYPAD_CAL_LCD_H_
#define KEYPAD_CAL_LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>
#include <stdlib.h>

#include "timer0.h"
#include "ds1307.h"
#include "uart0.h"
#include "text_lcd.h"
#include "button.h"
#include "menu.h"
#include "cal.h"
#include "keypad.h"

#define MAX 100

extern char *cal_inbuff;
extern volatile int run_calculation;
extern volatile int cal_buff_count;
extern volatile uint8_t _first;
extern volatile uint8_t keypad_scan_flag;
extern volatile int reset_count;

void calculator_text_lcd_Synchronization(t_ds1307 *ds1307);

#endif /* KEYPAD_CAL_LCD_H_ */