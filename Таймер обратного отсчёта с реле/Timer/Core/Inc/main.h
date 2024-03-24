/*
 * main.h
 *
 *  Created on: 19 апр. 2023 г.
 *      Author: Ilya
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_
#include <stm32f072xb.h>
/*
 * Ресурсы:
 * PB7 		- led
 * PA3 		- buzzer
 * PC15		- work
 * PC13 	- check_work
 * PA0,1,2 	- key3,2,1
 * PF0, PF1 - кварц
 * PB0,1,2,10,11 И PA5,6,7 - индикатор: d,e,f,g,dp,a,b,c
 * PB12,13,14,15 		   - индикатор: CC1,2,3,4
 * PA8 		- K1
 */
/******************************************************************************
* Пользовательские типы данных
******************************************************************************/
typedef union
{
	struct
	{
		unsigned set_indicator	:1;
		unsigned off_indicator	:1;
		unsigned timeout		:1;
		unsigned waiting 		:1;
		unsigned countdown 		:1;
		unsigned pause			:1;
	};
	unsigned char FlagREG;  // Регистр флагов
} TypeFlag;
//btn
typedef union
{
	struct
	{
		unsigned char btn_reg_short_press;
		unsigned char btn_reg_long_press;
	};
	struct
	{
		//btn_reg_short_press
		unsigned btn3_short_press	:1;
		unsigned btn2_short_press	:1;
		unsigned btn1_short_press	:1;
		unsigned empty4_short_press :1;
		unsigned empty5_short_press :1;
		unsigned empty6_short_press :1;
		unsigned empty7_short_press :1;
		unsigned empty8_short_press	:1;
		//btn_reg_long_press
		unsigned btn3_long_press	:1;
		unsigned btn2_long_press	:1;
		unsigned btn1_long_press	:1;
		unsigned empty4_long_press	:1;
		unsigned empty5_long_press	:1;
		unsigned empty6_long_press	:1;
		unsigned empty7_long_press	:1;
		unsigned empty8_long_press	:1;
	};
} TypeBtnStatus;
/******************************************************************************
* Макросы
******************************************************************************/
// Константы
#define TRUE  					1
#define FALSE					0
#define CLOCK_FREQUENCY			8000000			// Гц, тактовая частота ядра
//tim
#define psc 2
#define arr 16000
#define NUMBER_INTERRUPTS_PER_SECOND (CLOCK_FREQUENCY / psc / arr)
#define TIM17_START(time_ms) ({ \
	TIM17->EGR = TIM_EGR_UG; 	\
	TIM17->ARR = time_ms; 		\
	TIM17->CR1 |= TIM_CR1_CEN; 	})
//led
#define LED_ON() 	 (GPIOB->BSRR |= GPIO_BSRR_BS_7)
#define LED_OFF() 	 (GPIOB->BSRR |= GPIO_BSRR_BR_7)
#define LED_SWITCH() (GPIOB->ODR ^= GPIO_ODR_7)
//buzzer
#define BUZZER_ON() 	(GPIOA->BSRR |= GPIO_BSRR_BS_3)
#define BUZZER_OFF() 	(GPIOA->BSRR |= GPIO_BSRR_BR_3)
#define BUZZER_SWITCH() (GPIOA->ODR ^= GPIO_ODR_3)
//work
#define WORK_START()	(GPIOC->BSRR |= GPIO_BSRR_BR_15)
#define WORK_STOP() 	(GPIOC->BSRR |= GPIO_BSRR_BS_15)
//btn
#define BTN_REGISTER_MASK 	 0b00000111
#define BTN_STRUCT_MASK		 BTN_REGISTER_MASK
#define BTN_TIMER_LONG_PRESS(value_time_ms) (value_time_ms * NUMBER_INTERRUPTS_PER_SECOND / 1000) //время, начиная с которого считается, что кнопка долго нажата (удержана), значение высчитывается сходя из найтсроек таймера прерываний

#define BtnPort() (GPIOA->IDR & BTN_REGISTER_MASK)
#define BtnPress() (~BtnPort() & BTN_REGISTER_MASK) //1 - положение кнопки внизу, 0 - отпущена
//индикатор
#define SEC_UNITS 0b00000001
#define SEC_TENS  0b00000010
#define MIN_UNITS 0b00000100
#define MIN_TENS  0b00001000
//check_work
#define CHECK_WORK_REGISTER_MASK (0x01 << 13)
#define CHECK_WORK_PORT() (GPIOC->IDR & CHECK_WORK_REGISTER_MASK) //значение 1 - работает, 0 - не работате
//test_k1
//#define SIGNAL_HI() 	(GPIOA->BSRR = GPIO_BSRR_BS_8)
//#define SIGNAL_LOW() 	(GPIOA->BRR = GPIO_BRR_BR_8)
//#define SIGNAL_SWITCH() (GPIOA->ODR ^= GPIO_ODR_8)
//макросы для работы с таймером
#define SetWaiting() ({			\
	flag.FlagREG = FALSE; 		\
	flag.waiting = TRUE; 		\
	flag.set_indicator = TRUE; 	\
	time_in_seconds = 0;		\
	WORK_STOP();				\
	BUZZER_OFF();				\
	LED_OFF(); })
#endif /* INC_MAIN_H_ */
