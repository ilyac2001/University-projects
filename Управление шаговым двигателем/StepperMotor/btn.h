#include <stm32f072xb.h>
//PC13
#define BTN_REGISTER_MASK (0x01 << 13) //BTN_REGISTER_MASK = BTN_STRUCT_MASK << 8
#define BTN_STRUCT_MASK		(0x01 << 5) //BTN_STRUCT_MASK = BTN_REGISTER_MASK >> 8
#define BTN_TIMER_LONG_PRESS (NUMBER_INTERRUPTS_PER_SECOND * 0); //время, начиная с которого считается, что кнопка долго нажата (удержана), значение высчитывается сходя из найтсроек таймера прерываний

#define BtnPort() (GPIOC->IDR & BTN_REGISTER_MASK)
#define BtnPress() (~BtnPort() & BTN_REGISTER_MASK) //1 - положение кнопки внизу, 0 - отпущена

typedef union
{
	struct
	{
		unsigned char btn_reg_short_press;
/*
//		unsigned char btn_reg_long_press;
*/
	};
	struct
	{
		//btn_reg_short_press
		unsigned empty1_short_press	:1;
		unsigned empty2_short_press	:1;
		unsigned empty3_short_press	:1;
		unsigned empty4_short_press :1;
		unsigned empty5_short_press :1;
		unsigned btn_short_press 		:1; //минимальное выделение памяти, при сдвиге на 1 байт это значние соответствует регистру IDR13
		unsigned empty7_short_press :1;
		unsigned empty8_short_press	:1;
//		//btn_reg_long_press
/*
//		unsigned empty1_long_press	:1;
//		unsigned empty2_long_press	:1;
//		unsigned empty3_long_press	:1;
//		unsigned empty4_long_press	:1;
//		unsigned empty5_long_press	:1;
//		unsigned btn_long_press			:1;
//		unsigned empty7_long_press	:1;
//		unsigned empty8_long_press	:1;
*/
	};
} TypeBtnStatus;

void BtnInit(void);
void BtnSurvey(void);
