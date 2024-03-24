#include <stm32f072xb.h>
 //PA0 (та, что ниже),1
#define ledOn01() 	GPIOA->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BS_1;
#define ledOff01() GPIOA->BRR = GPIO_BRR_BR_0 | GPIO_BRR_BR_1;
#define ledSwitch01() GPIOA->ODR ^= GPIO_ODR_0 | GPIO_ODR_1;

void LedInit(void);
