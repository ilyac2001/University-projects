#include "main.h"
/******************************************************************************
 * Переменные
******************************************************************************/
TypeFlag flag;
TypeBtnStatus btn;
static short int time_in_seconds = 0; //хранит значение обратного отсчёта в секундах (начально задано максимальное возможное 99 мин 59 сек)
/******************************************************************************
 * Name:	Опрос кнопок
 * Input:   -
 * Return:	-
 * Remark:  Результат работы - заполнение структуры TypeBtnStatus
******************************************************************************/
void BtnSurvey(){
	static unsigned char MemoryLongClick = BTN_STRUCT_MASK;
	static unsigned char PastClick 		 = BTN_STRUCT_MASK;
	static unsigned char CurrentClick 	 = BTN_STRUCT_MASK;

	static int CounterPress = BTN_TIMER_LONG_PRESS(2000);
	CurrentClick = BtnPort();
	const uint8_t SPEED = 20; //исвользуется для уменьшения таймера определения длительного нажатия кнопки, а именно ускоряет опрос
	if(BtnPress()){
		//хотя бы одна кнопка нажата
		if (!(--CounterPress))
		{
			btn.btn_reg_long_press = ~CurrentClick & BTN_STRUCT_MASK;
			MemoryLongClick |= btn.btn_reg_long_press;
			CounterPress = BTN_TIMER_LONG_PRESS(2000 / SPEED);
		}
	}
	else{
		// Ни одна кнопка не нажата
		CounterPress = BTN_TIMER_LONG_PRESS(2000);
		btn.btn_reg_short_press |= (CurrentClick & ~PastClick) & ~MemoryLongClick; //не нажата и была нажата и не была долго нажата
		MemoryLongClick = 0x0000;
	}
	PastClick = CurrentClick;
}
/******************************************************************************
 * Name:	Получить ЦИФРУ на индикаторе
 * Input:   цифра 0-9, устанавливать ли точку, разряд индикаторе (сек ед, сек дес, мин ед, мин дес)
 * Return:	-
 * Remark:  -
******************************************************************************/
void SetValueIndicator(uint8_t number, uint8_t dp, uint8_t slot)
{
	GPIOB->BSRR |= GPIO_BSRR_BR_0 | GPIO_BSRR_BR_1 | GPIO_BSRR_BR_2 | GPIO_BSRR_BR_10 | GPIO_BSRR_BR_11;
	GPIOA->BSRR |= GPIO_BSRR_BR_5 | GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7;
	GPIOB->BSRR |= GPIO_BSRR_BR_12 | GPIO_BSRR_BR_13 | GPIO_BSRR_BR_14 | GPIO_BSRR_BR_15;
	switch(number)
	{
		case 0:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 1:
			GPIOA->BSRR |= GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 2:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6;
			break;
		case 3:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 4:
			GPIOB->BSRR |= GPIO_BSRR_BS_2 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 5:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_2 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_7;
			break;
		case 6:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_7;
			break;
		case 7:
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 8:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1 | GPIO_BSRR_BS_2 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		case 9:
			GPIOB->BSRR |= GPIO_BSRR_BS_0 | GPIO_BSRR_BS_2 | GPIO_BSRR_BS_10;
			GPIOA->BSRR |= GPIO_BSRR_BS_5 | GPIO_BSRR_BS_6 | GPIO_BSRR_BS_7;
			break;
		default:
			break;
	}
	if(dp)
	{
		GPIOB->BSRR |= GPIO_BSRR_BS_11;
	}
	switch(slot)
	{
		case SEC_UNITS:
			GPIOB->BSRR |= GPIO_BSRR_BS_15;
			break;
		case SEC_TENS:
			GPIOB->BSRR |= GPIO_BSRR_BS_14;
			break;
		case MIN_UNITS:
			GPIOB->BSRR |= GPIO_BSRR_BS_13;
			break;
		case MIN_TENS:
			GPIOB->BSRR |= GPIO_BSRR_BS_12;
			break;
		default:
			break;
	}
}
/******************************************************************************
 * Name:	Получить ЧИСЛО на индикаторе
 * Input:   четырехзначное число 0000-9999
 * Return:	-
 * Remark:  желательно вызывать с частотой 250 Гц
******************************************************************************/
void SetNumberIndicator(short int number)
{
	static uint8_t value = 0;
	static uint8_t slot = SEC_UNITS;
	static unsigned char dot = FALSE;
	switch(slot)
	{
		case SEC_UNITS:
			value = (number / 1) % 10;
			break;
		case SEC_TENS:
			value = (number / 10) % 10;
			break;
		case MIN_UNITS:
			value = (number / 100) % 10;
			dot = TRUE;
			break;
		case MIN_TENS:
			value = (number / 1000) % 10;
			break;
		default:
			value = 10;
			slot = SEC_UNITS;
			return;
	}
	SetValueIndicator(value, dot, slot);
	dot = FALSE;
	slot <<= 1;
}
/******************************************************************************
 * Name:	Преобазовать значение времени в секундах в формат минуты:секунды
 * Input:   время обратного отсчёта в секундах
 * Return:	четырехзначное число обратного отсчёта в формате минуты (старшие 2 разряда).секунды (младшие)
 * Remark:  используется для вывода на индикатор
******************************************************************************/
short int ConvertToTimeFormat(short int time_in_seconds)
{
	int minutes = time_in_seconds / 60;
	int seconds = time_in_seconds % 60;
	return (minutes * 100) + seconds;
}
/******************************************************************************
 * Name:	Вычислить шаг дискретизации таймера
 * Input:   -
 * Return:	значение на которое нужно уменьшить или увеличить время для таймера
 * Remark:  шаг < 5 минут - 1 сек, 5-10 минут - 30 сек, > 10 минут - 1 минута
******************************************************************************/
uint8_t GetSamplingStep()
{
	if(time_in_seconds < 5*60)
	{
		return 1;
	}
	else if ((time_in_seconds >= 5*60) & (time_in_seconds < 10*60))
	{
		return 30;
	}
	else if(time_in_seconds >= 10*60)
	{
		return 60;
	}
	return 0;
}
/******************************************************************************
* Обработчик прерывания TIM16 (250 Гц). Приоритет 0
*
******************************************************************************/
void TIM16_IRQHandler(void){
	TIM16->SR &= ~TIM_SR_UIF;
	static uint8_t counter = NUMBER_INTERRUPTS_PER_SECOND;
	static uint8_t counter_for_timeout_time_buzzer_work = 0;
	BtnSurvey();
	if(!(--counter))
	{
		if((time_in_seconds > 0) & flag.countdown)
		{
			time_in_seconds--;
		}
		else if((time_in_seconds == 0) & flag.countdown)
		{
			flag.countdown = FALSE;
			WORK_STOP();
			flag.timeout = TRUE;
			counter_for_timeout_time_buzzer_work = 0;
		}
		else if(flag.timeout)
		{
			if(counter_for_timeout_time_buzzer_work == 21)
			{
				BUZZER_OFF();
			}
			else
			{
				BUZZER_SWITCH();
				counter_for_timeout_time_buzzer_work++;
			}
			LED_SWITCH();
			flag.off_indicator = ~flag.off_indicator;
		}
		counter = NUMBER_INTERRUPTS_PER_SECOND;
	}
	if(!flag.off_indicator)
	{
		flag.set_indicator = TRUE;
	}
}

