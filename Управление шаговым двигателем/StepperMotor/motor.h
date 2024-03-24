#include <stm32f072xb.h>
#define MOTOR_REGISTER_MASK 0x000000F0 //ODR4,5,6,7 - 1
#define MOTOR_START_BIT GPIO_BSRR_BS_4
#define ClearMotorRegister() GPIOB->ODR &= ~MOTOR_REGISTER_MASK 

void MotorInit(void);
void MotorTakeStep(void); //сдвигает подаёт сигнал для управления мотора (сдвиг одного бита IN1-4)
