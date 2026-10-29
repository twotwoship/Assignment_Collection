/*
 * uart1.c
 *
 * Created: 2026-06-22 오전 10:38:28
 *  Author: kccistc
 */ 
#include "uart1.h"
#include "led1.h"
#include <stdio.h>

extern void UARTO_transmit(unsigned char data);


void init_uart1(void);
void UART1_transmit(unsigned char data);

volatile uint8_t bt_data;

// page 278 표 12-3
//bt로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.
//ex) led_all_on\n 이면 11번 이곳으로 진입 한다.
ISR(USART1_RX_vect){
	
	bt_data = UDR1;	//UDR1의 내용은 data에 copy가 아닌 move임. 즉 사라진다는 소리.
	UARTO_transmit(bt_data); // bt로 부터 들여온 데이터를 uart0으로 내보냄. comport 출력용
	
}
/*
1. 전송속도를 초기화 9600bps
2. start / stop bit 설정
3. RX(수신) : interrupt 로 설정 TX(송신) : polling
*/
void init_uart1(void){
	//1. 전송속도 : 9600bps
	UBRR1H = 0x00;
	UBRR1L = 207; // 9600bps /p 219
	UCSR1A |= 1 << U2X1; // 2배속 설정
	
	//2. UART0 를 송신하고 수신이 다 가능하고 RX interrupt가 가능하도록 설정.
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

// UART1로 1byte를 전송 하는 함수
void UART1_transmit(unsigned char data){
	while(!(UCSR1A & 1 << UDRE1)){ //data가 송신중이면 송신이 끝날때까지 기다림
		;	// no operation
	}
	UDR1 = data; // hw 전송 register에 data를 집어 넣는다.
}