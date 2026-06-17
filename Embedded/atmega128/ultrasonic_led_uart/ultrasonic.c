/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:32:58
 *  Author: kccistc
 */ 
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include "ultrasonic.h"
#include <util/delay.h>  // _delay_ms _delay_us 등

extern volatile int ultrasonic_check_time;
volatile int ultrasonic_distance = 0;
volatile char scm[50];

volatile int high_pulse = 0;
volatile int low_pulse = 0;
//p278 grape 12-3
//INT4 : PE4 external INT4 ultrasonic senser high low edge signal
//0x000A address
ISR(INT4_vect){
	
	if(ECHO_PORT & ( 1 << ECHO_PIN)){	// 1. high edge
		TCNT1 = 0;
		high_pulse = 1;
	}else{	// 2. low edge
		// TCNT1 : timer count 1
		// ex) TCNt1 is 10
		// 15.625KHz의 1주기 : 64us
		// 0.000064sec * 10개 => 640us
		// 640us / 58(1cm 이동하는데 소요시간) : 11cm
		// 1sec : 1000000us
		low_pulse = 1;
		ultrasonic_distance = TCNT1 * 1000000.0 * 1024 / F_CPU;
		// 소요 시간을 cm 으로 환산
		sprintf(scm,"dis:%dcm\n",ultrasonic_distance / 58);
	}
	//상승 하강을 다 플래그로해서 체크가 되면 다른 함수를 하나 새로 파서 해라.
}

void init_ultrasonic(void){
	TRIG_DDR |= 1 << TRIG_PIN;		//output mode setting
	ECHO_PORT &= ~(1<< ECHO_PIN);	//input mode setting	// .p289 grape 12-6, p288 picture 12-8 
	// ISC41 0 ISC41 1 : high edge low edge both interrupt request
	EICRB |= 0 << ISC41 | 1 << ISC40;
	// 16bit timer/counter 1번 사용 
	// timer int사용 안함
	// 16bit timer1 16bit로 표시할 수 있는 최대값 65535 : 0xffff
	// 16MHz / 1024 분주 : 16000000/1024 = 15625Hz = 15.625KHz
	// 1주기 : t = 1/f= 0.000064sec = 64us
	// 분주비 1024설정 p318 표14-1
	TCCR1B |= 1 << CS12 | 1 << CS10;
	
	//-------------- EINT4 SETTING------------
	// P287 picture 12-6
	EIMSK |= 1 << INT4;		//external INT4(ECHO PIN)
}

void make_trigger(void){
	TRIG_PORT &= ~(1 << TRIG_PIN);		// low로 만든다.
	_delay_us(1);
	TRIG_PORT |= 1 << TRIG_PIN;			// high로 만든다.
	_delay_us(15);						// minimum retention time rule 10us
	TRIG_PORT &= ~(1 << TRIG_PIN);		// low로 만든다.
}

void ultrasonic_processing(void){
	if(ultrasonic_check_time >= 1000){	//1초에 한번씩 트리거 펄스를 날린다.
		ultrasonic_check_time = 0;
		
		printf("%s", scm);
		make_trigger();
	}
}

//1초에 한번씩 트리거를 날리지말고 상승엣지 하강엣지가 발생하면 트리거를 날려라.

void litrasonic_processing_flag(void){
	make_trigger();
	if(high_pulse == 1 && low_pulse == 1){
		printf("%s", scm);
		high_pulse = 0;
		low_pulse = 0;
	}else{
	}
	
}