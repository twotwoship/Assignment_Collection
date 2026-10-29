/*
 * buttonmaster.c
 *
 *  Created on: Nov 6, 2025
 *      Author: dhksw
 */


#include "buttonmaster.h"
#include "oledcontoroller.h"
#include "heaterController.h"
int count_1 = 0;


void checkButton(){

	  if(g_f_sw_up){
		  printf("push\r\n");
		  g_f_sw_up = 0;

		  temper_up();

	  }
	  if(g_f_sw_down){
		  printf("__down\r\n");
		  g_f_sw_down = 0;

		  temper_down();
	  }
	  if(g_f_sw_fix){
		  printf("fix__\r\n");
		  g_f_sw_fix = 0;
		  HAL_GPIO_TogglePin(PB5_RELAY_ON_OFF_CTRL_GPIO_Port, PB5_RELAY_ON_OFF_CTRL_Pin);
		  startToggle();
		  setFixedTemper();
	  }
	  if(g_f_sw_on){
		  printf("___________on___________\r\n");
		  g_f_sw_on = 0;
	  }
}


ON_OFF_t getSwState(){
	if(HAL_GPIO_ReadPin(PB12_START_SW_PIN_GPIO_Port, PB12_START_SW_PIN_Pin)){
		return OFF_t;
	}else{
		return ON_t;
	}

}
