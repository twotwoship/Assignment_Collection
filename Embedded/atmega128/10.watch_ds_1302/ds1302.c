/*
 * ds1302.c
 *
 * Created: 2026-06-26 오후 2:42:12
 *  Author: kccistc
 */ 
#include "ds1302.h"
#include "uart0.h"
#include "interrupt.h"
#include "timer0.h"
#include <avr/interrupt.h> // sei cli ....etc interrupt function

volatile uint8_t watch = 0;


void ds1302_main(void){
	
	t_ds1302 ds1302;
	
	init_date_time(&ds1302);
	init_ddr_ds1302();
	init_gpio_ds1302();	//all 3개 포트 출력을 low로 설정
	
	//init_ds1302(&ds1302);		// 내가 원하는 시간 집어넣는 것.
	
	while(1){
			pc_command_processing(&ds1302);
			// 1. read time
			read_time_ds1302(&ds1302);
			// 2. read date
			read_date_ds1302(&ds1302);
			// 3. printf date & time
			UART0_print_1_byte_number(ds1302.year);
			UART0_print_string("\n");
			printf("date : %d-%d-%d-%d | time : %d-%d-%d \n", 
			ds1302.year, ds1302.month, ds1302.date, 
			ds1302.dayofweek, ds1302.hours, ds1302.minutes, 
			ds1302.second);
		// 4. delay_ms(1000);
		_delay_ms(1000);
	}
}


void ds1302_main_test(void){
	
	t_ds1302 ds1302;
	
	init_date_time(&ds1302);
	init_ddr_ds1302();
	init_gpio_ds1302();	//all 3개 포트 출력을 low로 설정
	
	//init_ds1302(&ds1302);		// 내가 원하는 시간 집어넣는 것.
	
	while(1){
		//pc_command_processing_2(&ds1302);
		if(watch){
			watch = 0;	
			//printf("tick\n");
			pc_command_processing_2(&ds1302);
			// 1. read time
			read_time_ds1302(&ds1302);
			// 2. read date
			read_date_ds1302(&ds1302);
			// 3. printf date & time
			//UART0_print_1_byte_number(ds1302.year);
			//UART0_print_string("\n");
			printf("date : %d-%d-%d-%d | time : %d-%d-%d \n", 
			ds1302.year, ds1302.month, ds1302.date, 
			ds1302.dayofweek, 
			ds1302.hours, ds1302.minutes, ds1302.second);
			
		}
		// 4. delay_ms(1000);
		//_delay_ms(1000);
	}
}

void read_time_ds1302(t_ds1302 *ds1302){
	ds1302->second = read_ds1302(ADDR_SECONDS);	
	ds1302->minutes = read_ds1302(ADDR_MINUTES);
	ds1302->hours = read_ds1302(ADDR_HOUR);
}

void read_date_ds1302(t_ds1302 *ds1302){
	ds1302->date = read_ds1302(ADDR_DATE);
	ds1302->month = read_ds1302(ADDR_MONTH);
	ds1302->dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	ds1302->year = read_ds1302(ADDR_YEAR);
}

uint8_t read_ds1302(uint8_t addr){
	uint8_t data8bits = 0;	// 1bit씩 읽어서 담을 변수
	
	 cli(); // bit 해드 뱅잉 방식이라 
	 // 비트가 흔들어지는 동안에는 잠시 꺼줘야한다.
	 
	// 1.ce high
	// 2. addr 전송
	// 3. data를 읽어들임.
	// 4. ce low
	// 5. return (bcd to dec)
	
	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	// 2. addr 전송
	tx_ds1302(addr+1); // 읽기주소는 더하기 1해줘야됨.
	// 3. data 읽기
	rx_ds1302(&data8bits);
	// 4. CE high -> low
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	
	sei(); // 그리고 다시 키기

	return (bcd2dec(data8bits));
}
// 버스트 모드를 하면 한번에 64개 읽어온다.

