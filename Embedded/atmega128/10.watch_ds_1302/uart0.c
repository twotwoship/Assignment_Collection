/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:57:53
 *  Author: kccistc
 */ 

#include "uart0.h"

#include <stdio.h>

void init_uart(void);
void UARTO_transmit(unsigned char data);

volatile int front = 0;
volatile int rear = 0;


//pc로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.
//ex) led_all_on\n 이면 11번 이곳으로 진입 한다.
ISR(USART0_RX_vect){ // 크기가 디파인된 원형큐를 만들어서 동작한다
	
	volatile unsigned char data;
	volatile static int i = 0;
	data = UDR0;	//UDR0의 내용은 data에 copy가 아닌 move임. 즉 사라진다는 소리.
	
	UARTO_transmit(data);	// uart0 송신이 제대로 동작 하고 있는지 확인용,.
	 
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

int passing(int a, int b){
	int answer = 0;
	int pas_front = rx_buff[front][a] - '0';
	int pas_rear = rx_buff[front][b] - '0';
	answer = pas_front*10 + pas_rear;
	return answer;
}

void pc_command_processing(t_ds1302 *ds1302){
	if(front != rear){	// data가 ru_buff에 존재하는지 check

			ds1302->year = passing(6,7);
			ds1302->month = passing(8,9);
			ds1302->date = passing(10,11);
			ds1302->hours = passing(12,13);
			ds1302->minutes = passing(14,15);
			ds1302->second = passing(16,17);
			
			init_ds1302(ds1302);
			
			front = (front + 1) % QUEUE_SIZE;

	}
}

void pc_command_processing_2(t_ds1302 *ds1302){ //  받은 0xBF 을 ds1302에 보내기 그냥 저주소를 보내서 읽으면 된다.\
	//64비트를 어디다 담아서 가져올 것이냐. 그냥 64안에다 다 담아.
	uint64_t temp = 0;
	uint64_t *p_temp = &temp;
	if(front != rear){	// data가 ru_buff에 존재하는지 check
		
		if(strcmp(rx_buff[front], "0xBF") == 0){
			_bust_read_ds1302(0xBF, p_temp);
		}
		// 자 64bit 받아 왔어 어떻게 읽어야할까.
		// 개씩 짤라서 읽어야됨.
		uint8_t *data = (uint8_t *)&temp;
		ds1302->second = bcd2dec(data[0]);
		ds1302->minutes = bcd2dec(data[1]);
		ds1302->hours = bcd2dec(data[2]);
		ds1302->date = bcd2dec(data[3]);
		ds1302->month = bcd2dec(data[4]);
		ds1302->dayofweek = bcd2dec(data[5]);
		ds1302->year = bcd2dec(data[6]);
		ds1302->wp = bcd2dec(data[7]);
		printf("_bust_date : %d-%d-%d-%d | time : %d-%d-%d \n",
		ds1302->year, ds1302->month, ds1302->date,
		ds1302->dayofweek,
		ds1302->hours, ds1302->minutes, ds1302->second);
		
		front = (front + 1) % QUEUE_SIZE;

	}
}

		/*
			setrtcyymmddhhmmss

			예를 들어 setrtc260629101411 이란 정보가 넘어왔어 그러면
			하나의 문자열 배열로 넘어오는데 5번까지는 무시하고 6번부터
			67은 ds1302->year
			89는 ds1302->month
			10 11 은 ds1302->date
			12 13 은 ds1302->hours
			14 15 는 ds1302->minutes
			16 17 은 ds1302->second
			이렇게 받아와야됨
			6하고 7을 하나로 어떻게 합치지? 언사인드 차로 받아오는데 26이 들어왓어 그러면 2, 6이 들어온다
		*/