//Основная функция
int main(void)
{
	//init//////////////////////////////////////////////////////////////
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOFEN;
	//настройка TIM7 для отлова ошибок по таймауту
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	TIM17->CR1 |= TIM_CR1_OPM;
	TIM17->PSC = (CLOCK_FREQUENCY / 1000) - 1; //в 1 секунде - 1000 млиллисекунд

	RCC->CR |= RCC_CR_HSEON;
	TIM17_START(250);
	while (!(RCC->CR & RCC_CR_HSERDY)) {
	  if (!(TIM17->CR1 & TIM_CR1_CEN)) {
		RCC->CR &= ~RCC_CR_HSEON;
		break;
	  }
	}

	//led, индикатор: d,e,f,g,dp;CC1,2,3,4
	GPIOB->MODER |= GPIO_MODER_MODER7_0
					| GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0
					| GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	//buzzer, test_k1, индикатор:  a,b,c
	GPIOA->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER8_0
					| GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
	//work
	GPIOC->MODER |= GPIO_MODER_MODER15_0;
	//btn
	//GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0;
	//check_work
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0;
	//timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16->PSC = psc - 1;
	TIM16->ARR = arr - 1;
  	TIM16->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM16_IRQn);
	NVIC_SetPriority(TIM16_IRQn, 0);
	TIM16->CR1 |= TIM_CR1_CEN;

	SetWaiting();
	//init end///////////////////////////////////////////////////////////
	while(TRUE)
	{
		if(CHECK_WORK_PORT())
		{
			if(flag.waiting || flag.pause)
			{
				if(btn.btn1_short_press|| btn.btn1_long_press)
				{
					if(time_in_seconds > 0)
					{
						time_in_seconds -= GetSamplingStep();
					}
					flag.set_indicator = TRUE;
					btn.btn1_short_press = FALSE;
					btn.btn1_long_press = FALSE;
				}
				if(btn.btn2_short_press)
				{
					flag.waiting = FALSE;
					flag.pause = FALSE;
					flag.countdown = TRUE;
					btn.btn2_short_press = FALSE;
				}
				if(btn.btn3_short_press || btn.btn3_long_press)
				{
					if(time_in_seconds < (99 * 60 + 59))
					{
						time_in_seconds += GetSamplingStep();
					}
					flag.set_indicator = TRUE;
					btn.btn3_short_press = FALSE;
					btn.btn3_long_press = FALSE;
				}
			}

			if(flag.countdown)
			{
				if(btn.btn2_short_press)
				{
					flag.pause = TRUE;
					flag.countdown = FALSE;
					btn.btn2_short_press = FALSE;
				}
				WORK_START();
			} else { WORK_STOP(); }

			if(btn.btn2_long_press)
			{
				SetWaiting();
				btn.btn2_long_press = FALSE;
			}
			if(flag.set_indicator)
			{
				SetNumberIndicator(ConvertToTimeFormat(time_in_seconds));
				flag.set_indicator = FALSE;
			}
			if(flag.off_indicator){
				GPIOB->BSRR |= GPIO_BSRR_BR_12 | GPIO_BSRR_BR_13 | GPIO_BSRR_BR_14 | GPIO_BSRR_BR_15;
			}
		}
		else
		{
			time_in_seconds = 0;
			SetNumberIndicator(8888);
			SetWaiting();
		}
	}
}
