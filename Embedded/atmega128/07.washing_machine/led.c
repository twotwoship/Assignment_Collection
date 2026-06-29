/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 
//#define direct_access_port_A *(volatile uint8_t *)0x1B;
#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
int led_main(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keepon(void);
void led_shift_right_keepon(void);
void led_flower_on(void);
void led_flower_off(void);

#define test *(volatile unsigned char *)0x3B

extern volatile uint16_t msec_count;

#define FUNC_SU 6

int func_state = 0;

void (*fp[]) () = {
	led_shift_left_on,			// func_state = 0;	
	led_shift_right_on,			// func_state = 1;
	led_shift_left_keepon,		// func_state = 2;
	led_shift_right_keepon,		// func_state = 3;
	led_flower_on,				// func_state = 4;
	led_flower_off				// func_state = 5;
};

int led_main(void){
	
	uint8_t led_toggle = 0;
	led_all_off();
	
	while(1){
#if 1
		fp[func_state]();
#else
		if(msec_count >= 500){ // 500ms
			msec_count = 0;
			led_toggle = !led_toggle;
			if(led_toggle){
				led_all_on();
				}else{
				led_all_off();
			}
		}
#endif
	}
	return 0;
}

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

void led_shift_left_on(void){
#if 1
	static int i = 0;
	
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B = 1<< i;
		if( (i = (i + 1) % 8) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
	}
#endif
#if 0
	static int i = 0;
	*(unsigned char *) 0x3B = 1<< i;
	_delay_ms(30);
	i = (i + 1) % 8;	
#endif
}

void led_shift_right_on(void){
#if 1
static int i = 0;
if(msec_count >= 100){
	msec_count = 0;
	*(unsigned char *) 0x3B = 0x80 >> i;
		if( (i = (i + 1) % 8) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
}
#endif	
#if 0
	static int i = 0;
	*(unsigned char *) 0x3B = 0x80 >> i;
	_delay_ms(30);
	i = (i + 1) % 8;
#endif
}

void led_shift_left_keepon(void){
	static int i = 0;
	if(i == 0){
		led_all_off();
	}
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B |= 1 << i;
		if( (i = (i + 1) % 9) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
	}
}

void led_shift_right_keepon(void){
	static int i = 0;
		if(i == 0){
			led_all_off();
		}
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B |= 0x80 >> i;
		//*(unsigned char *) 0x3B |= 1 << (8 - i);
		if( (i = (i + 1) % 9) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
	}
}
/*
void led_shift_right_keepon(void){
	static int i = 0;
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B |= 1 << (7 - i);
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
		}
	}
}
*/
void led_flower_on(void){
	static int i = 0;
		if(i == 0){
			led_all_off();
		}
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B |= 0x18 | (0x18 << i | 0x18 >> i);
		if( (i = (i + 1) % 8) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
	}
}

void led_flower_off(void){
	static int i = 0;
		if(i == 0){
			led_all_off();
		}
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B |= 0x00 | (0x81 >> i | 0x81 << i);
		if( (i = (i + 1) % 8) == 0 );
		/*
		if( (i = (i + 1) % 8) == 0 ){ // 다음 index 값을 계산
			func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 jump
			led_all_off();
		}
		*/
	}
}