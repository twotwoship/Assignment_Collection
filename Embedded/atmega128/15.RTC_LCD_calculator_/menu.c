/*
* menu.c
*
* Created: 2026-07-01 오후 7:50:48
*  Author: kccistc
*/
#include "text_lcd.h"
#include "ds1307.h"
#include "button.h"
#include "uart0.h"
#include "timer0.h"
#include "keypad_cal_lcd.h"


volatile int func_state = WATCH_MODE;
extern volatile uint16_t ms_count;
extern volatile uint8_t _first;
extern volatile uint8_t button0_flag;
extern volatile uint8_t button1_flag;
extern volatile uint8_t button2_flag;
extern volatile uint8_t button3_flag;
extern volatile uint8_t button4_flag;
void (*pfunc[])(t_ds1307 *) = {
	ds1307_text_lcd_Synchronization, ds1307_watch_setting, calculator_text_lcd_Synchronization
};

// 자 버튼으로 구현하자
// 3번 버튼 누르면 세팅모드로 바뀌어야됨. 3번 다시누르면 다시 원래대로 돌아오고.
// 0번 버튼 감소 // 계산기모드에서 (
// 1번 버튼 증가 // 계산기모드에서 )
// 2번 버튼 연월일날 시분초 순으로 돌아가게 만들기. 계산기모드에서 취소
// 4번 버튼 계산기 모드로 넘어감

// 버튼 3번하고 버튼 4번으로만 전체통제한다.
// 3번 딸깍하면 세팅모드하고 시계모드 번갈아가면서
// 4번 딸깍하면 시계모드하고 계산기모드 번갈아가면서
// 최초모드는 시계모드이다

//버튼 존나 안눌림 1초에 한번씩만 시간 불러오게 하기
void _RTC_LCD_calculator_control(t_ds1307 *ds1307){
	static uint16_t prev_ms_time = 0;

	// 여기 안에함수에서 변경하게 만들꺼임 약간수정필요.
	// 4번 모드일때는 또 다르게 되야됨 버튼동작이.

	if(button3_flag){	//3번 버튼 누르면 세팅 < - > 시계모드	get_button(BUTTON3, BUTTON3PIN) == 1
		button3_flag = 0;
		if(func_state == WATCH_MODE){
			func_state  = WATCH_SETTING_MODE;
			_first = 1;
			lcd_clear();
			}else if(func_state == WATCH_SETTING_MODE){
			timer_test_2_set(ds1307);
			func_state  = WATCH_MODE;
			_first = 1;
			lcd_clear();
		}
		pfunc[func_state](ds1307);   // 상태 바뀐 직후 바로 화면 갱신
		prev_ms_time = ms_count;     // 1초 타이머 리셋
	}
	
	if(button4_flag && func_state == WATCH_MODE)
	{
		button4_flag = 0;

		func_state = CALCULATE_MODE;
		_first = 1;
	    reset_count = 0;
	    cal_reset_ms_count = 0;

		lcd_clear();

	pfunc[func_state](ds1307);   // 상태 바뀐 직후 바로 화면 갱신
	prev_ms_time = ms_count;     // 1초 타이머 리셋
}
if(func_state == CALCULATE_MODE)
{
	pfunc[func_state](ds1307);     // 계산기 모드는 계속 실행
}
else
{
	if((uint16_t)(ms_count - prev_ms_time) >= 1000)
	{
		prev_ms_time = ms_count;
		pfunc[func_state](ds1307);
	}
}
}

