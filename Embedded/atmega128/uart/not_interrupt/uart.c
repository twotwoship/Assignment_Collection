/*
 * uart.c
 *
 * Created: 2026-06-13 오후 4:28:44
 *  Author: kccistc
 */ 
#include "uart.h"
// 모든 정의는 데이터 시트에 주소번지가 다 나와있고 iom128a.h에 친절하게 정의되어 있다.
void UART0_Init(void)
{
	// 상위 8비트 하위 8비트를 의미 전송속도의미 데이터시트 확인해라 비동기 1배속
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
	UBRR0L = (unsigned char)UBRR_VALUE;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // 디폴트로 송수신이 금지되어 있기 때문에 송수신비트를 1로 바꿔줘야함.

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 통신 데이터 형식 및 통신 방법을 결정하기 위한 것 2, 1
}				// 시트를 보면 알겠지만 둘다 1로 하면 데이터 비트는 8개라는걸 의미함.

void UART0_TxChar(char data) // transmit
{
	while (!(UCSR0A & (1 << UDRE0))){}; // 송신 여부 파악 송신버퍼 확인
		//usart control and status register
	UDR0 = data;
}

char UART0_RxChar(void) // receive
{
	while (!(UCSR0A & (1 << RXC0))); // 수신 여부 파악 수신완료

	return UDR0;	// usart data register 송수신된 데이터가 저장되는 버퍼 레지스터임.
}

void UART0_print_string(char *str){ // 문자열 송신용 하나씩 찢어서 보내기
	for(int i = 0; str[i]; i++){
		UART0_TxChar(str[i]);
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
		UART0_TxChar(numstring[i]);
	}
}