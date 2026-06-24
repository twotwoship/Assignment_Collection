/*
 * button.c
 *
 * Created: 2026-06-19 오후 8:58:46
 *  Author: kccistc
 */ 

#include "button.h"

volatile uint16_t debounce_count = 0;

void init_button(void){
	BUTTON_DDR &= ~(1 << BUTTON7PIN );
	// BUTTON_DDR &= 0xf0;
	// BUTTON_DDR &= 0x0f
}
int get_button(int button_num, int button_pin){
	
	static unsigned char button_status[BUTTON_NUMBER] =
	{	BUTTON_RELEASE	};
	// static 지역변수에 static을 선언하면 함수를 빠져나와 다시 들어가도 이전값유지
	int current_state;
	
	// 1. button을 읽는다.
	current_state = BUTTON_PIN & (1 << button_pin);
	// 2. 버튼 상태 check
	if (current_state  && button_status[button_num] == BUTTON_RELEASE)  // 버튼이 처음 눌려진 상태
	{
		button_status[button_num] =BUTTON_PRESS;
		debounce_count = 0;
		return 0;     // 아직은 완전히 눌렀다 뗀 상태가 아니다.
	}
	else if (button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE)
	{  // 버튼이 이전에 눌려진 상태였으며 지금은 떼어진 상태
		if(debounce_count >= 10){
			button_status[button_num] = BUTTON_RELEASE;   // 다음 버튼을 체크 하기 위하여 초기화
			debounce_count = 0;
			return 1;   // 완전히 1번 눌렀다 뗀 상태로 인정 한다.
		}
		
	}
	
	return 0;   // 버튼이 open 상태 이다.
}
