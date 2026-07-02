/*
 * ds1307.c
 *
 * Created: 2026-07-01 오후 1:22:19
 *  Author: kccistc
 */ 

#include "ds1307.h"
#include "uart0.h"


void init_date_time(t_ds1307 *ds1307){
	ds1307->second = 00;
	ds1307->minutes = 58;
	ds1307->hours = 15;
	ds1307->dayofweek= 2;	//friday
	ds1307->day = 01;
	ds1307->month = 07;
	ds1307->year = 26;
	//ds1307->wp = 00;
}

void i2c_init(void){
	DDRD &= ~(1 << I2C_SCL);
	DDRD &= ~(1 << I2C_SDA);

	PORTD |= (1 << I2C_SCL) | (1 << I2C_SDA);
	
	//200khz	
	TWSR = 0x00;
	TWBR = 32;
}

void i2c_start(void){
	
	uint8_t st;

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while( !(TWCR & ( 1 << TWINT)) );	// 전송 완료 대기
	
	//	printf("TWSR = %02X\r\n", TWSR & 0xF8);
	
}

void i2c_transmit(uint8_t data){
	TWDR = data;
	TWCR = (1 << TWINT) | ( 1 << TWEN) | ( 1 << TWEA);	// TWEA 빼도 되냐?
	
	while( !(TWCR & ( 1 << TWINT)) );	// 전송 완료 대기
	
	//	printf("TX 0x%02X, TWSR = %02X\r\n", data, TWSR & 0xF8);

}

void i2c_stop(void){
	TWCR = ( 1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

uint8_t i2c_receive_ack(void){
	TWCR = (1 << TWINT) | ( 1 << TWEN) | ( 1 << TWEA);
	
	while( !(TWCR & ( 1 << TWINT)) );	// 수신 완료 대기

	//	printf("RX ACK, TWSR = %02X\r\n", TWSR & 0xF8);
	
	return TWDR;
}

uint8_t i2c_receive_nack(void){
	TWCR = (1 << TWINT) | ( 1 << TWEN);
	
	while( !(TWCR & ( 1 << TWINT)) );	// 수신 완료 대기
	
	//	printf("RX NACK, TWSR = %02X\r\n", TWSR & 0xF8);

	return TWDR;
}

uint8_t bcd2dec(uint8_t data){
	uint8_t high, low;
	
	low = data & 0x0f;
	high = (data >> 4) * 10;
	
	return (high + low);
}

uint8_t dec2bcd(uint8_t data){
	uint8_t high, low;
	
	high = (data/10) << 4;
	low = (data%10);
	
	return(high + low);
}


void timer_test(t_ds1307 *ds1307){
	
	uint8_t *p = (uint8_t *)ds1307;
	printf("timer_test_start !!!!!!!!!!! \n");
	//i2c 시작
	i2c_start();
	printf("start_end !!!!!!!!!!! \n");
	//i2c 주소 보내기 쓰기모드
	i2c_transmit(SLAVE_ADDR << 1);
	
	//i2c 메모리 시작 주소 전송
	i2c_transmit(0);

	//시간 기입
	for(int i = 0; i< 7; i++){
		printf("%d byte written-- \r\n", i);
		i2c_transmit(dec2bcd(p[i]));
	}
	i2c_stop();
	
	_delay_ms(2000);
	
	i2c_start();
	i2c_transmit(SLAVE_ADDR << 1);
	i2c_transmit(0);
	i2c_stop();
	
	i2c_start();
	// 읽기모드
	i2c_transmit((SLAVE_ADDR << 1) + 1);
	
	printf("time data \r\n");
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d : ", bcd2dec(i2c_receive_ack()));
	printf("%4d \n", bcd2dec(i2c_receive_nack()));
	
	i2c_stop();
	
}

void timer_test_1(t_ds1307 *ds1307)
{
	printf("timer_test_start\r\n");

	i2c_start();
	printf("after start\r\n");

	i2c_transmit(SLAVE_ADDR << 1);
	printf("after slave addr write\r\n");

	i2c_transmit(0);
	printf("after reg addr\r\n");

	while (1);
}

void timer_test_2_set(t_ds1307 *ds1307){
		uint8_t *p = (uint8_t *)ds1307;

		i2c_start();
		i2c_transmit(SLAVE_ADDR << 1);
		i2c_transmit(0x00);
		
		i2c_transmit(dec2bcd(p[0]) & 0x7F);   // seconds, CH=0

		for (int i = 1; i < 7; i++)
		{
			i2c_transmit(dec2bcd(p[i]));
		}
		i2c_stop();
}

void timer_test_2_read(t_ds1307 *ds1307){
	uint8_t *p = (uint8_t *)ds1307;
	
	i2c_start();
	i2c_transmit(SLAVE_ADDR << 1);
	i2c_transmit(0x00);

	i2c_start();
	i2c_transmit((SLAVE_ADDR << 1) | 1);

	p[0] = bcd2dec(i2c_receive_ack()); // bcd 58임
	p[1] = bcd2dec(i2c_receive_ack());
	p[2] = bcd2dec(i2c_receive_ack());
	p[3] = bcd2dec(i2c_receive_ack());
	p[4] = bcd2dec(i2c_receive_ack());
	p[5] = bcd2dec(i2c_receive_ack());
	p[6] = bcd2dec(i2c_receive_nack());
	
	//printf("sec raw = 0x%02X\r\n", p[0]);  // 클럭 들어가나 볼라고 한거임.
	
	i2c_stop();
	
	
	/*
	printf("%02d:%02d:%02d  %02d/%02d/%02d  dow=%d\r\n",
		bcd2dec(p[2] & 0x3F),
		bcd2dec(p[1]),
		bcd2dec(p[0] & 0x7F),
		bcd2dec(p[6]),
		bcd2dec(p[5]),
		bcd2dec(p[4]),
		bcd2dec(p[3]));
	*/
	_delay_ms(1000);
}