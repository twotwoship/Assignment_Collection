/*
* keypad_cal_lcd.c
*
* Created: 2026-07-02 오전 10:39:29
*  Author: kccistc
*/

#include "keypad_cal_lcd.h"

volatile int run_calculation = 0;
volatile int cal_buff_count = 0;
volatile int reset_count = 0;
char* cal_inbuff  = 0;
volatile int calculation_done = 0;

void calculator_text_lcd_Synchronization(t_ds1307 *ds1307){ //인자 넘기기만하고 쓰지마 나중에 고민해보기 다른인자를 받는경우

	// 계산 완료 화면
	if(calculation_done){
		if(button4_flag){
			button4_flag = 0;
			calculation_done = 0;
			reset_count = 0;
			func_state = WATCH_MODE;
			_first = 1;
			top = -1;
			i_top = -1;
			ocb = 0;
			ccb = 0;
			run_calculation = 0;
			cal_buff_count = 0;

			if(cal_inbuff){
				free(cal_inbuff);
				cal_inbuff = 0;
			}

			lcd_clear();
			return;
		}
		return;    // 결과 화면 유지
	}
	
	// 계산 도중 종료시
	if(button4_flag){
		button4_flag = 0;
		if(func_state == CALCULATE_MODE){
			calculation_done = 0;
			reset_count = 0;
			func_state = WATCH_MODE;
			_first = 1;
			top = -1;
			i_top = -1;
			ocb = 0;
			ccb = 0;
			run_calculation = 0;
			cal_buff_count = 0;

			if(cal_inbuff){
				free(cal_inbuff);
				cal_inbuff = 0;
			}

			lcd_clear();
			return;
		}
	}

	
	// 10초이상 가만히 잇으면 종료 왜 5초만에 꺼지지?

	if(reset_count > 30){
		reset_count = 0;
		cal_reset_ms_count = 0;
		func_state  = WATCH_MODE;
		free(cal_inbuff);
		cal_buff_count = 0;
		lcd_clear();
		_first = 1;

		return;
	}
	if(_first){
		cal_inbuff = (char*)calloc(MAX, sizeof(char));
		cal_buff_count = 0;
		lcd_clear();
		_first = 0;
		lcd_goto_xy(0,0);
		lcd_write_string("Calculator");
		lcd_goto_xy(1,0);
		lcd_write_string("Mode");
	}

	
	//끝내고 꼭 프리해주기
	
	// 중위계산식 입력버퍼를 어떻게 출력할까
	// keydata가 하나 들어오면 어떻게? 출력을?
	//keydata // 임마를 넣어줘야됨.
	if(button0_flag){
		button0_flag=0;
		keydata = '(';
		if(keydata){
			reset_count  = 0;
			cal_inbuff[cal_buff_count] = keydata;
			cal_buff_count++;
			cal_inbuff[cal_buff_count] = '\0';
			lcd_goto_xy(0, 0);
			lcd_write_cal_window(cal_inbuff,cal_buff_count);
		}
	}
	if(button1_flag){
		button1_flag=0;
		keydata = ')';
		if(keydata){
			reset_count  = 0;
			cal_inbuff[cal_buff_count] = keydata;
			cal_buff_count++;
			cal_inbuff[cal_buff_count] = '\0';
			lcd_goto_xy(0, 0);
			lcd_write_cal_window(cal_inbuff,cal_buff_count);
		}
	}
	if(button2_flag){
		button2_flag = 0;

		if(cal_buff_count > 0)
		{
			cal_buff_count--;
			cal_inbuff[cal_buff_count] = '\0';

			lcd_goto_xy(0,0);
			lcd_write_cal_window(cal_inbuff, cal_buff_count);
		}
	}
	if(keypad_scan_flag){
		keypad_scan_flag = 0;
		keydata = keypad_scan();
		if(keydata){
			reset_count  = 0;
			cal_inbuff[cal_buff_count] = keydata;
			cal_buff_count++;
			cal_inbuff[cal_buff_count] = '\0';
			lcd_goto_xy(0, 0);
			lcd_write_cal_window(cal_inbuff,cal_buff_count);
			if(keydata == '=')
			{
				run_calculation = 1;
			}
		}
	}
	
	
	// = 들어오면 계산 바로 들어가.
	if(run_calculation){
		run_calculation--;
		calculation_done = 1;
		calc_error = 0;
		char* temp_buff = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 출력버퍼
		int* number_stack = (int*)calloc(MAX, sizeof(int)); // 후위 계산 할 때 쓰는 스택 // 이 스택용 pop push도 만들어야됨.
		char* operater_stack = (char*)calloc(MAX, sizeof(char)); // 후위계산식 만들 때 쓰는 스택		if(queue_empty() != TRUE){
			back_display_make(cal_inbuff, temp_buff, operater_stack); // 후위 계산식 만드는 함수
			
			// 예외처리
			if(calc_error)
			{
				calc_error = 0;
				top = -1;
				i_top = -1;
				ocb = 0;
				ccb = 0;
				_first = 1;

				run_calculation = 0;
				calculation_done = 1;

				lcd_clear();
				lcd_goto_xy(0,0);
				lcd_write_string("Equation Error");
				lcd_goto_xy(1,0);
				lcd_write_string("BTN4 : EXIT");

				free(temp_buff);
				free(number_stack);
				free(operater_stack);
				return;
			}

			char answer_buff[12];
			back_display_carculate(temp_buff, number_stack); // 후위 계산 함수
			
			// 예외처리
			if(calc_error)
			{
				calc_error = 0;
				top = -1;
				i_top = -1;
				ocb = 0;
				ccb = 0;
				_first = 1;

				run_calculation = 0;
				calculation_done = 1;

				lcd_clear();
				lcd_goto_xy(0,0);
				lcd_write_string("Equation Error");
				lcd_goto_xy(1,0);
				lcd_write_string("BTN4 : EXIT");

				free(temp_buff);
				free(number_stack);
				free(operater_stack);
				return;
			}
			
			// 결과
			if (ocb == ccb) {
				// 저 결과를 lcd에 출력해줘야됨.
				// 결과를 lcd에 출력해 주고 난 뒤에 결과값을 뽑아주고 다시 계산하고 싶으면 시계 다녀오기로 하게.
				//printf("%d\n", pop_i(number_stack));
				
				// 계산 완료되면 버퍼랑 변수들 초기화 시켜주기.
				cal_buff_count = 0;
				for(int i = 0; i < MAX; i++){
					cal_inbuff[i] = '\0';
				}
				lcd_goto_xy(1, 0);
				lcd_write_string("ans:");
				lcd_goto_xy(1, 4);
				itoa(pop_i(number_stack), answer_buff, 10);
				lcd_write_string(answer_buff);
				// 자동 종료될때 꼭 프리시켜주기
				top = -1;i_top = -1;	ocb = 0;ccb = 0;

				free(temp_buff);
				free(number_stack);
				free(operater_stack);
				free(cal_inbuff);
				cal_inbuff = 0;
				_first = 1;

				// 괄호 잘못넣엇을때.
				}else{
				top = -1;
				i_top = -1;
				ocb = 0;
				ccb = 0;
				_first = 1;

				run_calculation = 0;
				calculation_done = 1;

				lcd_clear();
				lcd_goto_xy(0,0);
				lcd_write_string("Equation Error");
				lcd_goto_xy(1,0);
				lcd_write_string("BTN4 : EXIT");

				free(temp_buff);
				free(number_stack);
				free(operater_stack);

				return;
			}
		}
	}
	