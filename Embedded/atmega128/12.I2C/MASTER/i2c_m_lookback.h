/*
 * i2c_m_lookback.h
 *
 * Created: 2026-06-30 오전 10:55:36
 *  Author: kccistc
 */ 


#ifndef I2C_M_LOOKBACK_H_
#define I2C_M_LOOKBACK_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> // sei cli ....etc interrupt function
#include <stdio.h>
#include <string.h>
#include <util/delay.h>  // _delay_ms _delay_us 등


// TWI(I2C) driver
void init_i2c(void);
uint8_t i2c_start(void);
void i2c_stop(void);
uint8_t i2c_data_write(uint8_t data);
uint8_t i2c_data_read_acksend(void);

uint8_t i2c_data_read_nacksend(void);
uint8_t i2c_slave_addr_send(uint8_t addr_rw);

void loopback_test(void);
int i2c_main(void);
#endif /* I2C_M_LOOKBACK_H_ */