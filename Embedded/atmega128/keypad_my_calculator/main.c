/*
 * 11. keypad
 *
 * Created: 2026-06-18 오후 2:04:08
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#define MAX 100
#include <stdio.h>
#include <stdlib.h>

#include "uart0.h"
#include "keypad.h"
#include "queue.h"
#include "cal.h"

volatile uint32_t keypad_count = 0;
volatile uint8_t circul_input_data = 0;
volatile int buff_count = 0;
volatile int answer_check = 0;
/*

*/

char* inbuff  = 0;
			
//파일을 맵핑해줘야됨. p232
FILE OUTPUT = FDEV_SETUP_STREAM(UARTO_transmit, NULL, _FDEV_SETUP_WRITE);

/*
ISR interrupt service routine : 인터럽트 처리 함수 isr로 시작
TIMER0_OVF_vect : timer0 overflow int가 발생이 되면 이곳으로 진입한다.
250개의 펄스를 count(1ms)하면 이곳으로 자동 진입 한다.
ISR은 가능한 짧게 작성한다.
*/
ISR(TIMER0_OVF_vect){		//4ms timer
	volatile uint8_t keydata = 0;
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count 
	if(++keypad_count >= 60){
		keypad_count = 0;
		if(keydata = keypad_scan()){// keypad를 check 해서 눌려 진 것이 있으면 저장함.
			// 스택에 저장해야됨
			inbuff[buff_count] = keydata;	// 하나씩 집어 넣어.
			if(keydata == '='){	// = 나오면 답 출력하게 하기.
				answer_check = 1;
			}
			printf("%c",keydata); // 키 입력받은거 하나씩 출력하게 
			buff_count++;
		}	
	}
}

int main(void){
	
	inbuff = (char*)calloc(MAX, sizeof(char)); // 중위 계산식 입력 버퍼
	char* temp_buff = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 출력버퍼
	int* number_stack = (int*)calloc(MAX, sizeof(int)); // 후위 계산 할 때 쓰는 스택 // 이 스택용 pop push도 만들어야됨.
	char* operater_stack = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 스택		if(queue_empty() != TRUE){
		
	uint8_t key_value;
	init_timer0();
	init_uart();
	init_keypad();
	queue_init();

	stdout = &OUTPUT;	// printf 동작 할 수 있도록 stdout을 설정.
	sei();	// 전역(대문) interrrupt 허용
	
	/* 계산기 참고
	int main(void) {
		char* inbuff = (char*)calloc(MAX, sizeof(char)); // 중위 계산식 입력 버퍼
		char* temp_buff = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 출력버퍼
		int* number_stack = (int*)calloc(MAX, sizeof(int)); // 후위 계산 할 때 쓰는 스택 // 이 스택용 pop push도 만들어야됨.
		char* operater_stack = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 스택
		printf("input : ");
		fgets(inbuff, 100, stdin);

		back_display_make(inbuff, temp_buff, operater_stack); // 후위 계산식 만드는 함수

		if (ocb != ccb) {
			printf("invalid_input\n");
			return 0;
		}

		back_display_carculate(temp_buff, number_stack); // 후위 계산 함수
		printf("answer = %d\n", pop_i(number_stack));
		return 0;
	}
	*/

    while (1){
		// 인터럽트로 누르면 무적권 큐에 들어간다. 어차피 읽기만 하면됨.
		// 큐에 들어간 것을 스택으로 다시 넣어줘야됨.
		// 자 생각해보자 한번에 전부 집어 넣느게 아니라 하나하나 버퍼에 차근차근 집어넣어야됨 한번에 절대 안들어감.
		
		if(answer_check){
			back_display_make(inbuff, temp_buff, operater_stack); // 후위 계산식 만드는 함수
			// inbuff 에있던 걸 temp_buff로 후위계산식으로 변경
			
			if (ocb != ccb) {	printf("invalid_input\n");	}
				
			back_display_carculate(temp_buff, number_stack); // 후위 계산 함수
			printf("%d\n", pop_i(number_stack));
			answer_check--;
			buff_count = 0;
			for(int i = 0; i < MAX; i++){
				inbuff[i] = '\n';
			}
		}
	}
}
/*
1. timer 0을 초기화 한다.
	AVR에서 8bit timer 0번, 2번 중에서 0번을 초기화 한다.
	임베디드에서 가장 신경을 써야 할 부분을 초기화 하는 것이다.
	초기화가 잘못되면 이후가 다 꼬인다.
2. 8bit 가지고 1ms를 측정 하는 timer/counter를 만들고자 한다.
2-1. 분주비 설정(1/64)
	16000000hz / 64 = 250,000hz
2-2. 1주기가 잡아먹는 시간 계산
	t = 1/f = 1/250,000 = 0.004ms
2-3. 8bit 가지고 count 하는 시간을 계산 ( 8bit timer ov(over flow)
	0.004ms * 256개  = 1.024ms
	0.004ms * 250개  = 1.000ms, 0.001sec
*/
void init_timer0(void){
	TCNT0 = 6;	// TCNT0 6~256 : 250개 pulse count 
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0; // TIMER0 OVERFLOW INT 활성화	
}
