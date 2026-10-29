/*
 * fnd.c
 *
 * Created: 2026-06-19 오후 7:47:04
 *  Author: kccistc
 */ 

#include "FND.h"
#include "ultrasonic.h"
#include "interrupt.h"
#include "menu.h"
#include <util/delay.h>  // _delay_ms _delay_us 등

extern volatile int ultrasonic_left_tige_count;
extern volatile uint16_t _us_count;

extern trun_left_count;
extern trun_right_count;
extern int run_record_count;
extern int motor_state;

uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; // 0부터 9 마지막은 점

/*
FND_@@_DIGIT_PORT = 01 02 04 08 로 제어할 것. 자리별 포트 열고 닫기. 왼쪽부터 오른쪽 순임.
*/


void init_fnd(void){
	FND_DATA_DDR = 0xff;	// 출력모드 설정.
	FND_DATA_PORT = 0xff;	// all off
		
	FND_UP_DIGIT_DDR |= 1 << FND_UP_DIGIT_D1 | 1 << FND_UP_DIGIT_D2 | 1 << FND_UP_DIGIT_D3 | 1 << FND_UP_DIGIT_D4;	// 출력모드 설정.
	FND_DOWN_DIGIT_DDR |= 1 << FND_DOWN_DIGIT_D1  | 1 << FND_DOWN_DIGIT_D2 | 1 << FND_DOWN_DIGIT_D3 | 1 << FND_DOWN_DIGIT_D4;	// 출력모드 설정.

}
		
void fnd_test(void){
	FND_DATA_PORT = 0x80;
	FND_UP_DIGIT_PORT |= 0x80; // 아래거
	FND_DOWN_DIGIT_PORT |= 0x08; // 위에거
	
}

void fnd_passvie_display(void){
	static int digit = 0;

	FND_UP_DIGIT_PORT = 0x00;
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0x00;

	switch (digit){
		case 0:
		FND_DATA_PORT = ~0x50;
		FND_UP_DIGIT_PORT = 0x10;
		break;

		case 1:
		FND_DATA_PORT = ~0x1c;
		FND_UP_DIGIT_PORT = 0x40;
		break;

		case 2:
		FND_DATA_PORT = ~0x54;
		FND_UP_DIGIT_PORT = 0x20;
		break;

		case 3:
		FND_DATA_PORT = 0xc0;
		FND_UP_DIGIT_PORT = 0x80;
		break;

		case 4:
		FND_DATA_PORT = 0xc0;
		FND_DOWN_DIGIT_PORT = 0x01;
		break;

		case 5:
		FND_DATA_PORT = 0xc0;
		FND_DOWN_DIGIT_PORT = 0x02;
		break;

		case 6:
		FND_DATA_PORT = 0xc0;
		FND_DOWN_DIGIT_PORT = 0x04;
		break;

		case 7:
		FND_DATA_PORT = 0xc0;
		FND_DOWN_DIGIT_PORT = 0x08;
		break;
	}
	digit = (digit + 1) % 8;
}

