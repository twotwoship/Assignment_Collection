/*
 * ds1307.h
 *
 * Created: 2026-07-01 오후 1:22:31
 *  Author: kccistc
 */ 


#ifndef DS1307_H_
#define DS1307_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>


#define I2C_SCL		0
#define I2C_SDA		1
#define SLAVE_ADDR 0x68		//7bit slave 주소

#define DS1307_MEMBER_NUMBER 7
#define DS1307_MEMBER_SECOND 0
#define DS1307_MEMBER_MINUTES 1
#define DS1307_MEMBER_HOURS 2
#define DS1307_MEMBER_DAYOFWEEK 3
#define DS1307_MEMBER_DAY 4
#define DS1307_MEMBER_MONTH 5
#define DS1307_MEMBER_YEAR 6

typedef struct _ds1307{
	uint8_t second;
	uint8_t minutes;
	uint8_t hours;		//	6bit 24 / 12
	uint8_t dayofweek;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t wp;
	uint8_t temp;
} t_ds1307;

void i2c_init(void);
void i2c_start(void);
void i2c_transmit(uint8_t data);
void i2c_stop(void);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
uint8_t bcd2dec(uint8_t data);
uint8_t dec2bcd(uint8_t data);

void timer_test(t_ds1307 *ds1307);

void timer_test_2_set(t_ds1307 *ds1307);
void timer_test_2_read(t_ds1307 *ds1307);

void timer_test_1(t_ds1307 *ds1307);

#endif /* DS1307_H_ */