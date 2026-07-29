#include "device_driver.h"
#include <stdio.h>

#define B_0 82
#define B_1 38
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LUMEN 35


static void Sys_Init(int baud)
{
	SCB->CPACR |= (0x3 << 10 * 2) | (0x3 << 11 * 2);
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

/*
하나 보내보자 GRB 순서대로 보내야되니까
GRB
G = 0 ~ 255  0x00XXXX ~ 0xFF0000
R = 0 ~ 255  0x0000XX ~ 0x00FF00
B = 0 ~ 255  0x000000 ~ 0x0000FF
4개 달려있어서 GRB/GRB/GRB/GRB 보내고 50us 로우신호 발송.
타이머로 가변 주파수를 뽑아주는게 좋을 거 같은데? 아닌가?
0이 되려면 high가 32퍼 1이 되려면 low가 32퍼
두개 합치면 1.25us 주파수는? (1.25*10^-6) * (8 * 10^5) = 1
800KHz가 나온다.
**********************************PC6으로 AF02 TIM3_CH1
*/

void __TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); // GPIOC IN
	Macro_Set_Bit(RCC->APB1ENR, 1); // TIMER3 IN

	// 하이 시작이 문제인듯? gpio로 먼저 로우 뽑아
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x1, 12);
	Macro_Clear_Bit(GPIOC->ODR, 6);

	// PC6으로 데이터 보내 줄 생각임.
	Macro_Write_Block(GPIOC->AFR[0], 0xf, 0x2, 24); // PC6 => AF02
	Macro_Write_Block(TIM3->CCMR1, 0xff, 0x78, 0);	// 1CH ON PWM2 repeat pwm1이랑 파형이 반대로 나옴
	TIM3->CCER = 0;									// 출력을 저어어어맨뒤로빼야될듯?
}

void BIG__EYE__TIM3_PWM_INIT(void)
{
	TIM3->CR1 = 0;
	TIM3->PSC = 0;
	// 요청한 주파수가 되도록 ARR 설정
	// 여기가 값을 입력해줘서 1.25us를 만들어야됨 100이 1.25us라면? 1틱은? 0.0125us
	// 0.0125us의 주파수는?
	// 그럼 0.125us 로 10틱으로 가자 1.25us가 800khz 였으니까 8mhz
	// 96mhz / 8mhz
	TIM3->ARR = 120;  // 걍 분주하지말고 쪼개
	TIM3->CCR1 = 120; // 처음에는 아무것도 없어야되
	// Manual Update(UG 발생)
	TIM3->EGR = (1 << 0);
	// Down Counter, Repeat Mode
	TIM3->CR1 = (1 << 4 | 0 << 3);
	TIM3->CCER = (1 << 0);						   // pc6핀 출력 나가잇 근데 막상 만들어놓고 보니까 의미없는듯.? 앞에서 출력해줘도 될듯?
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x2, 12); // PC6 => ALT
}

