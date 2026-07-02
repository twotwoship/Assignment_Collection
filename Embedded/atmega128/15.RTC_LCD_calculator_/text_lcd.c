/*
 * text_lcd.c
 *
 * Created: 2026-07-01 오전 11:14:45
 *  Author: kccistc
 */ 
#include "text_lcd.h"
#include "ds1307.h"
#include "uart0.h"
#include "timer0.h"


volatile uint8_t _first = 1;

// (*´
uint8_t face_left[8] = {
    0b01100,
    0b10000,
    0b10100,
    0b10010,
    0b10000,
    0b10000,
    0b01000,
    0b00110
};

// ∀
uint8_t face_mid[8] = {
    0b00000,
    0b10001,
    0b01010,
    0b01110,
    0b00100,
    0b00100,
    0b00000,
    0b00000
};

// ｀)
uint8_t face_right[8] = {
    0b00110,
    0b00001,
    0b01001,
    0b00100,
    0b00001,
    0b00001,
    0b00010,
    0b01100
};

// Y
uint8_t alp_Y[8] = {
    0b10001,
    0b11011,
    0b01010,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000
};

// M
uint8_t alp_M[8] = {
	0b10001,
    0b10001,
    0b11011,
    0b10101,
    0b10101,
    0b10001,
    0b10001,
    0b00000
};

// D
uint8_t alp_D[8] = {
	0b11110,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11110,
	0b00000
};

// : (중앙 정렬)
uint8_t colon[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b00000,
	0b00000,
	0b00100,
	0b00000,
	0b00000
};

// s
uint8_t alp_S[8] = {
	0b01110,
	0b10000,
	0b10000,
	0b01110,
	0b00001,
	0b00001,
	0b11110,
	0b00000
};

// [안 1-1] 고양이 왼쪽
uint8_t cat_left[8] = {
	0b00100, //     ■      (왼쪽 귀 끝)
	0b01100, //   ■■
	0b11111, // ■■■■■     (얼굴 왼쪽 테두리)
	0b10100, // ■   ■      (왼쪽 눈)
	0b10000, // ■
	0b11011, // ■■   ■■   (왼쪽 수염)
	0b01111, //   ■■■■     (턱 선)
	0b00000  //
};

// [안 1-2] 고양이 꼬리
uint8_t cat_tail[8] = {
	0b00111, //
	0b01000, //
	0b11000, // 
	0b01100, //
	0b00100, //
	0b01100, // 
	0b11000, // 
	0b00000  //
};

// [안 1-3] 고양이 오른쪽
uint8_t cat_right[8] = {
	0b00100, //     ■      (오른쪽 귀 끝)
	0b00110, //       ■■
	0b11111, // ■■■■■     (얼굴 오른쪽 테두리)
	0b00101, //       ■ ■  (오른쪽 눈)
	0b00001, //         ■
	0b11011, // ■■   ■■   (오른쪽 수염)
	0b11110, // ■■■■      (턱 선)
	0b00000  //
};
void create_char_one(void){
	lcd_create_char(0, face_left);
	lcd_create_char(1, face_mid);
	lcd_create_char(2, face_right);

	lcd_goto_xy(0, 10);
	lcd_write_data(0);

	lcd_goto_xy(0, 11);
	lcd_write_data(1);

	lcd_goto_xy(0, 12);
	lcd_write_data(2);
}

void lcd_create_char(uint8_t index, uint8_t pattern[8]){
	index %= 8;
	
	lcd_write_command(0x40 + (index * 8));
	
	for(uint8_t i = 0; i < 8; i++){
		lcd_write_data(pattern[i]);
	}
}

void lcd_pulse_enable(void){
	PORT_CONTROL |= ( 1 << E_PIN);
	_delay_us(1);
	PORT_CONTROL &= ~( 1 << E_PIN);
	_delay_us(1);
}

void lcd_write_data(uint8_t data){

	PORT_CONTROL |= ( 1 << RS_PIN);
	PORT_DATA = (PORT_DATA & 0x0F) | (data & 0xF0);	
	lcd_pulse_enable();
	PORT_DATA = (PORT_DATA & 0x0F) | ((data << 4) & 0xF0);
	lcd_pulse_enable();
	_delay_us(40);
	    
}

