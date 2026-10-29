/*
* menu.c
*
* Created: 2026-06-22 오후 2:14:41
*  Author: kccistc
*/

#include "motor.h"
#include "menu.h"
#include "ultrasonic.h"
#include "button.h"
#include "interrupt.h"

#include <util/delay.h>  // _delay_ms _delay_us 등

volatile int func_state = MANUAL_MODE;
extern volatile uint16_t _motor_count;

int motor_state = 0;
int prev_motor_state = 0;
int trun_left_count = 0;
int trun_right_count = 0;
int run_record_count = 0;

void (*pfunc[])() = {
	manual_mode, auto_mode, auto_mode_check
};
void manual_mode(void){
	run_record_count = 0;
	trun_right_count = 0;
	trun_left_count = 0;
	fnd_passvie_display();
	switch(bt_data){
		case 'f':
		case 'F':
		motor_forward();
		motor_speed(0, 500);
		break;
		case 'b':
		case 'B':
		motor_backward();
		motor_speed(500, 0);
		break;
		case 'l':
		case 'L':
		motor_left_turn();
		motor_speed(500, 500);
		break;
		case 'r':
		case 'R':
		motor_right_turn();
		motor_speed(500, 500);
		break;
		case 's':
		case 'S':
		motor_stop();
		motor_speed(0, 0);
		break;
		case 'n':
		func_state = (func_state + 1) % 3;
		break;
		default:
		break;
	}
}

void auto_mode(void){
	fnd_running();
	left_make_trigger();
	static int first = 1;
	if (first){
		motor_stop();
		motor_speed(300, 300);
		first = 0;
	}	motor_speed(300, 300);
	static uint16_t _compare_count  = 0;
	static uint16_t sonic_compare_count  = 0;
	led_on();
	if(bt_data == 'n'){
		func_state = (func_state + 1) % 3;
		return;
	}
	mid_make_trigger();
	if(get_button(BUTTON7, BUTTON7PIN) == 1){ //버튼 누르면 종료여
		func_state = (func_state + 1) % 3;
		return;
	}
	right_make_trigger();
	if ((_motor_count - _compare_count) >= STANDARD_VALUE){
		_compare_count = _motor_count;
		if(ultrasonic_mid_distance >= 13){ // 전방 거리가 13이상이다.
			motor_state = 0;   // forward

			}else{ // 전방거리가 13보다 낮다. 좌우로 틀어서 전방거리 확보해야됨.
			if(ultrasonic_left_distance < ultrasonic_right_distance){
				motor_state = 1;   // right turn
				}else{
				motor_state = 2;   // left turn
			}
		}
		if(ultrasonic_right_distance <= 13){ // 오른쪽이 13보다 낮다
			motor_state = 2;   // left turn
		}
		if(ultrasonic_left_distance <= 13){ // 왼쪽이 13보다 낮다.
			motor_state = 1;   // right turn
		}
	}
	
	
	switch(motor_state){
		case 0:
		motor_forward();
		prev_motor_state = 0;
		break;
		case 1:
		motor_right_turn();
		motor_speed(1000, 1000);
		if(prev_motor_state != 1){
			trun_right_count++;
			prev_motor_state = 1;
		}
		break;
		case 2:
		motor_left_turn();
		motor_speed(1000, 1000);
		if(prev_motor_state != 2){
			trun_left_count++;
			prev_motor_state = 2;
		}
		break;
	}
}

void auto_mode_check(void){
	int first = 1;
	switch(bt_data){
		case 'n':
		func_state = (func_state + 1) % 3;
		break;
	}
	fnd_auto_check_display();
	if (first){
		motor_stop();
		motor_speed(0, 0);
		first = 0;
	}
}