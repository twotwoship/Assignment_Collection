/*
 * buttonmaster.h
 *
 *  Created on: Nov 6, 2025
 *      Author: dhksw
 */

#ifndef INC_BUTTONMASTER_H_
#define INC_BUTTONMASTER_H_

#include "ex_var.h"
#include "main.h"
#include <stdio.h>
#include "controlType.h"
#include "heaterController.h"



void checkButton();
ON_OFF_t getSwState();

#endif /* INC_BUTTONMASTER_H_ */