void fnd_go(void){
	static int down_digit_select = 0;	// 자리수 선택
	switch(down_digit_select){
		case 0:
		FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0xfe; // 10분단위
		FND_DOWN_DIGIT_PORT = 0x01;
		break;
		case 1:
		FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT =  ~0xfe; // 1분단위
		FND_DOWN_DIGIT_PORT = 0x7f;
		break;
		case 2:
		FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0xfe; // 10초단위
		FND_DOWN_DIGIT_PORT = 0x04;
		break;
		case 3:
		FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0xfe; // 1초단위
		FND_DOWN_DIGIT_PORT = 0x7f;
		break;
	}
	down_digit_select = (down_digit_select + 1) % 4;
}
void fnd_running(void){
	static int digit = 0;

	FND_UP_DIGIT_PORT = 0x00;
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0xff;

	switch (digit){
		case 0:
		if (motor_state == 0) FND_DATA_PORT = ~0x36;
		else if (motor_state == 1) FND_DATA_PORT = ~0x31;
		else if (motor_state == 2) FND_DATA_PORT = ~0x07;
		FND_UP_DIGIT_PORT = 0x10;
		break;

		case 1:
		if (motor_state == 0) FND_DATA_PORT = ~0x36;
		else if (motor_state == 1) FND_DATA_PORT = ~0x31;
		else if (motor_state == 2) FND_DATA_PORT = ~0x07;
		FND_UP_DIGIT_PORT = 0x40;
		break;

		case 2:
		if (motor_state == 0) FND_DATA_PORT = ~0x36;
		else if (motor_state == 1) FND_DATA_PORT = ~0x31;
		else if (motor_state == 2) FND_DATA_PORT = ~0x07;
		FND_UP_DIGIT_PORT = 0x20;
		break;

		case 3:
		if (motor_state == 0) FND_DATA_PORT = ~0x36;
		else if (motor_state == 1) FND_DATA_PORT = ~0x31;
		else if (motor_state == 2) FND_DATA_PORT = ~0x07;
		FND_UP_DIGIT_PORT = 0x80;
		break;

		case 4:
		FND_DATA_PORT = fnd_font[(run_record_count / 600) % 6];
		FND_DOWN_DIGIT_PORT = 0x01;
		break;

		case 5:
		FND_DATA_PORT = 0x7f & fnd_font[(run_record_count / 60) % 10];
		FND_DOWN_DIGIT_PORT = 0x02;
		break;

		case 6:
		FND_DATA_PORT = fnd_font[(run_record_count / 10) % 6];
		FND_DOWN_DIGIT_PORT = 0x04;
		break;

		case 7:
		FND_DATA_PORT = fnd_font[run_record_count % 10];
		FND_DOWN_DIGIT_PORT = 0x08;
		break;
	}
	digit = (digit + 1) % 8;
}
void fnd_running_2(void){
	static int up_digit_select = 0;	// 자리수 선택
	static int down_digit_select = 0;	// 자리수 선택
		FND_UP_DIGIT_PORT = 0x00;
		FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = 0xff;
	if(motor_state == 0){ // 전진
		switch(up_digit_select){
		case 0:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0x36; // 10분단위
		FND_UP_DIGIT_PORT = 0x10;
		break;
		case 1:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT =  ~0x36; // 1분단위
		FND_UP_DIGIT_PORT = 0x40;
		break;
		case 2:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0x36; // 10초단위
		FND_UP_DIGIT_PORT = 0x20;
		break;
		case 3:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = ~0x36; // 1초단위
		FND_UP_DIGIT_PORT = 0x80;
		break;		
		}
	}
	if(motor_state == 1){ // 우
		switch(up_digit_select){
			case 0:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x31; // 10분단위
			FND_UP_DIGIT_PORT = 0x10;
			break;
			case 1:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x31; // 1분단위
			FND_UP_DIGIT_PORT = 0x40;
			break;
			case 2:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x31; // 10초단위
			FND_UP_DIGIT_PORT = 0x20;
			break;
			case 3:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x31; // 1초단위
			FND_UP_DIGIT_PORT = 0x80;
			break;
		}		
	}
	if(motor_state == 2){ // 좌
		switch(up_digit_select){
			case 0:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x07; // 10분단위
			FND_UP_DIGIT_PORT = 0x10;
			break;
			case 1:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x07; // 1분단위
			FND_UP_DIGIT_PORT = 0x40;
			break;
			case 2:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x07; // 10초단위
			FND_UP_DIGIT_PORT = 0x20;
			break;
			case 3:
			FND_UP_DIGIT_PORT = 0x00;
			FND_DATA_PORT = ~0x07; // 1초단위
			FND_UP_DIGIT_PORT = 0x80;
			break;
		}
	}
	up_digit_select = (up_digit_select + 1) % 4;
	FND_UP_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0xff;
	
			switch(down_digit_select){ // 시간 나오게 하기
				case 0:
				FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(run_record_count / 600) % 6];
				FND_DOWN_DIGIT_PORT = 0x01;
				break;
				case 1:
				FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = 0x7f & fnd_font[(run_record_count / 60) % 10];
				FND_DOWN_DIGIT_PORT = 0x02;
				break;
				case 2:
				FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(run_record_count / 10) % 6];
				FND_DOWN_DIGIT_PORT = 0x04;
				break;
				case 3:
				FND_DOWN_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[run_record_count % 10];
				FND_DOWN_DIGIT_PORT = 0x08;
				break;
			}
	down_digit_select = (down_digit_select + 1) % 4;
}

void fnd_auto_check_display(void){
	static int digit = 0;

	FND_UP_DIGIT_PORT = 0x00;
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0x00;

	switch (digit){
		case 0:
		FND_DATA_PORT = fnd_font[(run_record_count / 600) % 6];
		FND_UP_DIGIT_PORT = 0x10;
		break;

		case 1:
		FND_DATA_PORT = 0x7f & fnd_font[(run_record_count / 60) % 10];
		FND_UP_DIGIT_PORT = 0x40;
		break;

		case 2:
		FND_DATA_PORT = fnd_font[(run_record_count / 10) % 6];
		FND_UP_DIGIT_PORT = 0x20;
		break;

		case 3:
		FND_DATA_PORT = fnd_font[run_record_count % 10];
		FND_UP_DIGIT_PORT = 0x80;
		break;

		case 4:
		FND_DATA_PORT = fnd_font[(trun_left_count % 100) / 10];
		FND_DOWN_DIGIT_PORT = 0x01;
		break;

		case 5:
		FND_DATA_PORT = fnd_font[trun_left_count % 10];
		FND_DOWN_DIGIT_PORT = 0x02;
		break;

		case 6:
		FND_DATA_PORT = fnd_font[(trun_right_count % 100) / 10];
		FND_DOWN_DIGIT_PORT = 0x04;
		break;

		case 7:
		FND_DATA_PORT = fnd_font[trun_right_count % 10];
		FND_DOWN_DIGIT_PORT = 0x08;
		break;
	}
	digit = (digit + 1) % 8;
}

