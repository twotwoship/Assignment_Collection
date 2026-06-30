/*
 * i2c_S_lookback.h
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
#include <util/delay.h>  // _delay_ms _delay_us 등


// TWI(I2C) driver

void init_slave_i2c(void);
int i2c_main(void);
#endif /* I2C_M_LOOKBACK_H_ */