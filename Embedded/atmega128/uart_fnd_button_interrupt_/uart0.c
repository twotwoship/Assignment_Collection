/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:57:53
 *  Author: kccistc
 */ 

#include "uart0.h"

void init_uart(void);
void UARTO_transmit(unsigned char data);

/*
1. 전송속도를 초기화 9600bps
2. start / stop bit 설정
3. RX(수신) : interrupt 로 설정 TX(송신) : polling
*/
void init_uart(void){
	//1. 전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; // 9600bps /p 219
	UCSR0A |= 1 << U2X0; // 2배속 설정
	
	//2. UART0 를 송신하고 수신이 다 가능하고 RX interrupt가 가능하도록 설정.
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

// UARTO로 1byte를 전송 하는 함수
void UARTO_transmit(unsigned char data){
	while(!(UCSR0A & 1 << UDRE0)){ //data가 송신중이면 송신이 끝날때까지 기다림
		;	// no operation
	}
	UDR0 = data; // hw 전송 register에 data를 집어 넣는다.
}

//짝수 페러티 ,홀수 페러티,

void UART0_print_string(char *str){ // 문자열 송신용 하나씩 찢어서 보내기
	for(int i = 0; str[i]; i++){
		UARTO_transmit(str[i]);
	}
}

void UART0_print_1_byte_number(unsigned char n){ // 1바이트짜리 숫자 송신용
	char numstring[4] = "0";
	int i , index = 0;
	
	if(n>0){
		for(i = 0; n != 0; i++){
			numstring[i] = n % 10 + '0';
			n = n/10;
		}
		numstring[i] = '\0';
		index = i - 1;
	}
	for(i = index; i >= 0; i--){
		UARTO_transmit(numstring[i]);
	}
}