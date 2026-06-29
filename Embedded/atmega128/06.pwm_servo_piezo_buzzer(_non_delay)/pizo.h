/*
 * pizo.h
 *
 * Created: 2026-06-25 오후 2:57:38
 *  Author: kccistc
 */ 


#ifndef PIZO_H_
#define PIZO_H_

int pizo_example(void);
void power_on_melody(void);
void open_buzzer(void);
void drum_waching_stop_melody(void);
void stand_by(void);

extern int power_on_melody_play;
extern int open_buzzer_play;
extern int drum_waching_stop_melody_play;

extern void (*pizofunc[])();


#endif /* PIZO_H_ */