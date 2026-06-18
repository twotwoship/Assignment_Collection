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
#include "FND.h"
#include "uart0.h"

#define mode_stand 0
#define mode_washing 1
#define mode_rinse 2
#define mode_dehydration 3
#define mode_set_wash 4
#define mode_set_rinse 5
#define mode_set_dehydration 6


volatile int washing_time = 0;		// 세탁 시간 - 세탁 설정(standby_mode == 1) 안에서 botton2를 누르면 시간이 1초씩 추가
volatile int rinse_time = 0;			// 헹굼 시간 - 헹굼 설정(standby_mode == 2) 안에서 botton2를 누르면 시간이 1초씩 추가
volatile int dehydration_time= 0;	// 탈수 시간 - 탈수 설정(standby_mode == 3) 안에서 botton2를 누르면 시간이 1초씩 추가
volatile int standby_mode = mode_stand;		// 대기 모드 - int가 들어오기전까지 한무 대기.
volatile int washing_mode = 0;		// 세탁 모드 - 시간이 0이 되면 헹굼모드로 전환
volatile int rinse_mode = 0;			// 헹굼 모드 - 시간이 0이 되면 탈수모드로 전환
volatile int dehydration_mode = 0;	// 탈수 모드 = 시간이 0이 되면 대기모드로 전환
volatile int setting_mode = 0;		// 설정 모드 - 대기 모드에서 botton1을 누르면 세탁-헹굼-대기 순으로 돌아감.


extern volatile int washing_check;
extern volatile int rinse_check;
extern volatile int dehydration_check;

char _r[] = "\r\n";

void (*washing_command_queue[]) () = {
	start_on,
	stop_on
};

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
 	PORTF &= ~(1 << 6 | 1 << 7);
	PORTF |= 1 << 7;
}

/*
PE5 : PWM control
PF6 : IN1 / 방향설정
PF7 : IN2
*/

void start_on(void){
	standby_mode = mode_washing;
	//PORTA=0xf1;
	//_delay_ms(1000);
};

void stop_on(void){
	standby_mode = mode_stand;
}
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
				OCR3C = 250;	//motor speed
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

void _washing_machine_standby_mode_main(void){//대기모드 무한으로돌아야됨.
	
#if 0
	while(standby_mode == mode_stand){
		if(get_button(BUTTON0, BUTTON0PIN)){
			_washing_mode_func();
		}else if(get_button(BUTTON1, BUTTON1PIN)){
			_washing_setting_mode_func();
		}else{	standby_mode = mode_stand;
		}
	}
}
#else
		while(1){	//switch로 상태 전이되게하기 btn1누르면 세탁모드로 btn 0누르면 세탁모드로 전환
			//washing_command();
			switch(standby_mode){	//세탁모드와 설정모드 두개만 있으면 됨.
				case(mode_stand) :
					standby_mode = mode_stand;
					washing_command();
					FND_DIGIT_PORT = 0x80;
					FND_DATA_PORT = 0xc0;
					led_shift_left_on();
					OCR3C = 0;
					if(get_button(BUTTON0, BUTTON0PIN)){	standby_mode = mode_washing;	}
					if(get_button(BUTTON1, BUTTON1PIN)){	standby_mode = mode_set_wash;	}
				break;

				case(mode_washing) :
					_washing_mode_func();
				break;

				case(mode_rinse) :
					_rinse_mode_func();
				break;

				case(mode_dehydration) :
					_dehydration_mode_func();
				break;

				case(mode_set_wash) :
					_washing_setting_mode_func();
				break;

				case(mode_set_rinse) :
					_rinse_setting_mode_func();
				break;

				case(mode_set_dehydration) :
					_dehydration_setting_mode_func();
				break;
			}
		}
#endif
}




