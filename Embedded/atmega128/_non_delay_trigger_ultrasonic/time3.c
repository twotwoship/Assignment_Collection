/*
 * time3.c
 *
 * Created: 2026-06-22 오후 6:09:36
 *  Author: kccistc
 */ 

#include "time3.h"

void init_time3(void){
	TCCR3B |= 1 << CS12 | 1 << CS10;	// 분주비 1024설정 p318 표14-1, 분주비를 설정하면 타이머가 켜진다.

}