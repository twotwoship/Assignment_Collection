/*
 * pizo.c
 *
 * Created: 2026-06-25 오후 2:57:29
 *  Author: kccistc
 */ 
#include "pizo.h"
#include "button.h"
#include "Speaker.h"

extern const int LG_Washing_Ending[];
extern const int LG_Washing_Ending_Beats[];
extern volatile int turn_point;
extern volatile int func_state;

int power_on_melody_play = 0;
int open_buzzer_play = 0;



volatile int func_state = 0;

void (*pizofunc[])() = {
	stand_by, power_on_melody, open_buzzer, drum_waching_stop_melody
};

void stand_by(void){
	OCR3A = 0;
}

void power_on_melody(void){
	power_on_melody_play = 1;
	if(turn_point < 71){
		OCR3A = 1000;
	}else if(turn_point < 141){
		OCR3A = 500;
	}else if(turn_point < 211){
		OCR3A = 333;
	}else if(turn_point < 281){
		OCR3A = 250;
	}else if(turn_point < 3001){
		OCR3A = 0;
	}
}
// 아니지 어차피 OCR3A 이놈 하나만 통제하는데 여러 함수를 쓸 필요가 없다고 생각한다. 분기점을 줘버리면 된다.
void open_buzzer(void){
	open_buzzer_play = 1;
	if(turn_point < 71){
		OCR3A = 3831;
	}else if(turn_point < 141){
		OCR3A = 3039;
	}else if(turn_point < 211){
		OCR3A = 2551;
	}else if(turn_point < 281){
		OCR3A = 1805;
	}else if(turn_point < 3001){
		OCR3A = 0;
	}
}

void drum_waching_stop_melody(void){
	
	Music_Player(LG_Washing_Ending, LG_Washing_Ending_Beats);
	
}
