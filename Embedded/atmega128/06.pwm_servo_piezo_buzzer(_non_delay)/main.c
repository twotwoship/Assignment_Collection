/*
 * 08.pwm_servo_piezo_buzzer.c
 *
 * Created: 2026-06-25 오전 9:46:13
 * Author : kccistc
 */ 

#include <avr/io.h>
#include "pwm.h"
#include "pizo.h"
#include "Speaker.h"

extern void Music_Player(int *tone, int *Beats);
extern void init_speaker(void);
extern void Beep(int  repeat);
extern void Siren(int repeat);
extern void RRR(void);

extern const int Elise_Tune[];
extern const int Elise_Beats[];

extern const int LG_Washing_Ending[];
extern const int LG_Washing_Ending_Beats[];
extern volatile int func_state;


volatile int debounce_count = 0;
volatile int turn_point = 0;

// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.

ISR(TIMER0_OVF_vect){		//1ms timer
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	debounce_count++;
	turn_point++;
	if(turn_point > 10000){
		turn_point = 0;
	}
}

int main(void)
{
//	servo_motor_main();
	init_speaker();
	init_button();
	init_timer0();
	sei();
    while (1) 
    {
		pizofunc[func_state]();
		if(get_button(BUTTON0, BUTTON0PIN)){
			if(power_on_melody_play == 1){
				func_state = 0;
				power_on_melody_play = 0;
			}else{
				func_state = 1;
				turn_point = 0;	
			}
			
		}
		if(get_button(BUTTON1, BUTTON1PIN)){
			if(open_buzzer_play == 1){
				func_state = 0;
				open_buzzer_play = 0;
			}else{
				func_state = 2;
				turn_point = 0;	
			}
			

		}
		if(get_button(BUTTON3, BUTTON3PIN)){
			if(drum_waching_stop_melody_play == 1){
				func_state = 0;
				drum_waching_stop_melody_play = 0;
			}else{
				func_state = 3;
				turn_point = 0;
			}
		}
		//OCR3A=1702;   // 삐~뽀 레 
		/*
		OCR3A=1702;   // 삐~뽀 레 
		_delay_ms(1000);
		OCR3A=1431;   // 파  
		_delay_ms(1000);
	
*/
//		Beep(5);
		// RRR();
// 		_delay_ms(3000);
// 		Siren(5);
// 		_delay_ms(3000);
//		Music_Player(Elise_Tune, Elise_Beats);	
    }
}

void init_timer0(void){
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0; // TIMER0 OVERFLOW INT 활성화
	
}
