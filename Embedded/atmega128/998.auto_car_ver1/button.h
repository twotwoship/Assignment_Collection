/*
 * button.h
 *
 * Created: 2026-06-19 오후 8:58:56
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define F_CPU 16000000UL  //16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.

#define  BUTTON_DDR  DDRG
#define  BUTTON_PIN  PING

#define  BUTTON7PIN 4    // PORT

#define  BUTTON7 0   // PORTD.4의 가상 index (sw번호)

#define  BUTTON_PRESS  1      // 버튼을 누르면 high (active-high)
#define  BUTTON_RELEASE 0     // 버튼을 뗀 상태 (low)

#define  BUTTON_NUMBER  1   // 버튼 갯수

void init_button(void);
int get_button(int button_num, int button_pin);

extern volatile uint16_t debounce_count;


#endif /* BUTTON_H_ */