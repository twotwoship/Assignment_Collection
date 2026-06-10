/*
 * 01_LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:30
 * Author : kccistc
 */ 
#define F_CPU 16000000UL	// 16MHz Because int type 2Byte in atmega128, long type
#include <avr/io.h>			// PORTA PORTD for I/O register
#include <util/delay.h>
#include "button.h"
#include "led.h"

extern void init_button(void);
extern void init_led(void);	//init_led는 다른 파일에 들어 있다고 compiler에게 알려줌.
extern int get_button(int button_num, int button_pin);
extern void led_all_on(void);
extern void led_all_off(void);
//extern void led_right_on(void);
//extern void led_left_on(void);
extern int reset_button(void);
int button0_state = 1; // 초기 상태를 led all off

int main(void){
	
	
	init_button();
	init_led();
	
	while(1){
		reset_button();
		// toggle off <----> on
		if(get_button(BUTTON0, BUTTON0PIN)){
			button0_state = (button0_state) % 4;
			if(button0_state == 1){
				button0_state++;
				led_all_on();
			}else if(button0_state == 2){
				button0_state++;
				led_right_on();
			}else if(button0_state == 3){
				button0_state++;
				led_left_on();
			}else{
				button0_state++;
				led_all_off();
			}
		}
	}
}

#if 0
int main(void)
{
    /* Replace with your application code */
	//DDRA = 0b11111111;	// Since there are 8 LEDs connected to PORTA, set it to ALL 1 (output)
	*(volatile uint8_t *)0x3A = 0xFF;
	
    while (1) 
	{
		//PORTA = 0b11111111;
		//PORTA = 0xff;	// 0 1 off on setting
		*(volatile uint8_t *)0x3B = 0xFF;
		_delay_ms(3000);
		//PORTA = 0b00000000;
		//PORTA = 0x00;
		*(volatile uint8_t *)0x3B = 0x00;
		_delay_ms(1000);
    }
}
#endif