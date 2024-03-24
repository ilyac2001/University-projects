#include "motor.h"

void MotorInit(){
	GPIOB->MODER |= GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0; //на выход, для управления двигателем (подаём 1, которая инвертируется на плате)
}

void MotorTakeStep(){
	static unsigned short bufferMotorControl = MOTOR_START_BIT; //размер 16 бит соответствует размеру регистра
	
	bufferMotorControl  <<= 0x01; //сдвиг рабочего бита
	bufferMotorControl &= MOTOR_REGISTER_MASK; //выделяем 4 рабочих бита для двигателя IN1-4
	if(!bufferMotorControl){ //после наложения масски получили 0 => вышли за диапазон IN1-4
		bufferMotorControl = GPIO_BSRR_BS_4;
	}
	ClearMotorRegister(); //очищаем перед тем, как записать новое значение в IN1-4
	GPIOB->ODR |= bufferMotorControl;
}