void lcd_write_first_command(uint8_t command){

	PORT_CONTROL &= ~(1 << RS_PIN);
	PORT_CONTROL &= ~(1 << RW_PIN);
	PORT_DATA = (PORT_DATA & 0x0F) | (command & 0xF0);
	lcd_pulse_enable();
	_delay_us(40);

}

void lcd_write_command(uint8_t command){
	
	PORT_CONTROL &= ~(1 << RS_PIN);
	PORT_CONTROL &= ~(1 << RW_PIN);
	PORT_DATA = (PORT_DATA & 0x0F) | (command & 0xF0);
	lcd_pulse_enable();
	PORT_DATA = (PORT_DATA & 0x0F) | ((command << 4) & 0xF0);
	lcd_pulse_enable();
	_delay_us(40);

}

void lcd_clear(void){
	lcd_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(2);
}

void lcd_write_string(char *string){
	uint8_t i;
	for(i = 0; string[i]; i++){
		lcd_write_data(string[i]);
	}
}

void lcd_goto_xy(uint8_t row, uint8_t col){
	row %= 2;
	col %= 16;
	
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	
	lcd_write_command(command);
}

void ds1307_watch_setting(t_ds1307 *ds1307){
	// 0버튼 누르면 해당위치의 숫자 감소
	// 1버튼 누르면 해당위치의 숫자 증가
	// 2버튼 누르면 연 월 일 요일 시 분 초 순으로 돌아가게 해야됨.
	// 3번 버튼 원위치
	// 자 여기 들어왔으면 ds1307에 있는 정보를 새롭게 쓸 필요가 전혀 없다 한번만 써주면 되고 버튼을 눌러서 바뀐다면 그걸 출력해주면됨
	//
	uint8_t *p = (uint8_t *)ds1307; // 개편함.
	static uint8_t ds1307_edit = 0;
	int ds1306_edit_member;
	// 바꾼 놈 넣어줄 함수.
	/*		시계 읽어오는 놈인데 필요 없지 않다 어차피 ds1307 그때 받아올때만 쓰면됨.
	static uint16_t prev_ms_time = 0;
	if(ms_count - prev_ms_time > 1000){
		prev_ms_time = ms_count;
		timer_test_2_read(ds1307); // 시계 읽는 놈 //인자로 넘겨주기?
	}
	*/	
	if(_first){
		// 처음 바뀌엇을때 세팅할 부분
		ds1307_edit = 0;
		lcd_write_string("SET__MODE!");
		_delay_ms(300);
		lcd_clear();
		_first = 0;
	}
	if(button0_flag){ //0에 들어오면 해당 위치의 숫자 감소. 해
		// 위치 받아서 
		button0_flag = 0;
		switch(ds1307_edit){
			case(DS1307_MEMBER_SECOND):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_MINUTES):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_HOURS):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_DAYOFWEEK):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_DAY):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_MONTH):
				p[ds1307_edit]--;
				break;
			case(DS1307_MEMBER_YEAR):
				p[ds1307_edit]--;
				break;
			default:
				break;
		}
		/*
		ds1306_edit_member = bcd2dec(p[ds1307_edit]);
		ds1306_edit_member--;
		p[ds1307_edit] = dec2bcd(ds1306_edit_member);
		*/
		
	}
	if(button1_flag){ //1에 들어오면 해당 위치의 숫자 증가
		button1_flag = 0;
		switch(ds1307_edit){
			case(DS1307_MEMBER_SECOND):
			p[ds1307_edit] = (++p[ds1307_edit]) % 60;
			break;
			case(DS1307_MEMBER_MINUTES):
			p[ds1307_edit] = (++p[ds1307_edit]) % 60;
			break;
			case(DS1307_MEMBER_HOURS):
			p[ds1307_edit] = (++p[ds1307_edit]) % 24;
			break;
			case(DS1307_MEMBER_DAYOFWEEK):
			p[ds1307_edit] = (++p[ds1307_edit]) % 7;
			break;
			case(DS1307_MEMBER_DAY):
			p[ds1307_edit] = (++p[ds1307_edit]) % 31;
			if(p[ds1307_edit] == 0){	p[ds1307_edit]++; }
			break;
			case(DS1307_MEMBER_MONTH):
			p[ds1307_edit] = (++p[ds1307_edit]) % 12;
			break;
			case(DS1307_MEMBER_YEAR):
			p[ds1307_edit] = (++p[ds1307_edit]) % 99;
			break;
			default:
			break;
		}		/*
		ds1306_edit_member = bcd2dec(p[ds1307_edit]);
		ds1306_edit_member++;
		p[ds1307_edit] = dec2bcd(ds1306_edit_member);
		*/
		
	}
	if(button2_flag){ //2에 들어오면 연 월 일 용리 시 분 초 순으로 바뀌는 위치를 prev[i] 값을 바꿔줘야됨.
		// 아니지 생각을 거꾸로해보자 어차피 ds1307 구조체가 들어왓짢아.
		// 저 주고체는 8비트 단위로 값이 들어있으며 bcd 구조임 사람눈에 보이기 쉬운 구조라 이거지.
		// 만약 내가 1를 올리고 싶다면 2가지 방법이 있다.
		button2_flag = 0;
		ds1307_edit = (ds1307_edit + 1) % DS1307_MEMBER_NUMBER;	//변수 선언해버리기
	}

	// 
	lcd_goto_xy(0,0);
	lcd_write_data((p[6] / 10) + '0');	//연
	lcd_goto_xy(0,1);
	lcd_write_data((p[6] % 10) + '0');
	lcd_goto_xy(0,2);
	if(ds1307_edit == DS1307_MEMBER_YEAR){
		lcd_write_data(4);					// S
	}else{
		lcd_write_data(0);					// Y	
	}
	lcd_goto_xy(0,4);
	lcd_write_data((p[5] / 10) + '0');	//월
	lcd_goto_xy(0,5);
	lcd_write_data((p[5] % 10) + '0');
	lcd_goto_xy(0,6);
	if(ds1307_edit == DS1307_MEMBER_MONTH){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(1);					// M
	}
	lcd_goto_xy(0,8);
	lcd_write_data((p[4] / 10) + '0');	//일
	lcd_goto_xy(0,9);
	lcd_write_data((p[4] % 10) + '0');
	lcd_goto_xy(0,10);
	if(ds1307_edit == DS1307_MEMBER_DAY){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(2);					// D
	}	
	lcd_goto_xy(0,12);
	lcd_write_data((p[3] / 10) + '0');	//요일 이것좀 이쁘게 바꾸고 싶은데.
	lcd_goto_xy(0,13);
	lcd_write_data((p[3] % 10) + '0');
	if(ds1307_edit == DS1307_MEMBER_DAYOFWEEK){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(5);					// DAY
	}
	lcd_goto_xy(1,1);
	lcd_write_data((p[2] / 10) + '0');	//시
	lcd_goto_xy(1,2);
	lcd_write_data((p[2] % 10) + '0');
	lcd_goto_xy(1,3);
	if(ds1307_edit == DS1307_MEMBER_HOURS){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(3);					// colon
	}
	lcd_goto_xy(1,4);
	lcd_write_data((p[1] / 10) + '0');	//분
	lcd_goto_xy(1,5);
	lcd_write_data((p[1] % 10) + '0');
	lcd_goto_xy(1,6);
	if(ds1307_edit == DS1307_MEMBER_MINUTES){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(3);					// colon
	}
	lcd_goto_xy(1,7);
	lcd_write_data((p[0] / 10) + '0');	//초
	lcd_goto_xy(1,8);
	lcd_write_data((p[0] % 10) + '0');
	lcd_goto_xy(1,9);
	if(ds1307_edit == DS1307_MEMBER_SECOND){
		lcd_write_data(4);					// S
		}else{
		lcd_write_data(3);					// colon
	}
}

