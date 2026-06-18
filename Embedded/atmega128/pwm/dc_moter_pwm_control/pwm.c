/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:02
 *  Author: kccistc
 */ 
/*
초기화 해줄 것
버튼, 모터 드라이버, 타이머
16bit 3번 timer / counter를 사용 3번타이머로는 3개의 출력이 나간다.
PWM 출력 신호
============
PE3 : OC3A
PE4 : OC3B = interrupt 4 using ultrasonic senser
PE5 : OC3C = DC MOTOR (PWM output)
Button 0 : reset
Button 1 : speed-up
Button 2 : speed-down
Button 3 : right - reverse spin
*/
#include "pwm.h"

void init_timer3_pwm(void){
	// PE4 ultrasonic
	DDRE |= 1 << 3 | 1 << 5;
	// mode 5 : 8bit fast PWM mode using.
	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;
	// 비반전 모드 top : 0x00ff 비교일치 값 (PWM) 지정 OCR3C p360 grape15-7
	TCCR3A |= 1 << COM3C1;
	
	// -- 분주비 설정 -- 64
	// 16000000hz / 64 = 250000hz (250KHz)
	// T = 1/f 1/250000 --> 0.000004sec 4us
	// 250000Hz에서 256개의 펄스를 count 하면 걸리는 시간은 1.02ms
	// 그 중절반인 127개를 하면	= 0.5ms 정도
	// TCNT : 0~255(0x00ff)까지 count를 한 후 0으로 다시 돌아감.
	TCCR3B |= 1 << CS31 | 1 << CS30;	// 분주비 64
	OCR3C = 0;	// OCR(output compare regishter) : PWM 값 총 8비트의 표시(256)중 어느정도 전압을 뽑아줄것이냐.
	// OCR3C : 50인 경우 Duty (HIGH)가 멏 개 필요함?
	// Duty Cycle : (OCR3C / TOP) * 100 = 50 / 255 * 100 = 19.61%
 	
}

/*
PE5 : PWM control
PF6 : IN1 / 방향설정
PF7 : IN2
*/
void init_motor_diver(void){
	DDRF |= 1 << 6 | 1 <<7;	//출력 모드 설정
	PORTF &= ~(1 << 6 | 1 <<7);	// 6 7 reset
	PORTF |= (1 << 6 | 1 <<7); //
}

void dc_motor_pwm_control_main(void){
	int start_button = 0;
	int forward = 0;
	int backward = 0;
	while(1){
		if(get_button(BUTTON0, BUTTON0PIN)){ // start stop
			start_button = !start_button;
			if(start_button){
				OCR3C = 250;
			}else{
				OCR3C = 0;
			}
			
		}else if(get_button(BUTTON1, BUTTON1PIN)){ // speed-up
			if(OCR3C >= 250){
				OCR3C = 250;
			}else{
				OCR3C += 20;
			}
			
		}else if(get_button(BUTTON2, BUTTON2PIN)){ // speed-down
			if(OCR3C <= 70){
				OCR3C = 70;
				}else{
				OCR3C -= 20;
			}
			
		}else if(get_button(BUTTON3, BUTTON3PIN)){ // direction setting
			forward = !forward;
			PORTF &= ~(1 << 6 | 1 << 7);
			if(forward){
				PORTF |= 1 << 6;
			}else{
				PORTF |= 1 << 7;
			}
			
		}
	}
}