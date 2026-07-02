/*
 * 13.text_lcd.c
 *
 * Created: 2026-07-01 오전 11:14:11
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>


#include "timer0.h"
#include "ds1307.h"
#include "uart0.h"
#include "text_lcd.h"
#include "button.h"
#include "menu.h"
#include "cal.h"
#include "keypad.h"


FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);


int main(void)
{		
//============= module _ init ==================================
	init_uart();
	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.

	i2c_init();
	lcd_init();
	init_timer0();
	init_button();
	init_keypad();
	sei();	// 전역(대문) interrrupt 허용	
	
//============= element one =================================	

	t_ds1307 ds1307;
	//init_date_time(&ds1307);	// 구조체에 시간 집어 넣기
	timer_test_2_set(&ds1307);	//시계 최초세팅
	
	lcd_write_string("start_lcd!");
	_delay_ms(300);
	lcd_clear();
			
	//
    while (1) 
    {	
		_RTC_LCD_calculator_control(&ds1307); //임마 안에서 다돌려
	}
	return 0;
}

/*
int main(void)
{		
	init_uart();
	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.

	//init_timer0();
	i2c_init();
	//sei();	// 전역(대문) interrrupt 허용
	
	t_ds1307 ds1307;
	init_date_time(&ds1307);
	
	uint8_t *p = (uint8_t *)&ds1307;

	i2c_start();
	i2c_transmit(SLAVE_ADDR << 1);
	i2c_transmit(0x00);
	
	i2c_transmit(dec2bcd(p[0]) & 0x7F);   // seconds, CH=0

	for (int i = 1; i < 7; i++)
	{
		i2c_transmit(dec2bcd(p[i]));
	}

	i2c_stop();
	

	
    while (1) 
    {	
	uint8_t sec, min, hour, dow, day, mon, year;

	i2c_start();
	i2c_transmit(SLAVE_ADDR << 1);
	i2c_transmit(0x00);

	i2c_start();
	i2c_transmit((SLAVE_ADDR << 1) | 1);

	sec  = i2c_receive_ack();
	min  = i2c_receive_ack();
	hour = i2c_receive_ack();
	dow  = i2c_receive_ack();
	day  = i2c_receive_ack();
	mon  = i2c_receive_ack();
	year = i2c_receive_nack();

	i2c_stop();

	printf("%02d:%02d:%02d  %02d/%02d/%02d  dow=%d\r\n",
	bcd2dec(hour & 0x3F),
	bcd2dec(min),
	bcd2dec(sec & 0x7F),
	bcd2dec(year),
	bcd2dec(mon),
	bcd2dec(day),
	bcd2dec(dow));

	_delay_ms(1000);
	}
	return 0;
}

*/
