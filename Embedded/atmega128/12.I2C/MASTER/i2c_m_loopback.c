/*
 * i2c_m_loopback.c
 *
 * Created: 2026-06-30 오전 10:52:51
 *  Author: kccistc
 */ 
/*	
	"ABC" 문자열을 slave로 전송하고 slave가 돌려보낸 문자열을 수신한여 uart로 출력 하는 loop-back test program을 작성 한다.
	atmega128a 2개를 연결한다 1개는 master 1개는 slave로 동작 하도록 한다.
	
	
	
	H/W 조건
	=========
	1. I2C 연결
	pull-up 저항은 master만 연결
	PD0 / SCL 10K pull-up 
	PD1 / SDA 10K pull-up 
	두 atmega의 gnd 라인은 공통연결, vcc는 각자 알아서.	
	
	I2C address = 0x60 7bit만 사용
	0110 0000 --> 1bit를 shift 시킨다 --> 실제 전송--> 1100 0000 C0 0번 bit가 read write 자리임. 읽기시에는 C1임 

	
	2. 속도
	F_CPU : 16MHz I2C 속도 : 100KHz UART : 9600Bps
	
	3. Handshaking
		ACK / NACK 의 의미
		
		신호		SDA		Slave 입장에서의 의미
		====================================
		ACK		LOW		다음 바이트도 계속 전송, 정상 수신
		NACK	HIGH	이제 그만 보내쇼.
 */ 

#include "i2c_m_lookback.h"

// I2C 설정
#define SLAVE_ADDR 0x60		//7bit slave 주소
#define I2C_FREQUENCY 100000UL		//100KHz
// TWBR : Two wire bit rate (?)
// TWBR : (F_CPU / I2C_FREQUENCY - 16) / 2 (분주비 = 1)
#define TWBR_VALUE (F_CPU/ I2C_FREQUENCY - 16)/2 //TWBR(TWI Bit RAte Register)

//-------------- TWI 상태 코드 --------------------
/* TWSR 의 상태를 보고 코드를 짜야한다 하드웨어가 상태를 변경시킨다, 사람이 바꾸는게 아님. DATASHEET P.288
TWSR(TWI Status Register) : 7번 ~ 3번 bit를 참조. 역할 -> i2c 통신의 전송상태나 오류를 나타 낸다.
*/
//Table 26-3 Status Codes for Master Transmitter Mode // 4bit 씩 숫자계산하는걸 기억해라. hex
#define TWSR_START 0x08				//	A START condition has been transmitted.
#define TWSR_MT_SLA_ACK 0x18		//	SLA+W has been transmitted, ACK has been received.
#define TWSR_MT_DATA_ACK 0x28		//	Data byte has been transmitted, ACK has been received
//Table 26-4 Status codes for Master Receiver Mode
#define TWSR_MR_SLA_ACK 0x40		//	SLA+R has been transmitted, ACK has been received
#define TWSR_MR_DATA_ACK 0x50		//	Data byte has been received, ACK has been returned
#define TWSR_MR_DATA_NACK 0x58		//	Data byte has been received, NOT ACK has been returned

/*
	ATmega128a TWI(I2C) 관련 register
	TWCI : TWI control register // book p.391
		- 긱 bit의 역할
		TWINT	:7	인터럽트 플래그 ( 1이면 CLEAR) 명령을 내린 I2C 작업이 완료되고 기다릴때 하드웨어에 의해 세트됨.
		TWEA	:6	ACK enable (1을 세팅하면 자동으로 ACK 펄스가 생성됨, 0이면 NACK 펄스 생성)
		TWSTA	:5	START 신호 발생 / 마스터 모드에서 전송을 시작하기 위해
		TWSTO	:4	STOP 신호 발생 / 마스터 모드에서 전송을 끝내기 위해
		TWWC	:3	
		TWEN	:2	TWI(I2C) 활성화
		XXXX	:1	미사용
		TWIE	:0	TWI interrupt 활성화 : TWI 데이터 전송이 완료 된 경우 인터럽트 발생을 허용
	TWSR : TWI status register // 상위 5비트 == 상태 코드, 하위 2비트 == 분주비prescaler
	TWBR : TWI bit rate register
	TWDR : TWI  data register
	TWAR : TWI address register ( slave 모드에서만 사용 : 나의 어드레스를 설정하는 것)
나머지 비트는 직접 찾아보자
*/

// TWI(I2C) driver
void init_i2c(void){		//초기화
	TWSR = 0x00;
	TWBR = TWBR_VALUE; // SCL 주파수 결정 register 100KHz
}

//start 신호
uint8_t i2c_start(void){
	
	uint8_t st;
	uint32_t timeout = 100000;

	
	TWCR = 1 << TWINT | 1 << TWSTA | 1 << TWEN;	//주석 참조.
	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	while( !(TWCR & ( 1 << TWINT)) ){
				if (--timeout == 0) {
					printf("ERR : START TIMEOUT\r\n");
					return 0xff;
				}
	}
	st = TWSR & 0xf8;			// 7 ~ 3까지 값을 취한다.
	
	return (st == TWSR_START ) ? 0 : st;		// start bit 가 정상적으로 갔으면 st 아니면 에라 상태 코드 반환
}

