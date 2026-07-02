/*
 * text_lcd.h
 *
 * Created: 2026-07-01 오전 11:14:55
 *  Author: kccistc
 */ 


#ifndef TEXT_LCD_H_
#define TEXT_LCD_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등
#include "ds1307.h"

#define PORT_DATA				PORTD
#define PORT_CONTROL			PORTC
#define DDR_DATA				DDRD
#define DDR_CONTROL				DDRC	
#define RS_PIN					0
#define RW_PIN					1
#define E_PIN					2
#define COMMAND_CLEAR_DISPLAY	0x01
#define COMMAND_DISPLAY_ON_OFF_BIT	2
#define COMMAND_CURSOR_ON_OFF_BIT	1
#define COMMAND_BLINK_ON_OFF_BIT	0
#define COMMAND_8_BIT_MODE_FIRST	0x30
#define COMMAND_4_BIT_MODE_FIRST	0x20
#define COMMAND_4_BIT_MODE			0x28
#define LCD_SETCGRAMADDR			0x40


void lcd_pulse_enable(void);
void lcd_write_data(uint8_t data);
void lcd_write_command(uint8_t command);
void lcd_clear(void);
void lcd_write_first_command(uint8_t command);
void lcd_write_string(char *string);
void lcd_goto_xy(uint8_t row, uint8_t col);
void lcd_init(void);
void lcd_create_char(uint8_t index, uint8_t pattern[8]);
void create_char_one(void);
void lcd_create_clock_chars(void);
void ds1307_text_lcd_Synchronization(t_ds1307 *ds1307);
void ds1307_watch_setting(t_ds1307 *ds1307);

void lcd_write_if_changed(uint8_t row, uint8_t col, uint8_t data, uint8_t *prev);

void lcd_write_cal_window(char *buff, uint8_t count);
extern volatile uint16_t ms_count;


#endif /* TEXT_LCD_H_ */