#include "led.h"

void LedInit(){
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0; //ножки работают на выход
}
