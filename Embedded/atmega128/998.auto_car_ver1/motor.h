/*
 * motor.h
 *
 * Created: 2026-06-19 오후 9:16:54
 *  Author: kccistc
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_
#define F_CPU 16000000UL
#include <avr/io.h>

#define MOTOR_DRIVER_DDR

#define MOTOR_LEFT_SPEED OCR1A	// 0 ~ 1023 값 사이
#define MOTOR_RIGHT_SPEED OCR1B	// 0 ~ 1023 값 사이
#define MOTOR_LEFT_CONTROL 5 // PB5
#define MOTOR_RIGHT_CONTROL 6 // PB6
#define MOTOR_CONTROL_SETTING DDRB
#define MOTOR_INPUT_ DDRE
#define MOTOR_INPUT_CONTROL_ PORTE

#define MOTOR_DIRVER_IN_SET DDRF
#define MOTOR_FORWARD_BACKWARD PORTF


#define MOTOR_LEFT_VCC 0
#define MOTOR_LEFT_GND 1
#define MOTOR_RIGHT_VCC 2
#define MOTOR_RIGHT_GND 3

void init_motor(void);
void motor_speed(int left, int right);
void motor_test(void);
void motor_forward(void);
void motor_backward(void);
void motor_left_forward(void);
void motor_right_forward(void);
void motor_left_backward(void);
void motor_right_backward(void);
void motor_left_turn(void);
void motor_right_turn(void);


#endif /* MOTOR_H_ */