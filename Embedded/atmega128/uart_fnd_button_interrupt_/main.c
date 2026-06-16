/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:12
 * Author : kccistc
 */ 

#include "uart0.h"
#include "interrupt.h"
#include <avr/io.h>
#include "button.h"
#include "FND.h"
#include <stdio.h>

extern void UARTO_transmit(unsigned char data);
uint32_t ms_count = 0;	
uint32_t sec_count = 0;

uint32_t one_ms_count = 0;
uint32_t one_sec_count = 0;

uint32_t stopwatch_ms_count = 0;
uint32_t stopwatch_sec_count = 0;

FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);


uint32_t turn_select = 0; // 초시계에 돌아가는거 배열 카운터하는거임.
uint8_t dot_display = 0;
int run_select = 0;

int main(void){
		char _dot[] = ".";
		char _r[] = "\r\n";
	stdout = &OUTPUT;
		init_timer2();
	init_uart();
	void (*main_return[3])(void) = {
		fnd_main, fnd_second, fnd_stopwatch_
	};
	init_fnd();
	init_button();
	int button_select = 0;
	
	int reset_select = 0;
	/* Replace with your application code */
	while (1)
	{
		main_return[button_select]();
		if (get_button(BUTTON0, BUTTON0PIN)){
			button_select = (button_select + 1) % 3;
			turn_select = 0;
			dot_display = 0;
			one_ms_count = 0;
			one_sec_count = 0;
		}
		if (get_button(BUTTON1, BUTTON1PIN)){
			run_select = !run_select;
		}
		if (get_button(BUTTON2, BUTTON2PIN)){
			reset_select = !reset_select;
			stopwatch_ms_count  = 0;
			stopwatch_sec_count = 0;
		}
		if( get_button(BUTTON3, BUTTON3PIN)){
			switch(button_select){
				case 0:
					printf("Currently_time : ");
					UART0_print_1_byte_number(sec_count);
					UART0_print_string(_dot);
					UART0_print_1_byte_number(ms_count);
					UART0_print_string(_r);	
				break;
									
				case 1:
					printf("sec_time : ");
					UART0_print_1_byte_number(one_sec_count);
					UART0_print_string(_r);
				break;
				
				case 2:
					printf("stop_watch_time : ");
					UART0_print_1_byte_number(stopwatch_sec_count);
					UART0_print_string(_dot);
					UART0_print_1_byte_number(stopwatch_ms_count);
					UART0_print_string(_r);
				break;
				
				
			}
		}
	}
}