void BIG__EYE__TIM3_interrupt_en(int en)
{
	if (en)
	{
		// TIM3 Pending Clear
		Macro_Clear_Bit(TIM3->SR, 0);
		// NVIC Pending Clear
		NVIC_ClearPendingIRQ(29);
		// TIM4 Interrupt Enable
		Macro_Set_Bit(TIM3->DIER, 0);
		// NVIC Interrupt Enable
		NVIC_EnableIRQ(29);
		// 마지막 확실하게 해주기 위해서 올리고 원위치
		Macro_Write_Block(GPIOC->MODER, 0x3, 0x1, 12);
		Macro_Set_Bit(GPIOC->ODR, 6);
		Macro_Write_Block(GPIOC->MODER, 0x3, 0x2, 12); // PC6 => ALT
	}
	else
	{
		NVIC_DisableIRQ(29);
		Macro_Clear_Bit(TIM3->CR1, 0);
		Macro_Clear_Bit(TIM3->DIER, 0);
	}
}
void BIG__EYE__TIM3_PWM_START(void)
{
	TIM3->CR1 |= (1 << 0); // 스타트는 여기서켜
}
void BIG__EYE__TIM3_PWM_STOP(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

volatile int _GRB[] = {
	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	
	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,

	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,

	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,

	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,	B_0,
	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,	B_1,
};

/*
함수 하나 만들어보자
넘겨줄 인자.
_GRB[] 인트 배열
led_position : LED 위치 (0~3)
green        : 초록색 밝기 (0~255) 00000000 ~ 11111111
red          : 빨간색 밝기 (0~255) 00000000 ~ 11111111
blue         : 파란색 밝기 (0~255) 00000000 ~ 11111111

 */
void __SET_BIG_EYE_ELD(volatile int *_GRB,  int led_position, int green, int red, int blue)
{	
	// 값 배열에 넣어주는 좋은 친구
	volatile int sit;
	volatile int i;
	sit = led_position * 24; // 0번 0 1번 24 2번 48 3번 72
	// sit 시작 기준점 내가 통제할 led를 인자로 넘겨줬으니까
	// 그린은 앞에 8개 레드는 중간 8개 블루는 마지막 8개
	// 8를 하나씩????? 하나씩비교 어케?
	for (i = 0; i < 8; i++)
	{
		if (green & (0x80 >> i))
		{ // 맨처음에 제일 앞에꺼봐
			// 1이면?
			_GRB[sit + i] = B_1;
		}
		else
		{
			_GRB[sit + i] = B_0;
		}
	}
	for (i = 0; i < 8; i++)
	{
		if (red & (0x80 >> i))
		{ // 맨처음에 제일 앞에꺼봐
			// 1이면?
			_GRB[sit + i + 8] = B_1;
		}
		else
		{
			_GRB[sit + i + 8] = B_0;
		}
	}
	for (i = 0; i < 8; i++)
	{
		if (blue & (0x80 >> i))
		{ // 맨처음에 제일 앞에꺼봐
			// 1이면?
			_GRB[sit + i + 16] = B_1;
		}
		else
		{
			_GRB[sit + i + 16] = B_0;
		}
	}
}

volatile int i, j, data;
volatile int _tim3_sr = 0;
volatile int _zero_count = 0;
volatile int _one_count = 0;
volatile int __check = 1;
volatile int start_trigger = 0;
volatile int reset_trigger = 0;
volatile int green_color = 0;
volatile int red_color = 0;
volatile int blue_color = LUMEN;
volatile int led_set = 0;
volatile int v_res = 0;
volatile int _change_color = 1;
volatile int spi_gpio = 0;
volatile int Key_Pressed = 0;
volatile int spi_gpio_check = 0;


void karaoke_mirror_ball()	// 한번에 1led만 젲어
{
	// 0.1초마다 색깔 바꿔보기. 어케 바꾸??
	// ADC에서 읽어온 값을 바꿔.
	ADC1_Start();
	ADC1_Get_Status();
	TIM4->ARR = ADC1_Get_Data(); // 50 * 100 -1 로 시작 4999로 시작한거임.
	if (_change_color == 1)
	{
		__SET_BIG_EYE_ELD(_GRB, led_set, green_color, red_color, blue_color);
		led_set++;
		if (green_color == LUMEN)
		{
			red_color = LUMEN;
			green_color = 0;
		}
		else if (red_color == LUMEN)
		{
			red_color = 0;
			blue_color = LUMEN;
		}
		else if (blue_color == LUMEN)
		{
			blue_color = 0;
			green_color = LUMEN;
		}
		led_set = led_set % 4;
		_change_color = 0;
	}
	if (Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
	{
		// 무적권 다 쓰고 인터럽트 호출 후 싲가
		BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
		BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
		Macro_Clear_Bit(TIM4->SR, 0);
		_change_color = 1;
	}
}

void circular_led(void)	// 한번에 4놈다 바꾸기
{
	// 정해진 시간마다 모든 색깔 바꿔보기 기존 코드는 한번에 하나만 바꿨다.
	// ADC에서 읽어온 값을 바꿔.
	static int led_0;	static int led_1;	static int led_2;	static int led_3;
	led_0 = 0; led_1 = 1; led_2 = 2; led_3 = 3;
	static int green_0 = LUMEN;	static int red_0 = 0; 		static int blue_0 = 0;
	static int green_1 = 0;	 	static int red_1 = LUMEN;	static int blue_1 = 0;
	static int green_2 = 0; 	static int red_2 = 0; 		static int blue_2 = LUMEN;
	static int green_3 = LUMEN; static int red_3 = LUMEN; 	static int blue_3 = 0;
	ADC1_Start();
	ADC1_Get_Status();	// 최소 50 ~ 최대 3800정도나옴
	TIM4->ARR = ADC1_Get_Data() * 5; // 50 * 100 -1 로 시작 4999로 시작한거임.
	if (_change_color == 1)
	{
		__SET_BIG_EYE_ELD(_GRB, led_0, green_0, red_0, blue_0);
		__SET_BIG_EYE_ELD(_GRB, led_1, green_1, red_1, blue_1);
		__SET_BIG_EYE_ELD(_GRB, led_2, green_2, red_2, blue_2);
		__SET_BIG_EYE_ELD(_GRB, led_3, green_3, red_3, blue_3);
		led_0++; led_1++; led_2++;  led_3++;
		if (green_0 == LUMEN){	red_0 = LUMEN; 	green_0 = 0;	}
		else if (red_0 == LUMEN){	red_0 = 0;	blue_0 = LUMEN;	}
		else if (blue_0 == LUMEN){	blue_0 = 0;	green_0 = LUMEN;}

		if (green_1 == LUMEN){	red_1 = LUMEN; 	green_1 = 0;	}
		else if (red_1 == LUMEN){	red_1 = 0;	blue_1 = LUMEN;	}
		else if (blue_1 == LUMEN){	blue_1 = 0;	green_1 = LUMEN;}

		if (green_2 == LUMEN){	red_2 = LUMEN; 	green_2 = 0;	}
		else if (red_2 == LUMEN){	red_2 = 0;	blue_2 = LUMEN;	}
		else if (blue_2 == LUMEN){	blue_2 = 0;	green_2 = LUMEN;}

		if (green_3 == LUMEN){	red_3 = LUMEN; 	green_3 = 0;	}
		else if (red_3 == LUMEN){	red_3 = 0;	blue_3 = LUMEN;	}
		else if (blue_3 == LUMEN){	blue_3 = 0;	green_3 = LUMEN;}

		_change_color = 0;
	}
	if (Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
	{
		// 무적권 다 쓰고 인터럽트 호출 후 싲가
		BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
		BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
		Macro_Clear_Bit(TIM4->SR, 0);
		_change_color = 1;
	}
}
void random_color(int value, int* green_0, int* green_1, int* green_2, int* green_3,
					int* red_0, int* red_1, int* red_2, int* red_3, 
					int* blue_0, int* blue_1, int* blue_2, int* blue_3){
	int selve = 60;
	*green_0 = (*green_0 + value * 2) % selve;
	*green_1 = (*green_1  + value * 7) % selve; 
	*green_2 = (*green_2 + value * 17) % selve; 
	*green_3 = (*green_3 + value * 29) % selve;
	*red_0 = (*red_0 + value * 3) % selve; 
	*red_1 = (*red_1 + value * 11) % selve;
	*red_2 = (*red_2 + value * 19) % selve; 
	*red_3 = (*red_3 + value * 31) % selve;
	*blue_0 = (*blue_0 + value * 5) % selve; 
	*blue_1 = (*blue_1 + value * 13) % selve;
	*blue_2 = (*blue_2 + value * 23) % selve; 
	*blue_3 = (*blue_3 + value * 37) % selve;
		
}
void full_color_led(void)	// 3원색이 아니라 모든색 번갈아가면서 나오게 해보자
{
	// 정해진 시간마다 모든 색깔 바꿔보기 기존 코드는 한번에 하나만 바꿨다.
	// ADC에서 읽어온 값을 바꿔.
	static int led_0;	static int led_1;	static int led_2;	static int led_3;
	led_0 = 0; led_1 = 1; led_2 = 2; led_3 = 3;
	static int green_0 = 0;		static int red_0 = 0;		static int blue_0 = 0;
	static int green_1 = 0;		static int red_1 = 0;		static int blue_1 = 0;
	static int green_2 = 0;		static int red_2 = 0;		static int blue_2 = 0;
	static int green_3 = 0;		static int red_3 = 0;		static int blue_3 = 0;
	ADC1_Start();
	ADC1_Get_Status();	// 최소 50 ~ 최대 3800정도나옴
	TIM4->ARR = ADC1_Get_Data() * 5; // 50 * 100 -1 로 시작 4999로 시작한거임.
	if (_change_color == 1)
	{
		__SET_BIG_EYE_ELD(_GRB, led_0, green_0, red_0, blue_0);
		__SET_BIG_EYE_ELD(_GRB, led_1, green_1, red_1, blue_1);
		__SET_BIG_EYE_ELD(_GRB, led_2, green_2, red_2, blue_2);
		__SET_BIG_EYE_ELD(_GRB, led_3, green_3, red_3, blue_3);
		// 색변화를 하나씩 올려주고싶은거임
		random_color(7, &green_0, &green_1, &green_2, &green_3, 
						&red_0, &red_1, &red_2, &red_3, 
						&blue_0, &blue_1, &blue_2, &blue_3);

		_change_color = 0;
	}
	if (Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
	{
		// 무적권 다 쓰고 인터럽트 호출 후 싲가
		BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
		BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
		Macro_Clear_Bit(TIM4->SR, 0);
		_change_color = 1;
	}
}

void blue_team_led(void){
	// 0.1초마다 색깔 바꿔보기. 어케 바꾸??
	// ADC에서 읽어온 값을 바꿔. // 퍼랭이만 나오게
	ADC1_Start();
	ADC1_Get_Status();
	TIM4->ARR = ADC1_Get_Data(); // 50 * 100 -1 로 시작 4999로 시작한거임.
	static int blue_0 = LUMEN; static int blue_1 = 0; static int blue_2 = 0; static int blue_3 = 0;
	if (_change_color == 1){	
		__SET_BIG_EYE_ELD(_GRB, 0, 0, 0, blue_0);
		__SET_BIG_EYE_ELD(_GRB, 1, 0, 0, blue_1);
		__SET_BIG_EYE_ELD(_GRB, 2, 0, 0, blue_2);
		__SET_BIG_EYE_ELD(_GRB, 3, 0, 0, blue_3);
		if(blue_0 == LUMEN){blue_0 = 0; blue_1 = LUMEN;	}
		else if(blue_1 == LUMEN){blue_1 = 0; blue_2 = LUMEN;}
		else if(blue_2 == LUMEN){blue_2 = 0; blue_3 = LUMEN;}
		else if(blue_3 == LUMEN){blue_3 = 0; blue_0 = LUMEN;}
		
		_change_color = 0;
	}
	if(Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
	{
		// 무적권 다 쓰고 인터럽트 호출 후 싲가
		BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
		BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
		Macro_Clear_Bit(TIM4->SR, 0);
		_change_color = 1;
	}
}

void red_team_led(void){
	// 0.1초마다 색깔 바꿔보기. 어케 바꾸??
	// ADC에서 읽어온 값을 바꿔. // 빨갱이만 나오게
	ADC1_Start();
	ADC1_Get_Status();
	TIM4->ARR = ADC1_Get_Data(); // 50 * 100 -1 로 시작 4999로 시작한거임.
	static int red_0 = LUMEN; static int red_1 = 0; static int red_2 = 0; static int red_3 = 0;
	if (_change_color == 1){	
		__SET_BIG_EYE_ELD(_GRB, 0, 0, red_3, 0);
		__SET_BIG_EYE_ELD(_GRB, 1, 0, red_2, 0);
		__SET_BIG_EYE_ELD(_GRB, 2, 0, red_1, 0);
		__SET_BIG_EYE_ELD(_GRB, 3, 0, red_0, 0);
		if(red_0 == LUMEN){red_0 = 0; red_1 = LUMEN;	}
		else if(red_1 == LUMEN){red_1 = 0; red_2 = LUMEN;}
		else if(red_2 == LUMEN){red_2 = 0; red_3 = LUMEN;}
		else if(red_3 == LUMEN){red_3 = 0; red_0 = LUMEN;}
		
		_change_color = 0;
	}
	if(Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
	{
		// 무적권 다 쓰고 인터럽트 호출 후 싲가
		BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
		BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
		Macro_Clear_Bit(TIM4->SR, 0);
		_change_color = 1;
	}
}
#define TIM2_TICK	  		(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK) // Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)


void TIM2_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(0<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time - 1; 	// 50 * 시간 -1임사실

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
}
void right_spi_gpio(volatile int *spi_gpio){
	(*spi_gpio)++;
	if(*spi_gpio == 8){
		return ;	
	}
	if(*spi_gpio > 7){
		*spi_gpio = 0;
	}
}

void _reverse_spi_gpio(volatile int *spi_gpio){
	(*spi_gpio)--;
	if(*spi_gpio == 0){
		return ;	
	}
	if(*spi_gpio < 0){
		*spi_gpio = 8;
	}
}

void ping_pong_game(void){
		TIM2->ARR = (ADC1_Get_Data() * 1) % 65536 ; // 난이도 조절
		//버튼 pc7
		if(Key_Pressed)
		{
			//printf("KEY Pressed!!!\n");
			Key_Pressed = 0;
			spi_gpio_check ^= 1;
		}

		int reverse_spi_gpio = 0;
		if (Macro_Extract_Area(TIM2->SR, 0x1, 0) == 1){
			Macro_Clear_Bit(TIM2->SR, 0);
			// 흐름 01234567654321 이렇게 가고싶음
			if(spi_gpio_check == 0){
				right_spi_gpio(&spi_gpio);
			}else{
				_reverse_spi_gpio(&spi_gpio);	
			}
		}
			data = ~(1u << spi_gpio);
			SPI1_SC16IS752_Write_GPIO(data);

			if(spi_gpio_check == 0){
				blue_team_led();
				//karaoke_mirror_ball();
			}else{
				red_team_led();
				//full_color_led();
			}
	
}
void Main(void)
{
	Sys_Init(115200);
	printf("ADC Test\n");

	__TIM3_Out_Init();
	BIG__EYE__TIM3_PWM_INIT();
	TIM4_Repeat(100); // 시작신호 줄 친구 좋은 친구
	TIM2_Repeat(100); // spi led 바꿔줄 좋은 친구
	ADC1_IN6_Init();	
	SPI1_SC16IS752_Init(32); 			
	SPI1_SC16IS752_Config_GPIO(0xFF);
	Key_ISR_Enable(1);


#if 0
	for(;;)
	{
		ADC1_Start();
		//while(!ADC1_Get_Status());
		ADC1_Get_Status();
		//printf("0x%.4X\n", ADC1_Get_Data());
		printf("%d\n", ADC1_Get_Data());	// 50 ~ 3700 사이값임
		for(i=0; i<0x100000; i++);
	}
#else
#if 1
	#if 0
	for (;;)
	// 노래방 두가자
	{
		karaoke_mirror_ball();
		//circular_led();
		//full_color_led();

	}
	#else
	for(;;){
		ping_pong_game();
	}
	#endif
#else
	for (;;)	// 테스트용 잘돌아가는지
	{
		__SET_BIG_EYE_ELD(_GRB, 0, 0, 100, 0);
		__SET_BIG_EYE_ELD(_GRB, 1, 50, 100, 0);
		__SET_BIG_EYE_ELD(_GRB, 2, 100, 100, 0);
		__SET_BIG_EYE_ELD(_GRB, 3, 180, 100, 0);

		if (Macro_Extract_Area(TIM4->SR, 0x1, 0) == 1)
		{
			BIG__EYE__TIM3_interrupt_en(1); // 임마가 내가 통제할 __GRB 할때마다 켜줘야됨 자동으로 한번보내고 꺼버림.
			BIG__EYE__TIM3_PWM_START();		// 야 둘이는 한세트
			Macro_Clear_Bit(TIM4->SR, 0);
		}
	}
#endif
#endif
}
