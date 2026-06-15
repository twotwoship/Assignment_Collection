/*
 * FND.c
 *
 * Created: 2026-06-12 오전 10:52:28
 *  Author: kccistc
 */ 

#include "FND.h"
#include "button.h"
#define fnd_select 1	// 0 : anode, 1 : cathode

extern uint32_t ms_count;	//ms를 재는 count; uin32_t = unsigned int와 동이 4바이트
extern uint32_t sec_count;	//sec를 재는 count; uin32_t = unsigned int와 동이 4바이트
extern uint32_t turn_select;
extern uint8_t dot_display;
extern int run_select;
extern volatile uint16_t msec_count; // 이친구로 시간 카운트 하면 된다.


void fnd_main(void){// 메인문 잘 생각해보자.
	fnd_display(sec_count, dot_display); 
	//_delay_ms(1); 1ms를 기다린다.
	//1ms를 흐르기전에는 멈춰야됨.어케 멈추지?
	//1ms가 지날때까지 기다려야됨여기서
	//여기 진입한 순간 msec_count를 체크하고 1차이가 날때가지 계속 순회?
	static uint16_t msec_check = 0;
	msec_count = 0;
	msec_check = 0;
	msec_check = msec_count;
	while(msec_check == msec_count){} // 1ms 지나서 탈출
	ms_count++;
	if(ms_count >= 1000){
		ms_count = 0;
		sec_count++;
		dot_display = !dot_display;
	}
}

void stop_timer(void){
}

void init_fnd(void){
	FND_DATA_DDR = 0xff;	// 출력모드 설정.
	// 다 열었음.
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;	// 출력모드 설정.
	
	//FND_ALL_OFF
#if fnd_select	// common anode
	FND_DATA_PORT = 0xff;
#else	// common cathode
	FND_DATA_PORT = (uint8_t)~0xff;
#endif
}

void fnd_display(uint32_t sec_count, uint8_t dot_display){
	static int digit_select = 0;	// 자리수 선택

#if fnd_select
							//0		1	2	3		4	5		6	7		8		9	.
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};	
#else
							//0		1	2		3		4	5		6		7		8		9	.
	uint8_t fnd_font[] = {(uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};	
#endif

	
	switch(digit_select){
		case 0:	//10^0
#if fnd_select
		FND_DIGIT_PORT = 0x80;
#else
		FND_DIGIT_PORT = (uint8_t)~0x80;
#endif
		FND_DATA_PORT = fnd_font[sec_count % 10];
		break;
		case 1:	//10^1
#if fnd_select
		FND_DIGIT_PORT = 0x40;
#else
		FND_DIGIT_PORT = (uint8_t)~0x40;
#endif
		FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
		break;
		case 2:	//10^2
#if fnd_select
		FND_DIGIT_PORT = 0x20;
#else
		FND_DIGIT_PORT = (uint8_t)~0x20;
#endif
		if(dot_display == 1){
			FND_DATA_PORT = 0x7f & fnd_font[sec_count / 60 % 10]; // min
			//FND_DATA_PORT = 0x7f; 가 점이 맞는데
			// 어케 점찍지?? dp는 7번에 연결되어있다.
		}else{
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10]; // min	

		}
		
		break;
		case 3:	//10^3
#if fnd_select
		FND_DIGIT_PORT = 0x10;
#else
		FND_DIGIT_PORT = (uint8_t)~0x10;
#endif
		FND_DATA_PORT = fnd_font[sec_count / 600 % 6];
		break;
	}
	digit_select = (digit_select + 1) % 4;
}

void fnd_second(void){ // 초시계 왼쪽 2개는 시계돌리기, 오른쪽 2개는 초시계
	init_fnd();
	fnd_display_second(sec_count);
	static uint16_t msec_check = 0;
	msec_count = 0;
	msec_check = 0;
	msec_check = msec_count;
	while(msec_check == msec_count){} // 1ms 지나서 탈출
	//_delay_ms(1);
	ms_count++;
	if(ms_count >= 1000){
		ms_count = 0;
		sec_count++;
		turn_select++;
	}
		
}

