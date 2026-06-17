/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:57:53
 *  Author: kccistc
 */ 

#include "uart0.h"
#include "led.h"
#include <stdio.h>

extern int func_state;
extern void (*fp[])();

void init_uart(void);
void UARTO_transmit(unsigned char data);

volatile int front = 0;
volatile int rear = 0;


//pc로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.
//ex) led_all_on\n 이면 11번 이곳으로 진입 한다.
ISR(USART0_RX_vect){
	
	volatile unsigned char data;
	volatile static int i = 0;
	data = UDR0;	//UDR0의 내용은 data에 copy가 아닌 move임. 즉 사라진다는 소리.
	
	if(data == '\n' || data == '\r'){
		if((rear+1) % QUEUE_SIZE == front % QUEUE_SIZE){
			return;	//queue full states
		}
		rx_buff[rear][i] = '\0';	// 문장의 끝인 null 삽입
		i = 0;	// next string 저장을 위한 index 초기화.
		rear = (rear + 1) % QUEUE_SIZE; // 0~9
	}else{
		if((rear+1) % QUEUE_SIZE == front % QUEUE_SIZE){
			return;	//queue full states
		}
		rx_buff[rear][i++] = data;
	}
}
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

void pc_command_processing(void){
	if(front != rear){	// data가 ru_buff에 존재하는지 check
		printf("%s", rx_buff[front]);	//
		if(strncmp(rx_buff[front],"led_shift_left_on", strlen("led_shift_left_on")) == 0){
			func_state = 0;
			
		}else if(strncmp(rx_buff[front],"led_shift_right_on", strlen("led_shift_right_on")) == 0){
			func_state = 1;
			
		}else if(strncmp(rx_buff[front],"led_shift_left_keepon", strlen("led_shift_left_keepon")) == 0){
			func_state = 2;
		
		}else if(strncmp(rx_buff[front],"led_shift_right_keepon", strlen("led_shift_right_keepon")) == 0){
			func_state = 3;
	
		}else if(strncmp(rx_buff[front],"led_flower_on", strlen("led_flower_on")) == 0){
			func_state = 4;

		}else if(strncmp(rx_buff[front],"led_flower_off", strlen("led_flower_off")) == 0){
			func_state = 5;
		}
		front = (front + 1) % QUEUE_SIZE;
	}
	fp[func_state]();
}