void fnd_run_record(void){
	static int up_digit_select = 0;	// 자리수 선택
	// 1sec 마다 올라가니까 1개씩

	switch(up_digit_select){
		case 0:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(run_record_count / 600) % 6]; // 10분단위
		FND_UP_DIGIT_PORT = 0x10;
		break;
		case 1:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT =  0x7f &  fnd_font[(run_record_count / 60) % 10]; // 1분단위
		FND_UP_DIGIT_PORT = 0x40;
		break;
		case 2:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(run_record_count / 10 % 6)]; // 10초단위
		FND_UP_DIGIT_PORT = 0x20;
		break;
		case 3:
		FND_UP_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(run_record_count % 10)]; // 1초단위
		FND_UP_DIGIT_PORT = 0x80;
		break;
	}
	up_digit_select = (up_digit_select + 1) % 4;
}

void fnd_count(void){
	static int down_digit_select = 0;	// 자리수 선택

	switch(down_digit_select){
		case 0:
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(trun_left_count % 100) / 10];
	FND_DOWN_DIGIT_PORT = 0x01;
		break;
		case 1:
			FND_DOWN_DIGIT_PORT = 0x00;
			FND_DATA_PORT = fnd_font[(trun_left_count % 10)];
			FND_DOWN_DIGIT_PORT = 0x02;
		break;
		case 2:
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(trun_right_count % 100) / 10];
	FND_DOWN_DIGIT_PORT = 0x04;		
		break;
		case 3:
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(trun_right_count % 10)];
	FND_DOWN_DIGIT_PORT = 0x08;		
		break;
	}
	down_digit_select = (down_digit_select + 1) % 4;

}
void fnd_auto_test_2(void){
	static int digit_select = 0;

	FND_UP_DIGIT_PORT = 0x00;
	FND_DOWN_DIGIT_PORT = 0x00;

	switch(digit_select)
	{
		case 0:
		FND_DATA_PORT = fnd_font[(ultrasonic_left_distance % 100) / 10];
		FND_UP_DIGIT_PORT = 0x01;
		break;

		case 1:
		FND_DATA_PORT = fnd_font[ultrasonic_left_distance % 10];
		FND_UP_DIGIT_PORT = 0x02;
		break;

		case 2:
		FND_DATA_PORT = fnd_font[(ultrasonic_right_distance % 100) / 10];
		FND_UP_DIGIT_PORT = 0x04;
		break;

		case 3:
		FND_DATA_PORT = fnd_font[ultrasonic_right_distance % 10];
		FND_UP_DIGIT_PORT = 0x08;
		break;

		case 4:
		FND_DATA_PORT = fnd_font[(ultrasonic_mid_distance % 100) / 10];
		FND_DOWN_DIGIT_PORT = 0x10;
		break;

		case 5:
		FND_DATA_PORT = fnd_font[ultrasonic_mid_distance % 10];
		FND_DOWN_DIGIT_PORT = 0x20;
		break;
	}

	digit_select = (digit_select + 1) % 6;
}

void fnd_auto_test(void){
	static int up_digit_select = 0;	// 자리수 선택
	static int down_digit_select = 0;	// 자리수 선택
switch(up_digit_select){
	case 0:	// 제일 왼쪾 fnd. 10의자리
	FND_UP_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(ultrasonic_left_distance % 100) / 10];
	FND_UP_DIGIT_PORT = 0x01;
	break;
	case 1:	// 왼쪽에서 2번째 fnd. 1의자리
	FND_UP_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[ultrasonic_left_distance % 10];
	FND_UP_DIGIT_PORT = 0x02;
	break;
	case 2:
	FND_UP_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(ultrasonic_right_distance % 100) / 10];
	FND_UP_DIGIT_PORT = 0x04;
	break;
	case 3:
	FND_UP_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[ultrasonic_right_distance % 10];
	FND_UP_DIGIT_PORT = 0x08;
	break;
}
up_digit_select = (up_digit_select + 1) % 4;
switch(down_digit_select){
	case 0:	// 제일 왼쪾 fnd. 10의자리
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[(ultrasonic_mid_distance % 100) / 10];
	FND_DOWN_DIGIT_PORT = 0x10;
	break;
	case 1:	// 왼쪽에서 2번째 fnd. 1의자리
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = fnd_font[ultrasonic_mid_distance % 10];
	FND_DOWN_DIGIT_PORT = 0x20;
	break;
	case 2:
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0x08;
	FND_DOWN_DIGIT_PORT = 0x08;
	break;
	case 3:
	FND_DOWN_DIGIT_PORT = 0x00;
	FND_DATA_PORT = 0x08;
	FND_DOWN_DIGIT_PORT = 0x04;
	break;
}
down_digit_select = (down_digit_select + 1) % 4;
}

