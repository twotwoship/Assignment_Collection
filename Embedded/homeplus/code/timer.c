#include "device_driver.h"
#include "timer.h"

static volatile int timer_wait_done;

/*
 * Shared 1 MHz, 32-bit time base for sensor state machines.
 * SysTick is deliberately not touched because it is owned by the UART module.
 */
void Sensor_Timer_Init(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);
	TIM2->CR1 = 0;
	TIM2->PSC = (TIMXCLK / 1000000U) - 1U;
	TIM2->ARR = 0xffffffffU;
	TIM2->CNT = 0;
	TIM2->DIER = 0;
	TIM2->SR = 0;
	TIM2->EGR = 1U;
	TIM2->CR1 = 1U;
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
}

uint32_t Timer2_Micros(void)
{
	return TIM2->CNT;
}

static void Timer2_Arm(unsigned int channel, uint32_t delay_us)
{
	volatile uint32_t *ccr = &TIM2->CCR1 + channel;
	uint32_t flag = 1U << (channel + 1U);
	*ccr = TIM2->CNT + delay_us;
	TIM2->SR = ~flag;
	TIM2->DIER |= flag;
}

static void Timer2_Cancel(unsigned int channel)
{
	uint32_t flag = 1U << (channel + 1U);
	TIM2->DIER &= ~flag;
	TIM2->SR = ~flag;
}

void Timer2_Arm_CC1(uint32_t us) { Timer2_Arm(0U, us); }
void Timer2_Arm_CC2(uint32_t us) { Timer2_Arm(1U, us); }
void Timer2_Arm_CC3(uint32_t us) { Timer2_Arm(2U, us); }
void Timer2_Cancel_CC1(void) { Timer2_Cancel(0U); }
void Timer2_Cancel_CC2(void) { Timer2_Cancel(1U); }
void Timer2_Cancel_CC3(void) { Timer2_Cancel(2U); }

void Timer2_Timeout_Start(uint32_t delay_us)
{
	if (delay_us == 0U) delay_us = 1U;
	timer_wait_done = 0;
	Timer2_Arm(3U, delay_us);
}

void Timer2_Timeout_Cancel(void)
{
	Timer2_Cancel(3U);
}

int Timer2_Timeout_Expired(void)
{
	return timer_wait_done;
}

__attribute__((weak)) void Timer2_CC1_Callback(void) { }
__attribute__((weak)) void Timer2_CC2_Callback(void) { }
__attribute__((weak)) void Timer2_CC3_Callback(void) { }

void TIM2_IRQHandler(void)
{
	uint32_t pending = TIM2->SR & TIM2->DIER;
	if (pending & (1U << 1)) {
		Timer2_Cancel_CC1();
		Timer2_CC1_Callback();
	}
	if (pending & (1U << 2)) {
		Timer2_Cancel_CC2();
		Timer2_CC2_Callback();
	}
	if (pending & (1U << 3)) {
		Timer2_Cancel_CC3();
		Timer2_CC3_Callback();
	}
	if (pending & (1U << 4)) {
		Timer2_Cancel(3U);
		timer_wait_done = 1;
	}
}

void Timer_Wait_Ms(uint32_t delay_ms)
{
	if (delay_ms == 0U) return;
	Timer2_Timeout_Start(delay_ms * 1000U);
	while (!timer_wait_done) __WFI();
}