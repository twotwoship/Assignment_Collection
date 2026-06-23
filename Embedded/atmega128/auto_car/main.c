/*
 * 06.DCMOTOR_PWM_CONTROL.c
 *
 * Created: 2026-06-18 오후 2:04:08
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>

extern volatile uint16_t _us_count;
extern volatile char scm[50];
extern int run_record_count;

#include "BUTTON.h"
#include "FND.h"
#include "led1.h"
#include "motor.h"
#include "timer0.h"
#include "timer1.h"
#include "timer2.h"
#include "time3.h"
#include "interrupt.h"
#include "ultrasonic.h"
#include "uart0.h"
#include "uart1.h"
#include "menu.h"
#include "button.h"

extern volatile uint16_t _motor_count;

FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);


int main(void)
{
	init_time3();
	init_button();
	init_led();
	init_fnd();
	init_timer0();
	init_timer1();
	init_motor();
	init_uart();
	init_uart1();
	init_timer2();
	init_ultrasonic();
	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.
	_led_off();

	
	
	sei();	// timer interrupt	
    while(1){
		//fnd_test();

		pfunc[func_state]();
		if(get_button(BUTTON7, BUTTON7PIN) == 1){
			func_state++;
		}
		
	}
}
/*
	 맨처음에 수동모드에서 작동하다가 버튼을 누르면 AUTO_MODE로 들어가고 버튼을 한번더 누르면
	MANUAL_MODE로 들어가기
	
	AUTO_MODE에서는 좌 중앙 우 초음파 센서를 통해서 거리를 측정하며 거리가 일정거리 이하로 줄어들었을 때 후진하고 막히지 않은 방향으로 가야함.
	그걸 계속해서 반복한다.
	
	버튼을 한번더 누르면 AUTO_MODE_CHECK에서 주행시간 후진횟수 자회전 횟수 우회전 횟수가 나와야한다
	
	버튼을 한번더 누르면 다시 mamual_mode 한번더 누르면 다시 aut모드
*/	