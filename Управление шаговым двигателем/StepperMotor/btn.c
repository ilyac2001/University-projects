#include "btn.h"

TypeBtnStatus btn;

void BtnInit(){
	//GPIOA->MODER &= ~GPIO_MODER_MODER13; //по умлочанию ножки работают на вход
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0; //включение подтягивающего резистора pull-up
}

void BtnSurvey(){ //алгоритм подразуумевает, что в один момент времени нажата одна и только одна кнопка
	static unsigned char MemoryLongClick 	= BTN_STRUCT_MASK;
	static unsigned char PastClick 		=	BTN_STRUCT_MASK;
	static unsigned char CurrentClick = BTN_STRUCT_MASK;
/*
//	static unsigned char CounterPress = BTN_TIMER_LONG_PRESS;
//	unsigned char speed = 4;
*/
	CurrentClick = (unsigned char)(BtnPort() >> 8);
	
	if(BtnPress()){
		//хотя бы одна кнопка нажата 
/*
//		if (!(--CounterPress))
//		{
//			btn.btn_reg_long_press = ~CurrentClick & BTN_STRUCT_MASK;
//			MemoryLongClick |= btn.btn_reg_long_press;
//			CounterPress = BTN_TIMER_LONG_PRESS/speed;
//		}
*/
		__NOP(); //в данном случае не обрабатываем длинное нажатие на кнопку
	}
	else{
		// Ни одна кнопка не нажата
/*
//		CounterPress = BTN_TIMER_LONG_PRESS;
*/
		btn.btn_reg_short_press |= (CurrentClick & ~PastClick) & ~MemoryLongClick; //не нажата и была нажата и не была долго нажата
		MemoryLongClick = 0x0000;
	}

	PastClick = CurrentClick;
}
