/*
 * 07.MY_LED_PWM_CONTROL.c
 *
 * Created: 2026-06-18 오전 10:24:52
 * Author : kccistc
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define LED_TIME 20
/* Replace with your library code */
int main(void)
{
	DDRA = 0xff;
	
	int dim = 0;
	int direction = 1;
	
	while(1){
		turn_on_LED_in_PWM_manner(dim);
		
		dim += direction;
		
		if(dim == 0)direction = 1;
		if(dim == 255)direction = -1;	
	}
	return 0;
}

void turn_on_LED_in_PWM_manner(int dim){
	int i;
	
	PORTA = 0xff;
	
	for(i = 0; i < 256; i++){
		if(i>dim) PORTA = 0x00;
		_delay_us(LED_TIME);
	}
}