void _bust_read_ds1302(uint8_t addr, uint64_t *temp){
	
	cli(); // bit 해드 뱅잉 방식이라
	// 비트가 흔들어지는 동안에는 잠시 꺼줘야한다.
	
	// 1.ce high
	// 2. addr 전송
	// 3. data를 읽어들임.
	// 4. ce low
	// 5. return (bcd to dec)
	
	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	// 2. addr 전송
	tx_ds1302(addr);
	// 3. data 읽기
	_bust_rx_ds1302(temp);
	// 4. CE high -> low
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	
	sei(); // 그리고 다시 키기
}

void _bust_rx_ds1302(uint64_t *temp){

	//1. 입력 mode로 설정
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT);	//read mode
	//예) 0x80 LSB 부터 차례대로 들어옴.
	// L       M
	// 1000 0000
	for(int i = 0; i < 64; i++){
		if(DS1302_DAT_PIN & (1 << DS1302_DAT)){
			*temp |= (uint64_t)1 << i;		// 1들어올 때만 set함
			// 1임마가 문제임 ㄹㅇ!!!!!!!!!!!!!!!!!!!!
			// 1이 int형이였던거임
		}
		if(i != 63){		//마지막 비트를 읽을때는 클럭을 안보냄.
			clock_ds1302();
		}
	}
}

void rx_ds1302(uint8_t *pdata8bits){
	uint8_t temp = 0;
	//1. 입력 mode로 설정
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT);	//read mode
	//예) 0x80 LSB 부터 차례대로 들어옴.
	// L       M
	// 1000 0000
	for(int i = 0; i < 8; i++){
		if(DS1302_DAT_PIN & (1 << DS1302_DAT)){
			temp |= 1 << i;		// 1들어올 때만 set함
		}
		if(i != 7){		//마지막 비트를 읽을때는 클럭을 안보냄.
			clock_ds1302();
		}
	}
	*pdata8bits = temp;
}

void init_ds1302(t_ds1302 *ds1302){
	write_ds1302(ADDR_SECONDS, ds1302->second);
	write_ds1302(ADDR_MINUTES, ds1302->minutes);
	write_ds1302(ADDR_HOUR, ds1302->hours);
	write_ds1302(ADDR_DATE, ds1302->date);
	write_ds1302(ADDR_MONTH, ds1302->month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302->dayofweek);
	write_ds1302(ADDR_YEAR, ds1302->year);
}

//dec ----> bcd
//예시 25
// dec			bcd
// 0001 1001	0010 0101
uint8_t dec2bcd(uint8_t data){
	uint8_t high, low;
	
	high = (data/10) << 4;
	low = (data%10);
	
	return(high + low);
}
//bcd ---------> dec
// 예시 26년
// 0010 0110
// 2	6
// x10	x1
// 0001 1010


uint8_t bcd2dec(uint8_t data){
	uint8_t high, low;
	
	low = data & 0x0f;
	high = (data >> 4) * 10;
	
	return (high + low);
}
void write_ds1302(uint8_t addr, uint8_t data){
	    cli();


	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	// 2. addr 전송
	tx_ds1302(addr);
	// 3. data 전송
	tx_ds1302(dec2bcd(data));
	// 4. CE high -> low
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	
	
	sei();
	
}



void tx_ds1302(uint8_t data){
	//1. 출력 mode로 설정
	DS1302_DAT_DDR |= 1 << DS1302_DAT;	//write mode
	//예) 0x80
	// M       L
	// 1000 0000
	for(int i = 0; i < 8; i++){
		if(data & (1 << i)){
			DS1302_DAT_PORT |= 1 << DS1302_DAT; // 1
		}else{
			DS1302_DAT_PORT &= ~(1 << DS1302_DAT); // 0
		}
		clock_ds1302();
	}
}

void clock_ds1302(void){
	// low에서 high로 갓다가 다시 low로 감.
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
	DS1302_CLK_PORT |= (1 << DS1302_CLK);
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
}

void init_ddr_ds1302(void){
	DDRF &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	DDRF |= 1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST; // 출력 모드 설정.
}

void init_gpio_ds1302(void){
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	_delay_ms(2);
}

void init_date_time(t_ds1302 *ds1302){
	ds1302->year = 26;
	ds1302->month = 06;
	ds1302->date = 29;
	ds1302->dayofweek= 0;	//friday
	ds1302->hours = 20;
	ds1302->minutes = 05;
	ds1302->second = 00;
}
