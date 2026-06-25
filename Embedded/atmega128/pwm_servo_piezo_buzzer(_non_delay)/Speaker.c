/*
 * Speaker.c
 *
 * Created: 2026-06-25 오후 1:44:18
 *  Author: kccistc
 */ 

#include "Speaker.h"


int drum_waching_stop_melody_play = 0;
extern volatile int func_state;

extern volatile int turn_point;

int School_Bell_Tune[] = {SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, SO_01, MI_01, MI_01, RE_01, 
					 	  SO_01, SO_01, LA_01, LA_01, SO_01, SO_01, MI_01, 
					 	  SO_01, MI_01, RE_01, MI_01, DO_01,'/0'};
const int School_Bell_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1,
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_2, 
						   BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1_4, BEAT_1};
						   
// const int i = 0;
// i =20; 안되 ..... 읽는 작업만 가능하고 write는 안되  
// int j;
// j = i;


/*int He_Pirate[] = {RE_04, RE_04, RE_04, RE_04,
	RE_04, RE_04, RE_04, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	DO_04, RE_04, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, MI_04,
	FA_04, FA_04, FA_04, SO_04,
	MI_04, MI_04, RE_04, DO_04,
	RE_04, 0, 0, LA_03, DO_04,
	RE_04, RE_04, RE_04, FA_04,
	SO_04, SO_04, SO_04, LA_04,
	LA_04s, LA_04s, LA_04, SO_04,
	
	LA_04, RE_04, 0, RE_04, MI_04,
	FA_04, FA_04, SO_04,
	LA_04, RE_04, 0, RE_04, FA_04,
	MI_04, MI_04, FA_04, RE_04,
	MI_04, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	DO_05, RE_05, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, MI_05,
	FA_05, FA_05, FA_05, SO_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, 0, 0, LA_04, DO_05,
	RE_05, RE_05, RE_05, FA_05,
	SO_05, SO_05, SO_05, LA_05,
	LA_05s, LA_05s, LA_05, SO_05,
	LA_05, RE_05, 0, RE_05, MI_05,
	FA_05, FA_05, SO_05,
	LA_05, RE_05, 0, RE_05, FA_05,
	MI_05, MI_05, RE_05, DO_05,
	RE_05, RE_05, MI_05,
	
	FA_05, FA_05, FA_05, SO_05,
	LA_05, FA_05, 0, 0, FA_05, RE_05,
	LA_04, 0, 0, 0,
	LA_04s, 0, 0, SO_05, RE_05,
	LA_04s, 0, 0, 0,
	MI_05, MI_05, RE_05,
	FA_04, 0, FA_04, SO_04,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04,0 , 0,
	SO_04, SO_04, SO_04,
	SO_04, LA_04, 0, 0,
	LA_04, LA_04, LA_04,
	LA_04s, LA_04, 0, 0,
	SO_04, FA_04, MI_04,
	RE_04, 0, 0, RE_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, FA_04, MI_04,
	FA_04, SO_04, LA_04,
	SO_04, 0, 0, FA_04, SO_04,
	LA_04, 0, 0, SO_04, FA_04,
	MI_04, FA_04, MI_04,
	RE_04, 0, 0, MI_04, DO_04,
	RE_04, 0, 0, RE_05, MI_05,
	
	FA_05, 0, 0, MI_05, FA_05,
	SO_05, FA_05, SO_05,
	LA_05, SO_05, FA_05,
	RE_05, 0, 0, RE_05, MI_05,
	FA_05, SO_05, LA_05,
	LA_05s, RE_05, SO_05,
	FA_05, 0, 0, SO_05, MI_05,
	RE_05, 0, 0, MI_05, DO_05s,
	LA_05, 0, 0, LA_05s, 0, 0,
	LA_05, LA_05, LA_05,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, RE_05, MI_05, FA_05,
	LA_05, RE_05, MI_05, FA_05,
	LA_05s, RE_05, MI_05, FA_05,
	LA_05, LA_05, DO_06,
	LA_05, SO_05, 0, 0,
	SO_05, 0, 0,
	FA_05, 0, 0,
	FA_05, SO_05, MI_05,
	RE_05, 0, 0,
RE_04, '/0'};
int He_Pirate1_Beat[] = {BEAT_1_4, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_16, BEAT_1_16, BEAT_1_16,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_4, BEAT_1_2,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_2, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_4,
	BEAT_1_2, BEAT_1_8, BEAT_1_4,
BEAT_1};*/

const int LG_Washing_Ending[] = {
	DO_02_H, FA_02_H, MI_02_H, RE_02_H, DO_02_H, LA_01_H,
	SO_01, DO_02_H, RE_02_H, SO_01, LA_01_H, TI_01, LA_01_H, DO_02_H,
	DO_02_H, FA_02_H, MI_02_H, RE_02_H, DO_02_H, FA_02_H,
	FA_02_H, SO_02_H, FA_02_H, MI_02_H, RE_02_H, MI_02_H, FA_02_H, '/0'
};
	
const int LG_Washing_Ending_Beats[] = {
	BEAT_1_4, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4, BEAT_1_4, 
	BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4, BEAT_1_4, 
	BEAT_1_4, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4, BEAT_1_4,
	BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_4_3, BEAT_1_2, '/0'
	};

