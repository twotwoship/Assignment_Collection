/*
 * ledcontoroller.c
 *
 *  Created on: Nov 7, 2025
 *      Author: dhksw
 */


#include "ledcontoroller.h"

void led1OnOff(ON_OFF_t on_off){

	if(on_off == ON_t){
		HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 0);

	}else if(on_off == OFF_t){
		HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 1);
	}
}

void led2OnOff(ON_OFF_t on_off){

	if(on_off == ON_t){
		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 0);

	}else if(on_off == OFF_t){
		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);
	}
}
