/*
 * i2c_S_loopback.c
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

#include "i2c_s_lookback.h"

// I2C 설정
#define SLAVE_ADDR 0x60		//7bit slave 주소

//tr는 polling rx는 interrupt
//-------------- TWI 상태 코드(SLAVE) --------------------
/* TWSR 의 상태를 보고 코드를 짜야한다 하드웨어가 상태를 변경시킨다, 사람이 바꾸는게 아님. DATASHEET P.288
TWSR(TWI Status Register) : 7번 ~ 3번 bit를 참조. 역할 -> i2c 통신의 전송상태나 오류를 나타 낸다.
*/

// 아래의 코드를 수신하면 ISR(TWI_vect) 상태로 분기 된다. 인터럽트 서비스 루틴으로 슈윳

//Table 26-5 Status Codes for Slave Receiver Mode
#define TWSR_SR_SLA_ACK	0x60		//	Own SLA+W has been received, ACK has been returned 
#define TWSR_SR_DATA_ACK 0x80		//	Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWSR_SR_STOP 0xA0			//	A STOP condition or repeated START condition has been received while still addressed as Slave

//Table 26-6 Status Codes for Slave Transmitter Mode
#define TWSR_ST_SLA_ACK 0xA8		//	Own SLA+R has been received; ACK has been returned
#define TWSR_ST_DATA_ACK 0xB8		//	Data byte in TWDR has been transmitted; ACK has been received
#define TWSR_ST_DATA_NACK 0xC0		//	Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWSR_ST_LAST_DATA 0xC8		//	Last data byte in TWDR has been transmitted (TWEA =“0”); ACK has been received

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
#define RX_SIZE 16
volatile uint8_t rx_buff[RX_SIZE];			// 최적화 방지용 컴파일러
volatile int rx_len;					// 수신 byte 수
volatile int tx_index;					// 송신 index

///////////////////////////////////////
// TWI interrupt service routine
//////////////////////////////////////

ISR(TWI_vect){
	uint8_t st = TWSR & 0xf8;		// 7 - 3 bit 값을 덮어 씌운다
	
	switch(st){
// slave 수신 단계.============================================
		case TWSR_SR_SLA_ACK:	// 노예 주소 읽쓱수신 ack 전송
			rx_len = 0;
			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// ack 발사
			break;
			
		case TWSR_SR_DATA_ACK:	// 노예 데이터 수신 ack 전송
			if(rx_len < RX_SIZE){
				rx_buff[rx_len++] = TWDR;
			}
			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// ack 발사
			break;
			
		case TWSR_SR_STOP:		// 노예 멈춰 현재 loopback 코드상에서 만든 거임. 네이밍을 이래함 잠시 멈춰야해서
			tx_index = 0;		// 주석 처리 해보기 이친구가 문제가 있다.
			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// 읽기 모드로 대기
			break;
// slave 전송 단계.================================================
		case TWSR_ST_SLA_ACK:	// 노예 읽기모드 수신 ack 전송 
			tx_index = 0;		// 첫번째 byte 수신 준비?// d아래 부분 
																	
			// ack 를 이미 전송한 뒤기 때문에 첫번째 바이트 송신 준비를 해야된다?
			if( tx_index < rx_len){
				TWDR = rx_buff[tx_index++];
			}else{
				TWDR = '?'; // 버퍼 이상 발생 : dummy data 
			}

			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// 현재 작업 완료 처리 후 TWI 재개, 이후 주소 매칭 시 ACK 허용
			break;
		case TWSR_ST_DATA_ACK:	// 데이터 전송 : 악 수신
			if( tx_index < rx_len){
				TWDR = rx_buff[tx_index++];
			}else{
				TWDR = '?'; // 버퍼 이상 발생 : dummy data 
			}

			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// ack 준비
			break;
		case TWSR_ST_DATA_NACK:		//nack 받으면 송신 종료
		case TWSR_ST_LAST_DATA:		// 마지막 데이터 전송 + ack 수신 @@@@점검
			tx_index = 0;
			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// ack 준비
			break;
		default:	//error 복구 twint clear, ack 재활성화 한다.
			TWCR = (1 << TWINT | 1 << TWEA | 1 << TWEN | 1 << TWIE);	// ack 준비
			break;
	}	
}

// TWI(I2C) driver
void init_slave_i2c(void){		//초기화
	TWAR = (SLAVE_ADDR << 1);
	TWCR = ( 1 << TWEA | 1 << TWEN | 1 << TWIE);		// ack 활성화 
}

int i2c_main(void){
	
	init_slave_i2c();
	sei();
	
	while(1){
	}
}

