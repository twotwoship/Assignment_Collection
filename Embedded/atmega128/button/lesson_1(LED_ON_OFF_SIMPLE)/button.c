/*
 * button.c
 *
 * Created: 2026-06-10 오후 1:25:22
 *  Author: kccistc
 */

#include "button.h"
void init_button(void);
int get_button(int button_num, int button_pin);

void init_button(void){ // 초기화
	BUTTON_DDR &= ~(1 << BUTTON0PIN | 1 << BUTTON1PIN | 1 << BUTTON2PIN | 1 << BUTTON3PIN);
	// BUTTON_DDR &= 0xf0;
}
// button 을 눌렀다 떼면 : 1을 리턴
// idle 상태 : 0을 리턴
int get_button(int button_num, int button_pin){
	// static 지역변수에 static 을 선언하면 함수를 빠져나와 다시 들어와도 값이 남아있음.
	static unsigned char button_status[BUTTON0_NUMBER] = {
		BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE
	};
	int current_state;
	
	// 1.read button
	current_state = BUTTON_PIN & ( 1 << button_pin);
	// 2. state button check
	if(current_state && button_status[button_num] == BUTTON_RELEASE){ // 버튼이 처음 눌러진 상태.
		_delay_ms(15); // noise skip	
		button_status[button_num] = BUTTON_PRESS;
		return 0;
	}else if(button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE){
		//버튼이 이전에 눌러진 상태였으나 지금은 떨어진 상태.
		button_status[button_num] = BUTTON_RELEASE; // 버튼 초기화
		_delay_ms(15); // noise skip	
		return 1;
	}
	
	return 0; // button status is open.
}