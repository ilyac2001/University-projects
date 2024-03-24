#include "timer.h"

void TimerInit(){	
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;	// Тактирование TIM3
	TIM16->PSC = psc - 1;               	
  TIM16->ARR = arr - 1;               	
  TIM16->DIER |= TIM_DIER_UIE;        	// Update прерывания
	NVIC_EnableIRQ(TIM16_IRQn);         	
	NVIC_SetPriority(TIM16_IRQn, 0);    	// Приоритет прерываний
	TIM16->CR1 |= TIM_CR1_CEN;          	// Запустить
}
