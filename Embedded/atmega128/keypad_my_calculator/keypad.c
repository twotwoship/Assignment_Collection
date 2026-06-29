/*
 * keypad.c
 *
 * Created: 2026-06-29 오후 2:11:28
 *  Author: kccistc
 */ 

#include "keypad.h"

void init_keypad(void){
	KEYPAD_DDR = 0x0f;		// row : 입력 col: 출력
	KEYPAD_PORT = 0xff;		// 
}

uint8_t keypad_scan(void){
	uint8_t data = 0;	// key 값을 저장 변수로
	
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			data = get_button(row,col);
			if(data){
			return data;
			}
		}
	}
}

uint8_t get_button(int row, int col){
	
	uint8_t keypad_char[4][4] = {
		{' ', '0', '=', '+'	},
		{'4', '5', '6', '*'	},
		{'7', '8', '9', '-'	},
		{'1', '2', '3', '/'	}
	};
	//active low 로 동작.
	static int8_t prev_state[4][4] = {
		{ 1, 1, 1, 1 },		//초기 버튼은 눌려지지 않은 상태로 한다.
		{ 1, 1, 1, 1 },		
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	};
	
	int8_t current_state = 1;
	
	KEYPAD_PORT = 0xff;
	KEYPAD_PORT &= ~(1 << 3 - col);	// 해당 col에 전류를 흘린다.
	
	// 약간의 딜레이를 준다
	for(int delay = 0; delay < 20; delay++); // keypad check를 위한 delay
	
	current_state = (KEYPAD_PIN & ( 1 << (row+4) )) >> (row + 4);
	// 예) row 0 ; pa4에 연결
	// 76543210
	// 11101111 KEYPAD_PIN & ( 1 << (row+4) )
	// 00001110 >> (row+4)
	if(current_state == 0 && prev_state[row][col] == 1){ // 처음 눌려진 상태
		prev_state[row][col] = 0;
		return 0;
		
	}else if(current_state == 1 && prev_state[row][col] == 0 ){
		// 이전에 버튼이 눌려지고 현재는 버튼을 뗀 상태이면 버튼을 1번 눌렀다 뗀것으로 인정
		prev_state[row][col] = 1;	// prev_state를 초기화
		//printf("key: %c, row : %d, col : %d\n", keypad_char[row][col], row, col);
		return keypad_char[row][col];
		
	}
	
	return 0;
}