const int Elise_Tune[] = {MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01,
	MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, DO_02, TI_01, LA_01, 0,
	TI_01, DO_02, RE_02, MI_02, 0, SO_01, FA_02, MI_02, RE_02, 0, FA_01, MI_02, RE_02, DO_02, 0,
	MI_01, RE_02, DO_02, TI_01, 0, MI_01, MI_02, MI_03,
	RE_02_H, MI_02, RE_02_H, MI_02, RE_02, MI_02, RE_01_H, MI_02, TI_01, RE_01, DO_02, LA_01, 0,
	DO_01, MI_01, LA_01, TI_01, 0, MI_01, SO_01_H, TI_01, DO_02, 0,
	MI_01, MI_02, RE_02_H, MI_02, RE_02_H, MI_02, TI_01, RE_02, DO_02, LA_01, 0, DO_01, MI_01, LA_01, TI_01, 0,
RE_01, DO_02, TI_01, LA_01,'/0'};

const int Elise_Beats[] = 
   { BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
	BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8,
BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8};

int GrandFa_Tune[] = {SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
	DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02, DO_02, TI_01, LA_01, SO_01, DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02,
SO_01, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, '/0'};

const int GrandFa_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
	BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
BEAT_1_4, BEAT_1_2, BEAT_1_2, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2};


void Music_Player(int *tone, int *Beats)
{	
	drum_waching_stop_melody_play = 1;
	static int beats = 0;
	static int in_tone = 0;
	static int in_Beats = 0;
	if(tone[in_tone] != '/0'){
		if(beats == 0){
			beats = Beats[in_Beats];
			turn_point = 0;
		}
		if(turn_point < beats){
			OCR3A = tone[in_tone];
		}else{
			in_tone++;
			in_Beats++;	
			beats = 0;	
		}
	}else{
		beats = 0;
		in_tone = 0;
		in_Beats = 0;
		drum_waching_stop_melody_play = 0;
		//func_state = 0;
		return;
	}
	/*
	while(*tone != '/0')
	{
		OCR3A = *tone;
		delay_ms(*Beats);
		tone++;
		Beats++;
		OCR3A = 0;
		_delay_ms(10);		
	}
	return;
	*/
}  
// Timer3 위상교정 PWM
void init_speaker(void)
{
	DDRE |= 0x08;   // PWM CHANNEL  OC3A(PE3) 출력 모드로 설정 한다. 
	TCCR3A = (1<<COM3A0); // COM3A0 : 비교일치시 PE3 출력 반전 (P328 표14-6 참고) // 핵심 원리 출력이 자동으로 반전되서 isr을 따로 써주지 않아도됨.
	TCCR3B = (1<<WGM32) | (1<<CS31);   // WGM32 : CTC 4(P327 표14-5) CS31: 8분주(P318)
	// CTC mode : 비교일치가 되면 카운터는 reset되면서 PWM 파형 출력 핀의 출력이 반전 됨. 
	// 정상모드와 CTC모드의 차이점은 비교일치 발생시 TCNT1의 레지스터값을 0으로 설정 하는지 여부 이다. 
	// 정상모드를 사용시 TCNT1이 자동으로 0으로 설정 되지 않아 인터럽트 루틴에서 TCNT1을 0으로 설정 해 주었다. 
	// 위상교정 PWM mode4  CTC 분주비 8  16000000/8 ==> 2,000,000HZ(2000KHZ) : 
	// up-dounting: 비교일치시 LOW, down-counting시 HIGH출력
	// 1/2000000 ==> 0.0000005sec (0.5us)
	// P599 TOP 값 계산 참고
	// PWM주파수 = OSC(16M) / ( 2(up.down)x N(분주율)x(1+TOP) ) 
	// TOP =  (fOSC(16M) /  2(up.down)x N(분주율)x 원하는주파수 )) -1 
	//-----------------------------------------------------------
	// - BOTTOM :  카운터가 0x00/0x0000 일때를 가리킨다.
    // - MAX : 카운터가 0xFF/0xFFFF 일 때를 가리킨다.
    // - TOP?:  카운터가 가질 수 있는 최대값을 가리킨다. 오버플로우 인터럽트의 경우 TOP은 0xFF/0xFFFF
    //          이지만 비교일치 인터럽트의 경우 사용자가 설정한 값이 된다. 
    
	TCCR3C = 0;  // P328 그림 14-11 참고 
	OCR3A = 0;   // 비교 일치값을 OCR3A에 넣는다. 
	
	return;
}

void Beep(int repeat)
{
	int  i;
	
	for(i=0; i < repeat; i++)
	{
		OCR3A = 500;  // 0.00025sec (250us) : 0.0000005 * 500
		_delay_ms(200);
		OCR3A = 0;
		_delay_ms(200);
	}
}    

void Siren(int repeat)
{
	int i, j;
	
	OCR3A = 900;
	
	for(i=0; i < repeat; i++)
	{
		for(j=0; j < 100; j++)
		{
			OCR3A += 10;
			_delay_ms(20);
		}
		for(j=0; j < 100; j++)
		{
			OCR3A -= 10;
			_delay_ms(20);
		}
	}
}

void RRR(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		OCR3A = 1136;
		_delay_ms(100);
		OCR3A = 0;
		_delay_ms(20);
	}
}

void delay_ms(int ms)
{
	while(ms-- != 0)_delay_ms(1);
}