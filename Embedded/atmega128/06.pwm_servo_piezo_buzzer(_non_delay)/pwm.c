/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:02
 *  Author: kccistc
 */ 
#include "pwm.h"
/*
초기화 해줄 것
버튼, 모터 드라이버, 타이머
16bit 1번 timer / counter를 사용 
PWM 출력 신호
============ 
PE5 : OCR1A // 서브모터 통제
		
*/

void init_timer1_pwm(void){ // 50hz 짜리 20ms 계산하기 위해서 만드는거 
	// 분주비 8을 하는 이유는 16mhz를 가지고 50hz를 바로 만들지 못해 하는거
	// mode 14 : 16bit fast PWM mode using. 348page
	TCCR1A |= 1 << WGM11;	//icr1을 top으로 설정
	TCCR1B |= 1 << WGM12;
	TCCR1B |= 1 << WGM13;
	
	// 비반전 모드 top : ICR1 비교일치 값 (PWM) 지정 OCR1A OCR1B p350 grape15-7
	// 비교 일치 발생시 OCR1A 의 출력핀은 low로 바뀌고 bottom에서 high로 바뀐다.
	TCCR1A |= 1 << COM1A1;
	
	// -- 분주비 설정 -- 8
	// 16000000hz / 8 = 2000000hz (2MHz)
	// T = 1/f 1/16000000 --> 0.0000000625sec x 8 = 0.0000005sec / 0.5us
	// 16bit count의 최대값 65535    tcnt1의 최대값
	// 0.0000005 * 65535 = 0.0327675sec // 32.7675msec 마다 timer int 발생.
	// 20ms 길이(duty)를 갖는 사이클을 만들어야한다.
	// 0.0000005sec * 40000 = 0.02sec = 20msec
	// 250000Hz에서 256개의 펄스를 count 하면 걸리는 시간은 1.02ms
	// 그 중절반인 127개를 하면	= 0.5ms 정도
	//			0X3fff(1023) --> 4ms
	// TCNT : 0~255(0x00ff)까지 count를 한 후 0으로 다시 돌아감.
	TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS10);	// 분주비 리셋
	TCCR1B |= 1 << CS11;	// 분주비 8
	
	ICR1 = 40000 - 1; // 0.0000005sec * 40000 = 0.02sec = 20msec ---> top값
}

int servo_motor_main(void){
	//servo motor가 연결된 PORTB 5를 출력으로 설정
	
	DDRB |= 1 << 5;
	init_timer1_pwm();
	
	// 0(1ms) --> 90(1.5ms) --> 180(2ms)
	while(1){
		// 1ms는 pulse가 2000개여야됨.
		// 4000(20ms) / 20 --> 1ms(2000)
		OCR1A=1200; // 장비의 오차로인한 값 조절
		_delay_ms(300);
		
		// 1.5ms는 pulse가 3000개여야됨.
		// 3000(15ms)
		OCR1A=3200; // 장비의 오차로 인한 값 조절
		_delay_ms(300);
		
		// 2ms는 pulse가 4000개여야됨.
		// 4000(20ms)
		OCR1A=8000; // 장비의 오차로 인한 값 조절
		_delay_ms(300);
	}
	
	return 0;
}