/*
 * menu.h
 *
 * Created: 2026-06-22 오후 2:15:50
 *  Author: kccistc
 */ 


#ifndef MENU_H_
#define MENU_H_

#define STANDARD_VALUE 200
#define MANUAL_MODE		0
#define AUTO_MODE		1
#define AUTO_MODE_CHECK 2
//#define DISTANCE_CHECK	3

extern volatile uint8_t bt_data;
extern volatile int func_state;
extern void (*pfunc[])();

void manual_mode(void);
void auto_mode(void);
void auto_mode_check(void);
void distance_check(void);



#endif /* MENU_H_ */