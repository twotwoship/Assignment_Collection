used function 

Ignore the comments.


```c
	while(1){
		_non_delay_left_make_trigger_processing();
	}
```


```c
void _non_delay_left_make_trigger_processing(void){
	if(high_left_pulse == 0 && low_left_pulse == 0 && _non_delay_left_trigger_wait == 0){	
		_non_delay_left_make_trigger_high();
		_non_delay_left_trigger_wait = _us_count;
	}
	if(_non_delay_left_trigger_wait != 0){
		if ((_us_count - _non_delay_left_trigger_wait) >= 1)
		{
			_non_delay_left_make_trigger_low();
		}
	}
	if(high_left_pulse == 1 && low_left_pulse == 1){
		high_left_pulse = 0;
		low_left_pulse = 0;
		_non_delay_left_trigger_wait = 0;
	}
}
```

```c
ISR(TIMER2_OVF_vect){		// 16us timer
	TCNT2 = 0;				// 0.016/256 카운터 ㅋㅋ 1tik 0.625us
	_us_count++;			//  16us timer 
	if(_us_count> _US_COUNT_RESET){
		_us_count= 1;		
	}
}
```

```c
ISR(INT4_vect){	/
	if(ECHO_LAFT_PORT & ( 1 << ECHO_LAFT_PIN)){	// 1. high edge
		left_count = _us_count;	
		high_left_pulse = 1;
	}else{	// 2. low edge
		low_left_pulse = 1;
		_non_delay_left_trigger_wait = 0;
	}
}
```
