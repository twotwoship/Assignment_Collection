/*
 * 09.DHT11.c
 *
 * Created: 2026-06-26 오전 9:25:15
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <stdio.h>

#include "uart0.h"
#include "DHT11.h"


extern void UARTO_transmit(unsigned char data);

//파일을 맵핑해줘야됨. p232
FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	init_uart();
	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.
	
	sei();	// 전역(대문) interrrupt 허용

	//init_date_time();
	ds1302_main();
	
	while (1)
	{
		dht11_main();
		_delay_ms(1500);		// 안정화 시간 필요함 1.5sec
	}
}