/*
 * ultrasonic.c
 *
 * Created: 2026-06-20 오후 3:08:14
 *  Author: kccistc
 */ 
#include "ultrasonic.h"
#include <util/delay.h>  // _delay_ms _delay_us 등


volatile int ultrasonic_left_distance = 0;
volatile int ultrasonic_mid_distance = 0;
volatile int ultrasonic_right_distance = 0;

volatile int ultrasonic_left_tige_count = 0;
volatile int ultrasonic_mid_tige_count = 0;
volatile int ultrasonic_right_tige_count = 0;

volatile int high_left_pulse = 0;
volatile int low_left_pulse = 0;
volatile int high_mid_pulse = 0;
volatile int low_mid_pulse = 0;
volatile int high_right_pulse = 0;
volatile int low_right_pulse = 0;

volatile int trigger_left_high_check = 0;
volatile int trigger_mid_high_check = 0;
volatile int trigger_right_high_check = 0;

volatile int trigger_doing_left = 0;
volatile int trigger_doing_mid = 0;
volatile int trigger_doing_right = 0;

volatile int _non_delay_left_trigger_wait = 0;

volatile int high_pulse = 0;
volatile int low_pulse = 0;

volatile uint16_t prev_count = 0;

extern volatile uint16_t _us_count;

extern volatile char left_scm[50];
extern volatile char mid_scm[50];
extern volatile char right_scm[50];
extern volatile uint16_t _motor_count;


void init_ultrasonic(void){
	// output pin setting
	TRIG_LAFT_DDR |= 1 << TRIG_LAFT_PIN;
	TRIG_MID_DDR |= 1 << TRIG_MID_PIN;
	TRIG_RIGHT_DDR |= 1 << TRIG_RIGHT_PIN;
	
	// input pin setting
	ECHO_LAFT_DDR &= ~(1 << ECHO_LAFT_PIN);	
	ECHO_MID_DDR &= ~(1 << ECHO_MID_PIN);
	ECHO_RIGHT_DDR &= ~(1 << ECHO_RIGHT_PIN);
	
	EICRB |= 0 << ISC41 | 1 << ISC40;	//external INT : high edge low edge both interrupt request
	EICRB |= 0 << ISC51 | 1 << ISC50;	//external INT : high edge low edge both interrupt request
	EICRB |= 0 << ISC61 | 1 << ISC60;	//external INT : high edge low edge both interrupt request
	
	EIMSK |= 1 << INT4;		//external INT4(ECHO PIN) 허용
	EIMSK |= 1 << INT5;		//external INT5(ECHO PIN) 허용
	EIMSK |= 1 << INT6;		//external INT6(ECHO PIN) 허용
	// 인터럽트 발생지점하고 발생 시점 정하기.
}


// 하는 일을 다 쪼개야 한다.
	//_us_count // 를 비교해서 1차이나면 내리면 됨.


void ultrasonic_task(void)
{
	static uint8_t sensor_step = 0;

	if ((_motor_count - prev_count) < ULTRA_INTERVAL)
	return;

	prev_count = _motor_count;

	switch(sensor_step)
	{
		case 0:
		make_trigger(LAFT_TYPE);
		break;

		case 1:
		make_trigger(MID_TYPE);
		break;

		case 2:
		make_trigger(RIGHT_TYPE);
		break;
	}

	sensor_step = (sensor_step + 1) % 3;
}

void make_trigger(int type){
	switch(type){
		case LAFT_TYPE:
		if(trigger_doing_left == 0){
			_ultrasonic_processing_flag_laft();
		}
		break;
		case MID_TYPE:
		if(trigger_doing_mid == 0){
			_ultrasonic_processing_flag_mid();
		}
		break;
		case RIGHT_TYPE:
		if(trigger_doing_right == 0){
			_ultrasonic_processing_flag_right();
		}
		break;
		
	}
}

void left_make_trigger(void){
	if(trigger_doing_left == 0){
		trigger_doing_left = 0;
		TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);	// low
		_delay_us(1);
		TRIG_LAFT_PORT |= (1 << TRIG_LAFT_PIN);	//high
		_delay_us(15);						// minimum retention time rule 10us
		TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);	// low
	}
}
void mid_make_trigger(void){
	if(trigger_doing_mid == 0){
		trigger_doing_mid = 0;
		TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);	// low
		_delay_us(1);
		TRIG_MID_PORT |= (1 << TRIG_MID_PIN);	//high
		_delay_us(15);						// minimum retention time rule 10us
		TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);	// low
	}
}

void right_make_trigger(void){
	if(trigger_doing_right == 0){
		trigger_doing_right = 0;
		TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);	// low
		_delay_us(1);
		TRIG_RIGHT_PORT |= (1 << TRIG_RIGHT_PIN);	//high
		_delay_us(15);						// minimum retention time rule 10us
		TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);	// low
	}	
}

