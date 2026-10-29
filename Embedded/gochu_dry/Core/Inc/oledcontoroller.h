/*
 * oledcontoroller.h
 *
 *  Created on: Nov 6, 2025
 *      Author: dhksw
 */

#ifndef INC_OLEDCONTOROLLER_H_
#define INC_OLEDCONTOROLLER_H_

#include "ssd1306.h"
#include <stdio.h>
#include "heaterController.h"
#include "controlType.h"

void opening();
void printDefault();
void printTemper(int Temper);
void toggleScreen();
void startToggle();
void printHeaterState(ON_OFF_t onoff);
#endif /* INC_OLEDCONTOROLLER_H_ */
