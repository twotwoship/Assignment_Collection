/*
 * FND.c
 *
 * Created: 2026-06-12 오전 10:52:28
 *  Author: kccistc
 */ 

#include "FND.h"
#include "button.h"
#define fnd_select 1	// 0 : anode, 1 : cathode

extern uint32_t ms_count;	
extern uint32_t sec_count;	
extern uint32_t one_ms_count;
extern uint32_t one_sec_count;
extern uint32_t stopwatch_ms_count;
extern uint32_t stopwatch_sec_count;
extern uint32_t turn_select;
extern uint8_t dot_display;
extern int run_select;

void fnd_main(void){
	fnd_display(sec_count, dot_display);
	/*
	_delay_ms(1);
		ms_count++;
		if(ms_count >= 1000){
			ms_count = 0;
			sec_count++;
		}
		*/
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
	//FND_DATA_PORT = 0x00;
	static int digit_select = 0;	// 자리수 선택

#if fnd_select
							//0		1	2	3		4	5		6	7		8		9	.
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};	
#else
							//0		1	2		3		4	5		6		7		8		9	.
	uint8_t fnd_font[] = {(uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};	
#endif

	
	switch(digit_select){
		case 0:	//10^0 1의 자리
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[sec_count % 10];
		FND_DIGIT_PORT = 0x80;
		break;
		
		case 1:	//10^1
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[(sec_count / 10) % 6];
		FND_DIGIT_PORT = 0x40;
		break;
		
		case 2:	//10^2
		FND_DIGIT_PORT = 0x00;
		if(dot_display == 1){
			FND_DATA_PORT = 0x7f & fnd_font[sec_count / 60 % 10]; // min
			FND_DIGIT_PORT = 0x20;
			//FND_DATA_PORT = 0x7f; 가 점이 맞는데
			// 어케 점찍지?? dp는 7번에 연결되어있다.
		}else{
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10]; // min	
			FND_DIGIT_PORT = 0x20;
		}
		break;
		
		case 3:	//10^3
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[sec_count / 600 % 6];
		FND_DIGIT_PORT = 0x10;
		break;
	}
	digit_select = (digit_select + 1) % 4;
}

void fnd_second(void){ // 초시계 왼쪽 2개는 시계돌리기, 오른쪽 2개는 초시계
		fnd_display_second(one_sec_count);
		/*
		_delay_ms(1);
		one_ms_count++;
		if(one_ms_count >= 1000){
			one_ms_count = 0;
			one_sec_count++;
			turn_select++;
		}
		*/
}

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

void fnd_stopwatch_(void){
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f};
	if(run_select == 1){ // 버튼 1이 눌렸을 때만 동작해야됨.
		fnd_third(stopwatch_sec_count, stopwatch_ms_count);
		/*
		_delay_ms(1);
		stopwatch_ms_count++;
		if(stopwatch_ms_count >= 1000){
			stopwatch_ms_count = 0;
			stopwatch_sec_count++;
		}
		*/
	}else{
		fnd_third(stopwatch_sec_count, stopwatch_ms_count);

	}
}

void fnd_third(uint32_t stopwatch_sec_count, uint32_t stopwatch_ms_count){ // 스탑와치돌아가게하기
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
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[stopwatch_ms_count /10 % 100];
		FND_DIGIT_PORT = 0x80;
		break;
		
		case 1:	//10^1
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[stopwatch_ms_count % 10];
		FND_DIGIT_PORT = 0x40;
		break;
		
		case 2:	//10^2
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = 0x7f & fnd_font[stopwatch_sec_count % 10];
		FND_DIGIT_PORT = 0x20;
		break;
		
		case 3:	//10^3
		FND_DIGIT_PORT = 0x00;
		FND_DATA_PORT = fnd_font[stopwatch_sec_count / 10 % 6];
		FND_DIGIT_PORT = 0x10;
		break;
	}
	digit_select = (digit_select + 1) % 4;

}