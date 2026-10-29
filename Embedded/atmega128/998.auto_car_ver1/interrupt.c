/*
 * interrupt.c
 *
 * Created: 2026-06-20 오후 2:30:46
 *  Author: kccistc
 */ 
#include "interrupt.h"
#include "ultrasonic.h"
#include <util/delay.h>  // _delay_ms _delay_us 등
#include "fnd.h"

volatile uint16_t msec_count = 0;
volatile uint16_t left_count  = 0;
volatile uint16_t mid_count  = 0;
volatile uint16_t right_count  = 0;
volatile uint16_t _us_count  = 0;
volatile uint16_t _motor_count  = 0;


volatile char left_scm[50];
volatile char mid_scm[50];
volatile char right_scm[50];
volatile uint16_t auto_tick_count = 0;

extern volatile int high_left_pulse;
extern volatile int low_left_pulse;
extern volatile int high_mid_pulse;
extern volatile int low_mid_pulse;
extern volatile int high_right_pulse;
extern volatile int low_right_pulse;
extern volatile int trigger_doing_left;
extern volatile int trigger_doing_mid;
extern volatile int trigger_doing_right;
extern volatile uint16_t prev_count;
extern int run_record_count;
extern volatile int func_state;
extern volatile int _non_delay_left_trigger_wait;


ISR(TIMER0_OVF_vect){		//1ms timer
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	msec_count++; // 1ms count
	debounce_count++;
	_motor_count++;
	if(_motor_count > 1000){
		_motor_count = 0;
	}
		prev_count = 0;
		if(msec_count > MS_COUNT_RESET){
			msec_count = 0;
		}
	if (func_state == 1){
		auto_tick_count++;
		if (auto_tick_count >= 1000){
			auto_tick_count = 0;
			run_record_count++;
		}
	}else{
	auto_tick_count = 0;
	}
}
/*
29us당 1cm 이동
초음파는 왔다 갔다기 때문에 시간은 2배로 측정됨.
tcnt2는 1상승당 16us // count 
만약 58us면 1cm의 거리가 있다는거임
1cm는  64us로 갑시다. 간단하게.
어림계산하자 
4count가 1cm인걸로 하자.
*/

ISR(TIMER1_OVF_vect){
	TCNT1 = 0;	
}

ISR(TIMER2_OVF_vect){		// 16us timer 오버플로발생해서 저시간마다 들어옴.
	TCNT2 = 0;				// 0.016/256 카운터 ㅋㅋ 1틱당 0.625us
	_us_count++;			//  16us timer 마다 커지는거임.
	if(_us_count> _US_COUNT_RESET){
		_us_count= 1;		// 트리거 체크때문에 0으로 만들면 안됨.
	}
}

/*			임시 저장본
ISR(INT4_vect){		//ultrasonic left interrupt
	led_on();
	_delay_ms(1000);
	_led_off();
	_delay_ms(1000);
	//TCNT1 사용 1올라갈때마다 4us임 그러면 14.5개마다 1cm인거임
	// 에코로 상승 하강 엣지 발생할때마다 인터럽트가 딱 들어온다
	//어 떻게 tcnt2는 1이 count 될때마다 0.016ms임 즉 16us
	if(ECHO_LAFT_PORT & ( 1 << ECHO_LAFT_PIN)){ // high pulse
		// 상승 펄스 발생 후 하강펄스까지의 시간을 체크해야됨.
		//tcnt
		//TCNT2
		left_count = TCNT1;
		high_left_pulse = 1;
		}else{ // 4개 차이당 1임. 하강펄스 딱 발생했어! // low pulse
		//ultrasonic_left_distance =
		if(_us_count > left_count){ // 노초기화
			ultrasonic_left_distance = (_us_count - left_count)  / 4;
			}else{ // 초기화
			ultrasonic_left_distance = (_us_count + _US_COUNT_RESET - left_count)  / 4;
		}
		low_left_pulse = 1;
		trigger_doing_left = 0;
	}
}
*/

ISR(INT4_vect){	// echo핀에서 나온 게 들어간다 보드로
	// 타이머 2번을 쓸꺼임 타이머 2번은 분주를 하지 않음.
	//tcnt2 노분주로 타이머2인터럽트가 들어올때마다 1개씩 커짐.
	if(ECHO_LAFT_PORT & ( 1 << ECHO_LAFT_PIN)){	// 1. high edge
		left_count = _us_count;		// 딱 상승이 발생햇어
		high_left_pulse = 1;
	}else{	// 2. low edge
		low_left_pulse = 1;
		_non_delay_left_trigger_wait = 0;
	}
}

ISR(INT5_vect){		//ultrasonic mid interrupt
	if(ECHO_MID_PORT & ( 1 << ECHO_MID_PIN)){
		mid_count = TCNT3;
		high_mid_pulse = 1;
	}else{
		ultrasonic_mid_distance = (TCNT3 - mid_count) * 1000000.0 * 1024 / F_CPU / 58;
		low_mid_pulse = 1;
		trigger_doing_mid = 0;
		//sprintf(mid_scm,"mid:%dcm\n",ultrasonic_mid_distance);
	}
		
}	

ISR(INT6_vect){		//ultrasonic right interrupt
	if(ECHO_RIGHT_PORT & ( 1 << ECHO_RIGHT_PIN)){
		right_count = TCNT3;
		high_right_pulse = 1;
	}else{ 
		ultrasonic_right_distance = (TCNT3 - right_count) * 1000000.0 * 1024 / F_CPU / 58;
		low_right_pulse = 1;
		trigger_doing_right = 0;
		//sprintf(right_scm,"mid:%dcm\n",ultrasonic_right_distance);
	}
}	

void init_interrupt(void){
	
}