void fnd_ultrasonic_distance(int type){
	//가변적으로 들어오는 수에 대해서 1의자리 10의자리 100의자리 를 판단해야됨.
	
	
	static int up_digit_select = 0;	// 자리수 선택
	static int down_digit_select = 0;	// 자리수 선택
	static int table_select = 0;	// 자리수 선택
	switch(type){
		case LAFT_TYPE:
			switch(up_digit_select){
				case 0:	// 제일 왼쪾 fnd. 10의자리
					FND_DOWN_DIGIT_PORT = 0x00;
					FND_DATA_PORT = fnd_font[(ultrasonic_left_distance % 100) / 10];
					FND_DOWN_DIGIT_PORT = 0x01;	
				break;
				case 1:	// 왼쪽에서 2번째 fnd. 1의자리
					FND_DOWN_DIGIT_PORT = 0x00;
					FND_DATA_PORT = fnd_font[ultrasonic_left_distance % 10];
					FND_DOWN_DIGIT_PORT = 0x02;
				break;
				case 2:
					FND_DOWN_DIGIT_PORT = 0x00;
					FND_DATA_PORT = fnd_font[ultrasonic_left_distance % 10];
					FND_DOWN_DIGIT_PORT = 0x08;
				break;
				case 3:
					FND_DOWN_DIGIT_PORT = 0x00;
					FND_DATA_PORT = fnd_font[(ultrasonic_left_distance % 100) / 10];
					FND_DOWN_DIGIT_PORT = 0x04;
				break;
			}
				up_digit_select = (up_digit_select + 1) % 4;
		break;
		case MID_TYPE:
			switch(down_digit_select){
				case 0:	// 제일 왼쪾 fnd. 10의자리

				break;
				case 1:	// 왼쪽에서 2번째 fnd. 1의자리

				break;
				case 2:
				FND_UP_DIGIT_PORT = 0x00;
				FND_DATA_PORT = fnd_font[(ultrasonic_mid_distance % 100) / 10];
				FND_UP_DIGIT_PORT = 0x04;		
				break;
				case 3:
				FND_UP_DIGIT_PORT = 0x00;
				FND_DATA_PORT = fnd_font[ultrasonic_mid_distance % 10];
				FND_UP_DIGIT_PORT = 0x08;
				break;
			}
			down_digit_select = (down_digit_select + 1) % 4;
		break;
		case RIGHT_TYPE:
			switch(up_digit_select){
				case 0:	// 제일 왼쪾 fnd. 10의자리

				break;
				case 1:	// 왼쪽에서 2번째 fnd. 1의자리

				break;
				case 2:
				FND_UP_DIGIT_PORT = 0x00;
				FND_DATA_PORT = fnd_font[(ultrasonic_right_distance % 100) / 10];
				FND_UP_DIGIT_PORT = 0x04;
				break;
				case 3:
				FND_UP_DIGIT_PORT = 0x00;
				FND_DATA_PORT = fnd_font[ultrasonic_right_distance % 10];
				FND_UP_DIGIT_PORT = 0x08;
				break;
			}
			up_digit_select = (up_digit_select + 1) % 4;		break;
	}
}


/*
void fnd_display_second(uint32_t one_sec_count){
	uint8_t turn_table_font[] = {0xf7, 0xf9, 0xfe, 0xcf};
	#if fnd_select
	//0		1	2	3		4	5		6	7		8		9	.
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};
	#else
	//0		1	2		3		4	5		6		7		8		9	.
	uint8_t fnd_font[] = {(uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};

	#endif

	static int digit_select = 0;	// 자리수 선택
	static int table_select = 0;	// 자리수 선택
	int temp = turn_select % 4;
	switch(digit_select){
		case 0:	//10^0
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[one_sec_count % 10];
		FND_DIGIT_PORT = 0x80;
		break;
		
		case 1:	//10^1
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[one_sec_count / 10 % 6];
		FND_DIGIT_PORT = 0x40;
		break;
		
		case 2:
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = turn_table_font[temp % 4];
		FND_DIGIT_PORT = 0x20;
		break;
		
		case 3:
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = turn_table_font[temp % 4];
		FND_DIGIT_PORT = 0x10;
		break;
	}
	digit_select = (digit_select + 1) % 4;
}
*/