void _non_delay_left_make_trigger_high(void){		// 딜레이함수 쓰지 않고 초음파 쏘기 좌 하이
	TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);	// low
	TRIG_LAFT_PORT |= (1 << TRIG_LAFT_PIN);	//high
}
void _non_delay_left_make_trigger_low(void){		// 딜레이함수 쓰지 않고 초음파 쏘기 좌 로우
	TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);	// low
}
void _non_delay_left_make_trigger_processing(void){ // 딜레이 함수 쓰지 않고 초음파 쏘기 좌 통합
	// 어떻게 돌아가야하냐면 하이상태를 15us 유지하고 로우로 내려야됨.
	// 내리는건 타이머 인터럽트에서 해버리자.
	if(high_left_pulse == 0 && low_left_pulse == 0 && _non_delay_left_trigger_wait == 0){	
		_non_delay_left_make_trigger_high();		// 쏘고나서 15us 만큼 기다려야됨
		_non_delay_left_trigger_wait = _us_count;
	}
	if(_non_delay_left_trigger_wait != 0){ // 하이로 쏜상태임.
		if ((_us_count - _non_delay_left_trigger_wait) >= 1)
		{
			_non_delay_left_make_trigger_low();
		}
	}
	if(high_left_pulse == 1 && low_left_pulse == 1){ // 이거는 이제 에코가 다 들어오고나면 000으로 다 내려서 다시 하이를 쏠수있게 준비
		high_left_pulse = 0;
		low_left_pulse = 0;
		_non_delay_left_trigger_wait = 0;
	}
}

void _non_delay_mid_make_trigger_high(void){
	TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);	// low
	TRIG_MID_PORT |= (1 << TRIG_MID_PIN);	//high
}
void _non_delay_mid_make_trigger_low(void){
	TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);	// low
}
void _non_delay_right_make_trigger_high(void){
	TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);	// low
	TRIG_RIGHT_PORT |= (1 << TRIG_RIGHT_PIN);	//high
}
void _non_delay_right_make_trigger_low(void){
	TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);	// low
}

void _ultrasonic_processing_flag_laft(void){
	if(high_left_pulse == 0 && low_left_pulse== 0){
			left_make_trigger();
	}
	if(high_left_pulse == 1 && low_left_pulse == 1){
//		printf("%s", left_scm);
		high_left_pulse = 0;
		low_left_pulse = 0;
		}else{
	}
}

void _ultrasonic_processing_flag_mid(void){
	if(high_mid_pulse == 0 && low_mid_pulse== 0){
		mid_make_trigger();
	}
	if(high_mid_pulse == 1 && low_mid_pulse == 1){
//		printf("%s", mid_scm);
		high_mid_pulse = 0;
		low_mid_pulse = 0;
		}else{
	}
}

