/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:12
 * Author : kccistc
 */ 

#include "clock_config.h"
#include <avr/io.h>
#include "button.h"
#include "FND.h"
#include "uart.h"
#include <stdio.h>
uint32_t ms_count = 0;	
uint32_t sec_count = 0;
uint32_t turn_select = 0;
uint8_t dot_display = 0;
int run_select = 0;

// uart를 통해 전달되는 데이터를 스트림형태로 바꿔주기 위한 객체 생성
FILE OUTPUT \
	= FDEV_SETUP_STREAM(UART0_TxChar, NULL, _FDEV_SETUP_WRITE);
FILE INPUT \
	= FDEV_SETUP_STREAM(NULL, UART0_RxChar, _FDEV_SETUP_READ);

int main(void){
	
	// 입출력 객체 연결
	stdout = &OUTPUT;
	stdin = &INPUT;
	UART0_Init();
	
	void (*main_return[3])(void) = {
		fnd_main, fnd_second, fnd_stopwatch_
	};
	init_fnd();
	init_button();
	int button_select = 0;
	
	char str[] = "ms_count = ";
	char str1[] = "sec_count = ";
	char _space[] = " ";
	char _dot[] = ".";
	char _r[] = "\r\n";


	unsigned char num = 128;
	

	UART0_print_1_byte_number(num);
	UART0_print_1_byte_number("\n\r");
	
	UART0_print_string(str);
	UART0_print_string("\n\r");
	
	int reset_select = 0;
	/* Replace with your application code */
	while (1)
	{
		/* rx tx test code
		char c = UART0_RxChar();
		UART0_TxChar(c);
		*/
	
		main_return[button_select]();
		if (get_button(BUTTON0, BUTTON0PIN)){
			button_select = (button_select + 1) % 3;
			ms_count = 0;
			sec_count = 0;
			turn_select = 0;
			dot_display = 0;
		}
		if (get_button(BUTTON1, BUTTON1PIN)){
			run_select = !run_select;
			
		}
		if (get_button(BUTTON2, BUTTON2PIN)){
			reset_select = !reset_select;
			ms_count = 0;
			sec_count = 0;
		}
		if (get_button(BUTTON3, BUTTON3PIN)){
		
			printf("what time is it now : ");
			UART0_print_1_byte_number(sec_count);
			UART0_print_string(_dot);
			UART0_print_1_byte_number(ms_count);
			UART0_print_string(_r);
			

		}
	}
}