//stop 신호
void i2c_stop(void){
		
	TWCR = 1 << TWINT | 1 << TWSTO | 1 << TWEN;	//주석 참조, 스탑 요청
	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	// STOP 후 TWINT clear 되기 까지 대기
	_delay_us(10);
	
}

// SLAVE_ADDR+R SLAVE_MODE+W


uint8_t i2c_data_write(uint8_t data){
	
	TWDR = data;
	TWCR = 1 << TWINT | 1 << TWEN;	//주석 참조.
	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	while( !(TWCR & ( 1 << TWINT)) );
	
	return (TWSR & 0xf8);			// 7 ~ 3까지 값을 취한다.
}

uint8_t i2c_data_read_acksend(void){
	
	TWCR = 1 << TWINT | 1 << TWEN | 1 << TWEA;	// twea 하면 아크가 나감

	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	while( !(TWCR & ( 1 << TWINT)) );
	
	return TWDR;	
}

uint8_t i2c_data_read_nacksend(void){
	
	TWCR = 1 << TWINT | 1 << TWEN;	// twea = 0 하면 아크가 나감

	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	while( !(TWCR & ( 1 << TWINT)) );
	
	return TWDR;
}
uint8_t i2c_slave_addr_send(uint8_t addr_rw){
	
	uint8_t st;
	TWDR = addr_rw;
	TWCR = 1 << TWINT | 1 << TWEN;	//주석 참조.
	// I2C HW가 동작이 완료 될 떄까지 기다린다. 동작이 완료되면 TWINT 플래그를 1로 세트.
	while( !(TWCR & ( 1 << TWINT)) );
	st = TWSR & 0xf8;			// 7 ~ 3까지 값을 취한다.
	
	return st;
}
void loopback_test(void){
	char tx_msg[] = "six";
	uint8_t tx_msg_len = strlen(tx_msg);
	char rx_buff[16] = {0};
	uint8_t status;
	
	//--------------------------------------
	// phase#1 : master --> slave 전송
	//--------------------------------------
	
	//1. start 전송
	status = i2c_start();
	if(status){		//잘 시작했나 확인.
		printf("ERR : START FAILED ?!?!?! \n");
		return;
	}
	
	// 2. SLAVE_ADDR + W 전송
	status = i2c_slave_addr_send(SLAVE_ADDR << 1);	//0xc0 : slave addr << 1
	if(status != TWSR_MT_SLA_ACK ){
		printf("ERR : SLAVE_ADDR + NACK ?!?!?! \n");
		i2c_stop();
		return;
	}
	
	//3. data 3byte write
	for(int i = 0 ; tx_msg[i] != '\0'; i++){
		status = i2c_data_write(tx_msg[i]);
		if(status != TWSR_MT_DATA_ACK){
			printf("ERR : DATA WRITE + NACK ?!?!?! \n");
			i2c_stop();
			return;	
		}
	}
	
	//4. stop signal
	i2c_stop();
	// start 0xC0 ACK 'A' ACK 'B' ACK 'C' ACK STOP // 지금까지 흐름의 구조
	
	//5. 대기 _delay_ms(5);
	_delay_ms(5);		//slave 대기 시간
	
	//--------------------------------------
	//phase#2 : master <---- slave 전송
	//--------------------------------------
	// 1. start
		status = i2c_start();
		if(status){		//잘 시작했나 확인.
			printf("ERR : START READ FAILED ?!?!?! \n");
			return;
		}
		
	// 2. SLAVE_ADDR + R 전송
	status = i2c_slave_addr_send((SLAVE_ADDR << 1) | 1);	//0xc1 : slave addr + r
	if(status != TWSR_MR_SLA_ACK ){
		printf("ERR : SLAVE_ADDR READ + NACK ?!?!?! \n");
		i2c_stop();
		return;
	}
	
	// 3. 3byte RECEIVE A, B, C AND ACK TRANSMITT
	for(int i = 0; i < tx_msg_len; i++){
		if(i == tx_msg_len-1){
			rx_buff[i] = i2c_data_read_nacksend();
		}else{
			rx_buff[i] = i2c_data_read_acksend();	
		}
	}
	/*
	rx_buff[0] = i2c_data_read_acksend();		//	A를 수신하고 ACK 전송(다음 놈도 보내라)
	rx_buff[1] = i2c_data_read_acksend();		//	B를 수신하고 ACK 전송(다음 놈도 보내라)
	rx_buff[2] = i2c_data_read_nacksend();		//	C를 수신하고 NACK 전송(고만 보내라)
	*/
	// 4. i2c stop
	i2c_stop();
	
	////// 결과 출력 ////////////
	printf("TX : %s, RX : %s \n", tx_msg, rx_buff);
	if(strncmp(tx_msg, rx_buff, tx_msg_len) == 0){
		printf("LOOP_BACK_OK  ^o^; \n");
	}else{
		printf("LOOP_BACK_NOT OK OTL !!!  \n");
	}
}
//---------------------------------------------
//look back test : "ABC" 전송 --> 수신
//---------------------------------------------
int i2c_main(void){
	
	init_i2c();
	
	printf("i2c loopback test start !@!@!@! \n");
	while(1){
		loopback_test();
		_delay_ms(1000);
	}
}

