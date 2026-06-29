/*
 * ds1302.h
 *
 * Created: 2026-06-26 오후 2:42:02
 *  Author: kccistc
 */ 


#ifndef DS1302_H_
#define DS1302_H_

#define F_CPU 16000000UL  //16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>
#include <string.h>

#define DS1302_CLK_DDR DDRF
#define DS1302_CLK_PORT PORTF

#define DS1302_DAT_DDR DDRF
#define DS1302_DAT_PORT PORTF	// 쓸때 쓰는는거임 내보낼떄
#define DS1302_DAT_PIN PINF		//read 읽을때 쓰는거임. 들여올떄

#define DS1302_RST_DDR DDRF
#define DS1302_RST_PORT PORTF

#define DS1302_CLK	0	// 0번 pin
#define DS1302_DAT	1	// 1번 pin
#define DS1302_RST	2	// 2번 pin

#define ADDR_SECONDS 0x80		//write 만 정의하고 read는 +1 하는걸로.
#define ADDR_MINUTES 0x82
#define ADDR_HOUR 0x84
#define ADDR_DATE 0x86
#define ADDR_MONTH 0x88
#define ADDR_DAYOFWEEK 0x8A
#define ADDR_YEAR 0x8C
#define ADDR_WRITEPROTECTED 0x8E
#define ADDR_TCS 0x90

typedef struct _ds1302{
	uint8_t second;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t dayofweek;
	uint8_t year;
	uint8_t wp;
	uint8_t ampm;	//1 : pm, 0 : am
	uint8_t hourmode;	//0 : 24 1 : 12
} t_ds1302;



void init_date_time(t_ds1302 *ds1302);
void init_gpio_ds1302(void);
void init_ddr_ds1302(void);
void init_ds1302(t_ds1302 *ds1302);

void write_ds1302(uint8_t addr, uint8_t data);
void tx_ds1302(uint8_t data);
void clock_ds1302(void);
uint8_t dec2bcd(uint8_t data);
void ds1302_main(void);

void read_time_ds1302(t_ds1302 *ds1302);
void read_date_ds1302(t_ds1302 *ds1302);

void _bust_read_ds1302(uint8_t addr, uint64_t *temp);
void _bust_rx_ds1302(uint64_t *temp);
void rx_ds1302(uint8_t *pdata8bits);
uint8_t bcd2dec(uint8_t data);
uint8_t read_ds1302(uint8_t addr);

void ds1302_main_test(void);

#endif /* DS1302_H_ */