void _ultrasonic_processing_flag_right(void){
	if(high_right_pulse == 0 && low_right_pulse== 0){
			right_make_trigger();
	}
	if(high_right_pulse == 1 && low_right_pulse == 1){
//		printf("%s", right_scm);
		high_right_pulse = 0;
		low_right_pulse = 0;
		}else{
	}
}
void litrasonic_processing_flag(void){
	if(high_left_pulse == 0 && low_left_pulse== 0){
			right_make_trigger();
	}
	if(high_left_pulse == 1 && low_left_pulse == 1){
//		printf("%s", left_scm);
		high_left_pulse = 0;
		low_left_pulse = 0;
		}else{
	}
}
// 아래는 _delay_us 안쓰고 해보렸던 흔적 시간 남을때 주말에 해보기
/*
void make_trigger_doing(int type){ // 에코가 들어온게 끝나면 다시 트리거 작동하게 만들어야됨.
	switch(type){
		case LAFT_TYPE:
		if(trigger_doing_left == 0){
			make_trigger_high(type);
			make_trigger_low(type);
		}
		break;
		case MID_TYPE:
		if(trigger_doing_mid == 0){
			make_trigger_high(type);
			make_trigger_low(type);
		}
		break;
		case RIGHT_TYPE:
		if(trigger_doing_right == 0){
			make_trigger_high(type);
			make_trigger_low(type);
		}
		break;
		
	}
}


void make_trigger_high(int type){ // 인자를 좌 중 우로 받아서 이것도 디파인 해놓고 트리거를  하이주는 트리거 따로주고
	// 로우주는 트리거를 따로 설정해줘야됨 같이 설정하면 결국엔 멈추게 되어있음 비교연산으로 가야된다.
	// make_trigger_high 하고 make_trigger_low를 따로 만들어서 high 트리거가 발생한지 15us 뒤에 low 트리거가 발생하게 되야됨
	// 전체 while 안에서 돌아가게;
	
	//TRIG_LAFT_PORT |= (1 << TRIG_LAFT_PIN);	  // test

	switch(type){ 
		case LAFT_TYPE:
			if(trigger_left_high_check == 0){
				trigger_left_high_check = 1;
			}else{
				break;
			}
			TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);	// low
			TRIG_LAFT_PORT |= (1 << TRIG_LAFT_PIN);	//high
			ultrasonic_left_tige_count = _us_count;		// 카운터 셀꺼 넣어줘야됨. 
		break;
		case MID_TYPE:
			if(trigger_mid_high_check == 0){
				trigger_mid_high_check = 1;
			}else{
				break;
			}
			TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);
			TRIG_MID_PORT |= (1 << TRIG_MID_PIN);
			ultrasonic_mid_tige_count = _us_count;
		break;
		case RIGHT_TYPE:
			if(trigger_right_high_check == 0){
				trigger_right_high_check = 1;
			}else{
				break;
			}
			TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);
			TRIG_RIGHT_PORT |= (1 << TRIG_RIGHT_PIN);
			ultrasonic_right_tige_count = _us_count;
		break;
	}
}

void make_trigger_low(int type){ // 인자를 좌 중 우로 받아서 이것도 디파인 해놓고 트리거를  하이주는 트리거 따로주고
	// 로우주는 트리거를 따로 설정해줘야됨 같이 설정하면 결국엔 멈추게 되어있음 비교연산으로 가야된다.
	// make_trigger_high 하고 make_trigger_low를 따로 만들어서 high 트리거가 발생한지 15us 뒤에 low 트리거가 발생하게 되야됨
	// 전체 while 안에서 돌아가게;
	
	// low 트리거가 발생하면 초음파를 보내기 시작한다. 파바바바박 이때 trigger_high와 trigger_low의 발생 시간차이는 최소 10us
	// 나는 16us 줄꺼임
	

	
	switch(type){
		case LAFT_TYPE:
			if(trigger_left_high_check == 1){
				uint16_t temp = _us_count;
				if(temp - ultrasonic_left_tige_count >= TRIG_HIGH_TICK){ // 비교하기 어차피 1인터럽트인데 다르면 16us 지난거지 ㅋㅋ	
					trigger_left_high_check = 0;
					trigger_doing_left = 1;
					TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);
					}else{
					break;
				}	
				
			}
		break;
		case MID_TYPE:
			if(trigger_mid_high_check == 1){
				uint16_t temp = _us_count;
				if(temp - ultrasonic_mid_tige_count >= TRIG_HIGH_TICK){ // 비교하기 어차피 1인터럽트인데 다르면 16us 지난거지 ㅋㅋ
					trigger_mid_high_check = 0;
					trigger_doing_mid = 1;
					}else{
					break;
				}			
				TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);
			}
		break;
		case RIGHT_TYPE:
			if(trigger_right_high_check == 1){
				uint16_t temp = _us_count;
				if(temp  - ultrasonic_right_tige_count >= TRIG_HIGH_TICK){ // 비교하기 어차피 1인터럽트인데 다르면 16us 지난거지 ㅋㅋ
					trigger_right_high_check = 0;
					trigger_doing_right = 1;
					}else{
					break;
				}
				TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);
			}
		break;
	}
}

void make_left_trigger(void){
	// low
	TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);
	//high
	TRIG_LAFT_PORT |= ~(1 << TRIG_LAFT_PIN);
	// 10us 만큼의 상승신호를 보내주는 여유를 줘야함. 어떻게? 15를 주는게 안전함
	//low
	TRIG_LAFT_PORT &= ~(1 << TRIG_LAFT_PIN);

}

void make_mid_trigger(void){
	// low
	TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);
	//high
	
	TRIG_MID_PORT |= ~(1 << TRIG_MID_PIN);
	
	// 10us 만큼의 상승신호를 보내주는 여유를 줘야함. 어떻게?
	
	//low
	TRIG_MID_PORT &= ~(1 << TRIG_MID_PIN);

}

void make_right_trigger(void){
	// low
	TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);
	//high
	
	TRIG_RIGHT_PORT |= ~(1 << TRIG_RIGHT_PIN);
	
	// 10us 만큼의 상승신호를 보내주는 여유를 줘야함. 어떻게?
	//_us_count // 를 비교해서 1차이나면 내리면 됨.
	
	//low
	TRIG_RIGHT_PORT &= ~(1 << TRIG_RIGHT_PIN);
}

void ultrasonic_left_process(void){
	//make_left_trigger();
	if(high_left_pulse == 1 && low_left_pulse == 1){
		// 나중에 뭐 출력할 것 있으면 여기다 집어넣기. 상승 하강 펄스가 발생하고 거리가 나왓을때만 값이 나온다.
		//ultrasonic_left_distance이 발생한 시점.
		high_left_pulse = 0;
		low_left_pulse = 0;
	}
}

void ultrasonic_mid_process(void){
	//make_mid_trigger();
	if(high_mid_pulse == 1 && low_mid_pulse == 1){
		high_mid_pulse = 0;
		low_mid_pulse = 0;
	}
}

void ultrasonic_right_process(void){
	//make_right_trigger();
	if(high_right_pulse == 1 && low_right_pulse == 1){
		high_right_pulse = 0;
		low_right_pulse = 0;
	}
}

*/