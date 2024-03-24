#include <stm32f072xb.h>

// 8000000 (Гц, тактовая частота ядра) / psc / arr = кол-во прерываний в секунду
//psc - предделитель, по формуле для установки таймера (psc - 1) 
//arr - начальное значение, по формуле для установки таймера (arr - 1) 
// psc < arr
//arr кратно 8000000 -  Гц, тактовая частота ядра
//arr, psc - 16 bit
//arr - max,  psc - min
#define psc 1 //2
#define arr 20000 //40000
#define NUMBER_INTERRUPTS_PER_SECOND (CLOCK_FREQUENCY / psc / arr) //прерывание 400 раз в секунду для частоты 100Гц

void TimerInit(void);
