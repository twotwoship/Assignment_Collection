/*
 * heaterController.h
 *
 *  Created on: Oct 20, 2025
 *      Author: dhksw
 */

#ifndef INC_HEATERCONTROLLER_H_
#define INC_HEATERCONTROLLER_H_

#include "main.h"
#include "controlType.h"
#include "defines.h"
#include "ledcontoroller.h"


uint8_t getHeaterState();
void heaterOnOff(uint8_t onOff);

void temper_up();
void temper_down();
void setFixedTemper();
int getFixedTemper();

void heaterControl(float temper);



#endif /* INC_HEATERCONTROLLER_H_ */