void fnd_display_second(uint32_t sec_count){
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
		#if fnd_select
		FND_DIGIT_PORT = 0x80;
		#else
		FND_DIGIT_PORT = (uint8_t)~0x80;
		#endif
		FND_DATA_PORT = fnd_font[sec_count % 10];
		break;
		case 1:	//10^1
		#if fnd_select
		FND_DIGIT_PORT = 0x40;
		#else
		FND_DIGIT_PORT = (uint8_t)~0x40;
		#endif
		FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
		break;
		case 2:	
		#if fnd_select
		FND_DIGIT_PORT = 0x20;
		#else
		FND_DIGIT_PORT = (uint8_t)~0x40;
		#endif
		FND_DATA_PORT = turn_table_font[temp % 4];
		break;
		case 3:	
		#if fnd_select
		FND_DIGIT_PORT = 0x10;
		#else
		FND_DIGIT_PORT = (uint8_t)~0x40;
		#endif
		FND_DATA_PORT = turn_table_font[temp % 4];
		break;
	}
	digit_select = (digit_select + 1) % 4;
}

void fnd_stopwatch_(void){
	static uint16_t msec_check = 0;
	init_fnd();
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};
	if(run_select == 1){
		fnd_third(sec_count, ms_count);
			msec_count = 0;
			msec_check = 0;
			while(msec_check == msec_count){} // 1ms 지나서 탈출
			//_delay_ms(1);
		ms_count++;
		if(ms_count >= 1000){
			ms_count = 0;
			sec_count++;
			turn_select++;
		}	
	}else{
			FND_DIGIT_PORT = 0x80;
			FND_DATA_PORT = fnd_font[ms_count % 100];
			msec_count = 0;
			msec_check = 0;
			while(msec_check == msec_count){} // 1ms 지나서 탈출
			//_delay_ms(1);
			FND_DIGIT_PORT = 0x40;
			FND_DATA_PORT = fnd_font[ms_count % 10];
			msec_count = 0;
			msec_check = 0;
			while(msec_check == msec_count){} // 1ms 지나서 탈출
			//_delay_ms(1);
			FND_DIGIT_PORT = 0x20;
			FND_DATA_PORT = 0x7f & fnd_font[sec_count % 10];
			msec_count = 0;
			msec_check = 0;
			while(msec_check == msec_count){} // 1ms 지나서 탈출
			//_delay_ms(1);
			FND_DIGIT_PORT = 0x10;
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
			msec_count = 0;
			msec_check = 0;
			while(msec_check == msec_count){} // 1ms 지나서 탈출
			//_delay_ms(1);
	}
	
}

void fnd_third(uint32_t sec_count, uint32_t ms_count){ // 스탑와치돌아가게하기
	static int digit_select = 0;	// 자리수 선택

#if fnd_select
							//0		1	2	3		4	5		6	7		8		9	.
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};	
#else
							//0		1	2		3		4	5		6		7		8		9	.
	uint8_t fnd_font[] = {(uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};	
#endif

	
	switch(digit_select){
		case 0:	//10^0
#if fnd_select
		FND_DIGIT_PORT = 0x80;
#else
		FND_DIGIT_PORT = (uint8_t)~0x80;
#endif
		FND_DATA_PORT = fnd_font[ms_count % 100];
		break;
		case 1:	//10^1
#if fnd_select
		FND_DIGIT_PORT = 0x40;
#else
		FND_DIGIT_PORT = (uint8_t)~0x40;
#endif
		FND_DATA_PORT = fnd_font[ms_count % 10];
		break;
		case 2:	//10^2
#if fnd_select
		FND_DIGIT_PORT = 0x20;
#else
		FND_DIGIT_PORT = (uint8_t)~0x20;
#endif
		FND_DATA_PORT = 0x7f & fnd_font[sec_count % 10];
		break;
		case 3:	//10^3
#if fnd_select
		FND_DIGIT_PORT = 0x10;
#else
		FND_DIGIT_PORT = (uint8_t)~0x10;
#endif
		FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
		break;
	}
	digit_select = (digit_select + 1) % 4;

}