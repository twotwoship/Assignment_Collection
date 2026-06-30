/*
 * 12.I2C_M_LOOKBACK.c
 *
 * Created: 2026-06-30 오전 10:45:43
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <stdio.h>

#include "uart0.h"
#include "i2c_m_lookback.h"

volatile int msec_count = 0;
			
//파일을 맵핑해줘야됨. p232
FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);

/*
ISR interrupt service routine : 인터럽트 처리 함수 isr로 시작
TIMER0_OVF_vect : timer0 overflow int가 발생이 되면 이곳으로 진입한다.
250개의 펄스를 count(1ms)하면 이곳으로 자동 진입 한다.
ISR은 가능한 짧게 작성한다.
*/

ISR(TIMER0_OVF_vect){		//4ms timer
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count 
	msec_count++;
}

int main(void){
	

	init_timer0();
	init_uart();

	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.
	sei();	// 전역(대문) interrrupt 허용
	i2c_main();

    while (1){

	}
}
/*
1. timer 0을 초기화 한다.
	AVR에서 8bit timer 0번, 2번 중에서 0번을 초기화 한다.
	임베디드에서 가장 신경을 써야 할 부분을 초기화 하는 것이다.
	초기화가 잘못되면 이후가 다 꼬인다.
2. 8bit 가지고 1ms를 측정 하는 timer/counter를 만들고자 한다.
2-1. 분주비 설정(1/64)
	16000000hz / 64 = 250,000hz
2-2. 1주기가 잡아먹는 시간 계산
	t = 1/f = 1/250,000 = 0.004ms
2-3. 8bit 가지고 count 하는 시간을 계산 ( 8bit timer ov(over flow)
	0.004ms * 256개  = 1.024ms
	0.004ms * 250개  = 1.000ms, 0.001sec
*/
void init_timer0(void){	//4msec
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count 
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0; // TIMER0 OVERFLOW INT 활성화	
}