void lcd_create_clock_chars(void){
	lcd_create_char(0, alp_Y);		// 디자인
	lcd_create_char(1, alp_M);		// 디자인
	lcd_create_char(2, alp_D);		// 디자인
	lcd_create_char(3, colon);		// 디자인
	lcd_create_char(4, alp_S);		// 디자인
	lcd_create_char(5, cat_left);		// 디자인
	lcd_create_char(6, cat_tail);		// 디자인
	lcd_create_char(7, cat_right);		// 디자인
	
	lcd_goto_xy(0,2);
	lcd_write_data(0);					// Y
	lcd_goto_xy(0,6);
	lcd_write_data(1);					// M
	lcd_goto_xy(0,10);
	lcd_write_data(2);					// D
	lcd_goto_xy(0,13);
	lcd_write_data(5);					// DAY
	lcd_goto_xy(1,3);
	lcd_write_data(3);					// COLON
	lcd_goto_xy(1,6);
	lcd_write_data(3);					// COLON
}

void lcd_write_if_changed(uint8_t row, uint8_t col, uint8_t data, uint8_t *prev){
	if(*prev != data)
	{
		lcd_goto_xy(row, col);
		lcd_write_data(data);
		*prev = data;
	}
}

void ds1307_text_lcd_Synchronization(t_ds1307 *ds1307){
		//자 노예시계하고 텍스트엘씨디 연동이여
		// 일단 저거 받아왔은께 가져다 붙여넣기만 하면되
		// 여러개를 받을 함수를 새로 만들어줘야됨.
		// 너무 쳐 오래걸려서 바뀐 문자만 갱신하자.

	uint8_t *p = (uint8_t *)ds1307; // 개편함.
	
	if(_first){
		lcd_create_clock_chars();
		_first = 0;
	}
	
	static uint16_t prev_ms_time = 0;
	if(ms_count - prev_ms_time > 1000){
		prev_ms_time = ms_count;
		timer_test_2_read(ds1307); // 시계 읽는 놈 //인자로 넘겨주기?
	}
	/*
		static uint8_t prev[16] = {
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
		};

		uint8_t now_year0 = (p[6] / 10) + '0';
		uint8_t now_year1 = (p[6] % 10) + '0';
		uint8_t now_month0 = (p[5] / 10) + '0';
		uint8_t now_month1 = (p[5] % 10) + '0';
		uint8_t now_day0 = (p[4] / 10) + '0';
		uint8_t now_day1 = (p[4] % 10) + '0';
		uint8_t now_dayweek0 = (p[3] / 10) + '0';
		uint8_t now_dayweek1 = (p[3] % 10) + '0';
		uint8_t now_hour0 = (p[2] / 10) + '0';
		uint8_t now_hour1 = (p[2] % 10) + '0';
		uint8_t now_minute0 = (p[1] / 10) + '0';
		uint8_t now_minute1 = (p[1] % 10) + '0';
		uint8_t now_second0 = (p[0] / 10) + '0';
		uint8_t now_second1 = (p[0] % 10) + '0';

	lcd_write_if_changed(0, 0, (p[6] / 10) + '0', &prev[0]);
	lcd_write_if_changed(0, 1, (p[6] % 10) + '0', &prev[1]);
	lcd_write_if_changed(0, 4, (p[5] / 10) + '0', &prev[2]);
	lcd_write_if_changed(0, 5, (p[5] % 10) + '0', &prev[3]);
	lcd_write_if_changed(0, 8, (p[4] / 10) + '0', &prev[4]);
	lcd_write_if_changed(0, 9, (p[4] % 10) + '0', &prev[5]);
	lcd_write_if_changed(0, 12, (p[3] / 10) + '0', &prev[6]);
	lcd_write_if_changed(0, 13, (p[3] % 10) + '0', &prev[7]);
	
	lcd_write_if_changed(1, 1, (p[2] / 10) + '0', &prev[8]);
	lcd_write_if_changed(1, 2, (p[2] % 10) + '0', &prev[9]);
	lcd_write_if_changed(1, 4, (p[1] / 10) + '0', &prev[10]);
	lcd_write_if_changed(1, 5, (p[1] % 10) + '0', &prev[11]);
	lcd_write_if_changed(1, 7, (p[0] / 10) + '0', &prev[12]);
	lcd_write_if_changed(1, 8, (p[0] % 10) + '0', &prev[13]);
	*/
	
	lcd_goto_xy(0,0);
	lcd_write_data((p[6] / 10) + '0');	//연	
	lcd_goto_xy(0,1);
	lcd_write_data((p[6] % 10) + '0');	
	lcd_goto_xy(0,2);
	lcd_write_data(0);					// Y
	lcd_goto_xy(0,4);
	lcd_write_data((p[5] / 10) + '0');	//월
	lcd_goto_xy(0,5);
	lcd_write_data((p[5] % 10) + '0');	
	lcd_goto_xy(0,6);
	lcd_write_data(1);					// M
	lcd_goto_xy(0,8);
	lcd_write_data((p[4] / 10) + '0');	//일
	lcd_goto_xy(0,9);
	lcd_write_data((p[4] % 10) + '0');
	lcd_goto_xy(0,10);
	lcd_write_data(2);					// D
	lcd_goto_xy(0,12);
	lcd_write_data((p[3] / 10) + '0');	//요일 이것좀 이쁘게 바꾸고 싶은데.
	lcd_goto_xy(0,13);
	lcd_write_data((p[3] % 10) + '0');
	lcd_write_data(5);					// DAY
	
	lcd_goto_xy(1,1);
	lcd_write_data((p[2] / 10) + '0');	//시
	lcd_goto_xy(1,2);
	lcd_write_data((p[2] % 10) + '0');
	lcd_goto_xy(1,3);
	lcd_write_data(3);					// COLON
	lcd_goto_xy(1,4);
	lcd_write_data((p[1] / 10) + '0');	//분
	lcd_goto_xy(1,5);
	lcd_write_data((p[1] % 10) + '0');
	lcd_goto_xy(1,6);
	lcd_write_data(3);					// COLON
	lcd_goto_xy(1,7);
	lcd_write_data((p[0] / 10) + '0');	//초
	lcd_goto_xy(1,8);
	lcd_write_data((p[0] % 10) + '0');

	lcd_goto_xy(1,11);
	lcd_write_data(5);					// go
	lcd_goto_xy(1,12);
	lcd_write_data(7);					// yang
	lcd_goto_xy(1,13);
	lcd_write_data(6);					// yee

	

	/*  테스트용으로 프린터 찍어본거임
		printf("%02d:%02d:%02d  %02d/%02d/%02d  dow=%d\r\n",
		p[2],p[1],p[0],p[6],p[5],p[4],p[3]); // 시 분 초 연 월 일 요일
		
	*/
}

