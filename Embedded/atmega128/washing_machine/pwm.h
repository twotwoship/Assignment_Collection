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
#include "led.h"

void init_timer3_pwm(void);
void init_motor_diver(void);
void dc_motor_pwm_control_main(void);

void _washing_mode_func(void);
void _rinse_mode_func(void);
void _dehydration_mode_func(void);
void _washing_setting_mode_func(void);
void _rinse_setting_mode_func(void);
void _dehydration_setting_mode_func(void);

void start_on(void);
void stop_on(void);

#endif /* PWM_H_ */