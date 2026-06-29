/*
 * DHT11.c
 *
 * Created: 2026-06-26 오전 9:28:22
 *  Author: kccistc
 */ 
#include "DHT11.h"

#define DHT11_DDR	DDRG
#define DHt11_PORT	PORTG
#define DHT11_PIN	PING
#define DHT11_INPUT_PIN	0

enum t_state{ OK, STARTUP_TIMEOUT, DATA_TIMEOUT, CHECKSUM_ERROR};

void dht11_main(void){
	uint8_t bytes[6];		// 온도 센서가 보내주는 데이터 저장 장소 40bit 보내줌
	uint8_t state = 0;		// 상태를 저장하는 변수
	int us_counter = 0;		// 시간 재는 것.
	
	// 1. 변수 초기화
	state = OK;
	memset(bytes, 0, sizeof(bytes));	// 공간 초기화 지원해주는 함수(해당공간, 값, 크기)
	
	//****************************************************************
	//						start up signal scenario 
	//****************************************************************
	// 1. start up signal  전송
	//---1.1 reset DHT11
	DHT11_DDR |= 1 << DHT11_INPUT_PIN;	//output mode
	DHt11_PORT |= 1 << DHT11_INPUT_PIN;	// high
	_delay_ms(100);
	
	//--- 1.2 low 최소 18ms
	DHt11_PORT &= ~(1 << DHT11_INPUT_PIN);	// low
	_delay_ms(20);		//spec상으로 최소 18ms 유지
	
	DHt11_PORT |= 1 << DHT11_INPUT_PIN;	// pull-up
	_delay_us(30);		//spec상으로 20~40us 유지, 
		//위와 같은 과정을 거치면 mcu(출력모드)에서 dht11을 호출한다는 의미임
		
	//2. start signal 응답 check
	DHT11_DDR &= ~(1 << DHT11_INPUT_PIN);	//input mode 전환
	// 2.1 DHT11이 low로 응답하는지 check (최대 100us까지 기다려 주겠다)
	us_counter = 0;
	while((DHT11_PIN & ( 1 << DHT11_INPUT_PIN ))){ // pin g의 0번이 하이일때
		_delay_us(1);
		if(++us_counter > 100){
			state = STARTUP_TIMEOUT;
			break;
		}
	}
	// 2.2 low구간 유지 확인 ( 80us )
	if(state == OK){
		us_counter = 0;
		while( !(DHT11_PIN & ( 1 << DHT11_INPUT_PIN )) ){ // pin g의 0번이 로우일때
			_delay_us(1);
			if(++us_counter > 100){
				state = STARTUP_TIMEOUT;
				break;
			}
		}		
	}
	// 2.3 high구간 유지 확인 ( 80us )
	if(state == OK){	// 해당 이후는 start data transmission
		us_counter = 0;
		while( (DHT11_PIN & ( 1 << DHT11_INPUT_PIN )) ){ // pin g의 0번이 하이일때
			_delay_us(1);
			if(++us_counter > 100){
				state = STARTUP_TIMEOUT;
				break;
			}
		}		
	}
	//****************************************************************
	//						DATA READ PART
	//****************************************************************
	//3.1 data 40bits read task
	if(state == OK){ // 에러 났는지 안났는지 확인하는 절차임.
		for(int i = 0; i < 5; i++){
			uint8_t one_byte = 0;
			for(int j = 0; j < 8; j++){
				
				// 1. low 구간 50us check
				us_counter = 0;
				while( !(DHT11_PIN & ( 1 << DHT11_INPUT_PIN )) ){ // pin g의 0번이 로우일때
					_delay_us(1);
					if(++us_counter > 100){
						state = DATA_TIMEOUT;
						break;
					}
				}
					
				// 2. high 구간 길이 check(spec : 0인 경우에는 26 ~ 28us 1인 경우에는 70us)
				//	논리구성 30us 이내면 0 이상이면 1로 판단.
				us_counter = 0;
				while( (DHT11_PIN & ( 1 << DHT11_INPUT_PIN )) ){ // pin g의 0번이 high일때
					_delay_us(1);
					if(++us_counter > 100){
						state = DATA_TIMEOUT;
						break;
					}
				}
				//	논리구성 30us 이내면 0 이상이면 1로 판단.
				if(us_counter > 30){
					one_byte |= 1 << (7-j);
				}
			}
			bytes[i] = one_byte;	//완성되면 집어 넣기.
		}
	}
	//3.2 checksum 확인
	uint8_t checksum;
	if(state == OK){
		checksum = bytes[0] + bytes[1] + bytes[2] + bytes[3];
		if(bytes[4] != checksum){
			state = CHECKSUM_ERROR;
		}
	}

	switch(state){
		case OK:
			printf("humi : %d.%d\n", bytes[0], bytes[1]);
			printf("temp : %d.%d\n", bytes[2], bytes[3]);
			break;
		case STARTUP_TIMEOUT:
			printf("STARTUP_TIMEOUT !!!!!!\n");
			break;
		case DATA_TIMEOUT:
		printf("DATA_TIMEOUT !!!!!!\n");
			break;
		case CHECKSUM_ERROR:
			printf("CHECKSUM_ERROR bytes[4] : %0x  checksum : %0x !!!!!!\n", bytes[4], checksum);
			break;
	}
}