void lcd_init(void){
	//_delay_ms(50);
	
	DDR_DATA |= 0xf0;
	PORT_DATA &= ~0xf0;
	DDR_CONTROL |= (1 << RS_PIN) | ( 1<< RW_PIN) | (1 << E_PIN);
	
	// rw rs 둘다 로우로 내려서 쓰기 모드로 들어감.
	PORT_CONTROL &= ~(1 << RS_PIN);
	PORT_CONTROL &= ~(1 << RW_PIN);
	PORT_CONTROL &= ~(1 << E_PIN);
	
	// 8bit 모드로 3번 안정화
	lcd_write_first_command(COMMAND_8_BIT_MODE_FIRST);
	_delay_ms(5);

	lcd_write_first_command(COMMAND_8_BIT_MODE_FIRST);
	_delay_ms(1);

	lcd_write_first_command(COMMAND_8_BIT_MODE_FIRST);
	_delay_ms(1);

	// 4bit 모드 진입
	lcd_write_first_command(COMMAND_4_BIT_MODE_FIRST);
	_delay_ms(1);
	
	lcd_write_command(COMMAND_4_BIT_MODE);
	
	uint8_t _command = 0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT);
	lcd_write_command(_command);
	
	lcd_clear();
	
	
	lcd_write_command(0x06);	//커서 하나밀어
	
	
}

void lcd_write_cal_window(char *buff, uint8_t count){
	uint8_t start = 0;
	if(count > 16)
	start = count - 16;
	lcd_goto_xy(0, 0);
	for(uint8_t i = 0; i < 16; i++)
	{
		if(buff[start + i] != '\0')
		lcd_write_data(buff[start + i]);
		else
		lcd_write_data(' ');
	}
}