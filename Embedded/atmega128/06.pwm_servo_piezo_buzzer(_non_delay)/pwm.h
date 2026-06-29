/*
 * pwm.h
 *
 * Created: 2026-06-18 오후 2:22:46
 *  Author: kccistc
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "button.h"
#include <avr/interrupt.h>
#include <stdio.h>

void init_timer1_pwm(void);
int servo_motor_main(void);

#endif /* PWM_H_ */