// TCNT0 == 6은 1초가 지났다는 뜻.
void _washing_mode_func(void){
	char _buff[] = "washing_mode : ";
	int pre_time = 0;
	while(washing_time > 0){ // 1초가 지나야지 줄어들게 끔 설정해야됨.
		fnd_washing_machine(washing_time);
		if(pre_time != washing_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(washing_time);
			UART0_print_string(_r);
			pre_time = washing_time;
		}
		PORTA=0x01;
		OCR3C = 250;
		washing_command();
		if(standby_mode == mode_stand){	break; }
		if(get_button(BUTTON0, BUTTON0PIN)){	standby_mode = mode_stand; break;	}
		if(washing_check >= 1000){ washing_check = 0; washing_time--;	}
	}	if(washing_time <= 0){	standby_mode = mode_rinse; }
}

void _rinse_mode_func(void){
	char _buff[] = "rinse_mode : ";
	int pre_time = 0;
	while(rinse_time > 0){ // 1초가 지나야지 줄어들게 끔 설정해야됨.
		fnd_washing_machine(rinse_time);
		if(pre_time != rinse_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(rinse_time);
			UART0_print_string(_r);
			pre_time = rinse_time;
		}
		PORTA=0x02;
		OCR3C = 70;
				washing_command();
				if(standby_mode == mode_stand){	break; }
		if(get_button(BUTTON0, BUTTON0PIN)){	standby_mode = mode_stand; break;	}
		if(rinse_check >= 1000){ rinse_check = 0; rinse_time--; }
	}	if(rinse_time <= 0){standby_mode = mode_dehydration; }
}

void _dehydration_mode_func(void){
	char _buff[] = "dehydration_mode : ";
	int pre_time = 0;
	while(dehydration_time > 0){ // 1초가 지나야지 줄어들게 끔 설정해야됨.
		fnd_washing_machine(dehydration_time);
		if(pre_time != dehydration_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(dehydration_time);
			UART0_print_string(_r);
			pre_time = dehydration_time;
		}
		PORTA=0x04;
		OCR3C = 250;
		washing_command();
		if(standby_mode == mode_stand){	break; }
		if(get_button(BUTTON0, BUTTON0PIN)){	standby_mode = mode_stand;  break;		}
		if(dehydration_check >= 1000){ dehydration_check = 0; dehydration_time--;	}
	}	if(dehydration_time <= 0){standby_mode = mode_stand;}
}

void _washing_setting_mode_func(void){		// 세탁 시간 설정모드에서 bottom2를 누르면 세탁시간이 1초씩 늘어나게 하기.
	char _buff[] = "washing_setting_mode : ";
	int pre_time = 0;
	while(1){
		fnd_washing_machine(washing_time);
		PORTA=0x01;
		if(get_button(BUTTON2, BUTTON2PIN)){	washing_time += 60;	}
		if(get_button(BUTTON1, BUTTON1PIN)){	break;	}
		if(pre_time != washing_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(washing_time);
			UART0_print_string(_r);
			pre_time = washing_time;
		}		
	}	standby_mode = mode_set_rinse;
}

void _rinse_setting_mode_func(void){
	char _buff[] = "rinse_setting_mode : ";
	int pre_time = 0;
	while(1){
		fnd_washing_machine(rinse_time);

		PORTA=0x02;
		if(get_button(BUTTON2, BUTTON2PIN)){	rinse_time += 60;	}
		if(get_button(BUTTON1, BUTTON1PIN)){	break;	}
		if(pre_time != rinse_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(rinse_time);
			UART0_print_string(_r);
			pre_time = rinse_time;
		}
	}	standby_mode = mode_set_dehydration;
}

void _dehydration_setting_mode_func(void){
	char _buff[] = "dehydration_setting_mode : ";
	int pre_time = 0;
	while(1){
		fnd_washing_machine(dehydration_time);

		PORTA=0x04;
		if(get_button(BUTTON2, BUTTON2PIN)){	dehydration_time += 60;	}
		if(get_button(BUTTON1, BUTTON1PIN)){	standby_mode = mode_stand; break;	}
		if(pre_time != dehydration_time){
			UART0_print_string(_buff);
			UART0_print_1_byte_number(dehydration_time);
			UART0_print_string(_r);
			pre_time = dehydration_time;
		}
	}	
}