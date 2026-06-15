/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:12
 * Author : kccistc
 */ 
#include "interrupt.h"
#include <avr/io.h>
#include "button.h"
#include "FND.h"

uint32_t ms_count = 0;	
uint32_t sec_count = 0;
uint32_t turn_select = 0;
uint8_t dot_display = 0;
int run_select = 0;
extern volatile uint16_t msec_count; // 이친구로 시간 카운트 하면 된다.


int main(void){
	init_timer2();
	
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
	}
}

