/*
 * button.c
 *
 * Created: 2026-06-12 오후 1:29:41
 *  Author: kccistc
 */ 

#include "button.h"

#include <avr/delay.h>
void init_button(void);
int get_button(int button_num, int button_pin);
extern volatile uint16_t msec_count;
extern volatile uint16_t debounce_count;


void init_button(void){
		BUTTON_DDR &= ~(1 << BUTTON0PIN | 1 << BUTTON1PIN | 1 << BUTTON2PIN | 1 << BUTTON3PIN);
		// BUTTON_DDR &= 0xf0;
		// BUTTON_DDR &= 0x0f
}
// 예) BUTTON0 3
//    button을 눌렀다 떼면 : 1을 리턴
//          이 idle 상태 : 0을 리턴

int get_button(int button_num, int button_pin){
		static uint16_t msec_check = 0;
		
		static unsigned char button_status[BUTTON_NUMBER] =
		{
			BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE
		};
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
			if(debounce_count >= 15){
				button_status[button_num] = BUTTON_RELEASE;   // 다음 버튼을 체크 하기 위하여 초기화
				debounce_count = 0;
				return 1;   // 완전히 1번 눌렀다 뗀 상태로 인정 한다.	
			}
			
		}
		
		return 0;   // 버튼이 open 상태 이다.
}
