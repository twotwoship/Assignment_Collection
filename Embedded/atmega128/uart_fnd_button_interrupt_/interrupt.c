/*
 * interrupt.c
 *
 * Created: 2026-06-15 오후 4:20:42
 *  Author: kccistc
 */ 
#include "interrupt.h"

volatile uint16_t msec_count = 0;
extern uint32_t ms_count;
extern uint32_t sec_count;
extern uint32_t one_ms_count;
extern uint32_t one_sec_count;
extern uint32_t stopwatch_ms_count;
extern uint32_t stopwatch_sec_count;
extern uint32_t turn_select;
extern uint8_t dot_display;
extern int run_select;
extern int debounce_check;


/*
ISR interrupt service routine : 인터럽트 처리 함수 isr로 시작
TIMER0_OVF_vect : timer0 overflow int가 발생이 되면 이곳으로 진입한다.
250개의 펄스를 count(1ms)하면 이곳으로 자동 진입 한다.
ISR은 가능한 짧게 작성한다.
*/

/*
1. timer 0을 초기화 한다.
	AVR에서 8bit timer 0번, 2번 중에서 0번을 초기화 한다.
	임베디드에서 가장 신경을 써야 할 부분을 초기화 하는 것이다.
	초기화가 잘못되면 이후가 다 꼬인다.
2. 8bit 가지고 1ms를 측정 하는 timer/counter를 만들고자 한다.
2-1. 분주비 설정(1/256)
	16000000hz / 256 = 62,500hz
2-2. 1주기가 잡아먹는 시간 계산
	t = 1/f = 1/62,500 = 0.016ms
2-3. 8bit 가지고 count 하는 시간을 계산 ( 8bit timer ov(over flow)
	0.016ms * 62개  = 992ms, 0.992sec
*/

ISR(TIMER2_OVF_vect){ // 여기 들어오는 간격이 1ms 인거임.
	TCNT2 = 194;	// TCNT0 194~256 : 62개 pulse count 124 196 227 더쪼개봐 243
	//msec_count++; // 1ms count
	ms_count++;
	if(ms_count >= 1000){
		ms_count = 0;
		sec_count++;
	}

	one_ms_count++;
	if(one_ms_count >= 1000){
		one_ms_count = 0;
		one_sec_count++;
		turn_select++;
	}
	if(run_select == 1){
		stopwatch_ms_count++;
		if(stopwatch_ms_count >= 1000){
			stopwatch_ms_count = 0;
			stopwatch_sec_count++;
		}	
	}
	debounce_check++;	
	
}

void init_timer2(void){
	TCNT2 = 194;	// TCNT0 6~256 : 250개 pulse count
	
	TCCR2 &= ~(1 << CS22 | 1 << CS21 | 1 << CS20);
	TCCR2 |= 1 << CS22 | 0 << CS21 | 0 << CS20;
	TIMSK |= 1 << TOIE2; // TIMER0 OVERFLOW INT 활성화
	sei();	// 전역(대문) interrrupt 허용
}