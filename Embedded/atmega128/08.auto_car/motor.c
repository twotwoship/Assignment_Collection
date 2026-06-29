/*
 * motor.c
 *
 * Created: 2026-06-19 오후 9:16:38
 *  Author: kccistc
 */ 
#include "motor.h"


/*
1. 왼쪽 바퀴
PORTF0 -- IN1 ( DC motor driver )
PORTF1 -- IN2 (

2. 오른쪽 바퀴
PORTF2 -- IN3 ( DC motor driver )
PORTF3 -- IN4 (

IN1/IN3			IN2/IN4
=======			=======
0				1	: 역회전
1				0	: 정회전
1				0	: 정	지
*/
void init_motor(void){
	MOTOR_CONTROL_SETTING &= ~(1 << MOTOR_LEFT_CONTROL | 1 << MOTOR_RIGHT_CONTROL);
	MOTOR_CONTROL_SETTING |= (1 << MOTOR_LEFT_CONTROL | 1 << MOTOR_RIGHT_CONTROL);
	MOTOR_DIRVER_IN_SET &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND);
	MOTOR_DIRVER_IN_SET |= (1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND);
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND);
	
}

void motor_test(void){

}

void motor_speed(int left, int right){
	MOTOR_LEFT_SPEED = left;
	MOTOR_RIGHT_SPEED = right;
	
}

void motor_forward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (1 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 전진
	
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_backward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND); // 후진
	
		OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_left_forward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_right_forward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (1 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_left_backward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_right_backward(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_left_turn(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}
void motor_right_turn(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (1 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}

void motor_stop(void){
	MOTOR_FORWARD_BACKWARD &= ~(1 << MOTOR_LEFT_VCC | 1 << MOTOR_LEFT_GND | 1 << MOTOR_RIGHT_VCC | 1 << MOTOR_RIGHT_GND); // 초기화
	MOTOR_FORWARD_BACKWARD |= (0 << MOTOR_LEFT_VCC | 0 << MOTOR_LEFT_GND | 0 << MOTOR_RIGHT_VCC | 0 << MOTOR_RIGHT_GND);
	OCR1A=OCR1B; // OCR1A = left pwm, OCR1B = right pwm. 주기안에 전압 시간을 얼마나 줄지. 정하는 것

}