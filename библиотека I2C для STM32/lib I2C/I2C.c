#include "CONST.h"
#include "I2C.h"
/*1 master*/
/******************************************************************************
 * Переменные
******************************************************************************/

/******************************************************************************
 * Name:		Инициализация I2C2
 * Input:		-
 * Return:	-
 * Remark:  Master-mode, Fast-mode, отключены аналоговый и цифровой фильтр, 7-bit addressing mode
******************************************************************************/
void I2C2Init(){
	//настройка TIM7 для задержки и отлова ошибок по таймауту
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->CR1 |= TIM_CR1_OPM;
	TIM7->PSC = (CLOCK_FREQUENCY / 1000) - 1; //в 1 секунде - 1000 млиллисекунд
	TIM7->EGR = TIM_EGR_UG;

	//альтернатитвные функции ног
	GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOB->AFR[1] |= AF1_AFRH10;																	//I2C2_SCL
	GPIOB->AFR[1] |= AF1_AFRH11;																	//I2C2_SDA
	
	//настройка I2C Initial settings
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	//Clear PE bit in I2C_CR1
	I2C2->CR1 &= ~I2C_CR1_PE;
	//Configure ANFOFF and DNF[3:0] in I2C_CR1
	//По умолчанию
	//Configure PRESC[3:0], SCLDEL[3:0], SDADEL[3:0], SCLH[7:0], SCLL[7:0] in I2C_TIMINGR
	I2C2->TIMINGR = 0x00 << 28 | 0x03 << 20 | 0x01 << 16 | 0x03 << 8 | 0x09 << 0; //8 MHz -> Fast-mode (Fm), 400 kHz
	//Configure NOSTRETCH in I2C_CR1
	//По умолчанию
	//Set PE bit in I2C_CR1
	I2C2->CR1 |= I2C_CR1_PE;
};

/******************************************************************************
 * Name:		Ресет I2C
 * Input:		-
 * Return:	-
 * Remark:  освобождаются линии I2C SCL и SDA, На регистры конфигурации это не влияет. Сброс регистров: I2C_CR2 register: START, STOP, NACK, I2C_ISR register: BUSY, TXE, TXIS, RXNE, ADDR, NACKF, TCR, TC, STOPF, BERR, ARLO, OVR
******************************************************************************/
void SoftwareReset(){
	I2C2->CR1 &= ~I2C_CR1_PE;
	while(I2C2->CR1 &= I2C_CR1_PE){}
	I2C2->CR1 |= I2C_CR1_PE;
}
/******************************************************************************
 * Name:		Проверка флагов ошибок ISR
 * Input:		-
 * Return:	Признак ошибки
 * Remark:  -
******************************************************************************/
unsigned char CheckError(){
	if ((I2C2->ISR & I2C_ISR_BERR) | (I2C2->ISR & I2C_ISR_ARLO) | (I2C2->ISR & I2C_ISR_NACKF)){
		SoftwareReset();
		return I2C_ERROR;
	} else {
		return I2C_OK;
	}
}
/******************************************************************************
 * Name:		Генерация START
 * Input:		Данные для регистра CR2
 * Return:	Признак выполнения
 * Remark:  Уставновлен флаг I2C_CR2_RELOAD, NBYTES reload mode
******************************************************************************/
unsigned char StartI2C(unsigned short count_bytes, const unsigned char HEADER, unsigned int OptionRW){
	//if(((I2C2->ISR & I2C_ISR_BUSY) >> 10) <= !(I2C2->ISR & I2C_ISR_STOPF)){ //импликация, нельзя генерировать старт, если линия занята, а стоп не было
	SoftwareReset();
	I2C2->CR2 = I2C_CR2_RELOAD | (SIZE_TX_NBYTES(count_bytes) << 16) | OptionRW | HEADER | I2C_CR2_START;
	
	TIM7->CNT = 0x0000;
	TIM7->ARR = TIMEOUT_TRANSMIT_BYTE; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	while(I2C2->CR2 & I2C_CR2_START){
		if(!(TIM7->CR1 & TIM_CR1_CEN)){ return I2C_ERROR; }
	}
	return CheckError();
	//} else{return I2C_ERROR;}
}
/******************************************************************************
 * Name:		Генерация STOP
 * Input:		-
 * Return:	Признак выполнения
 * Remark:  -
******************************************************************************/
unsigned char StopI2C(){
	I2C2->CR2 |= I2C_CR2_STOP;
	while(!(I2C2->ISR & I2C_ISR_STOPF)){//ожидание установки сигнала стоп
		I2C2->CR2 &= ~I2C_CR2_STOP;
	}
	return CheckError();
}
/******************************************************************************
 * Name:		Передать байт в I2C
 * Input:		-
 * Return:	Признак выполнения
 * Remark:  -
******************************************************************************/
unsigned char TransmitByte(unsigned char byte, unsigned short count_bytes){
	I2C2->TXDR = byte;
	
	TIM7->CNT = 0x0000;
	TIM7->ARR = TIMEOUT_TRANSMIT_BYTE; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	//Жду пока передастся бай или сработает таймаут
	while (!(I2C2->ISR & I2C_ISR_TXE)){
		if(!(TIM7->CR1 & TIM_CR1_CEN)){ return I2C_ERROR; }
		//передалось NBYTES -> если в теле этого цикла, значит пакета передача ещё не закончилась (более 255 байт)
		if(I2C2->ISR & I2C_ISR_TCR){
			I2C2->CR2 |= (SIZE_TX_NBYTES(count_bytes) << 16);
		}
	}
	return CheckError();
}
/******************************************************************************
 * Name:		Прочитать байт из шины I2C2
* Input:		Указатель, куда записать результат
 * Return:	Результат выполнения
 * Remark:  -
******************************************************************************/
unsigned char ReadByteI2C2(unsigned char* byte, unsigned char count_bytes)
{
	TIM7->CNT = 0x0000;
	TIM7->ARR = TIMEOUT_TRANSMIT_BYTE; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	//Жду пока передастся бай или сработает таймаут
	while (!(I2C2->ISR & I2C_ISR_RXNE)){
		if(!(TIM7->CR1 & TIM_CR1_CEN)){ return I2C_ERROR; }
		//передалось NBYTES -> если в теле этого цикла, значит пакета передача ещё не закончилась (более 255 байт)
		if(I2C2->ISR & I2C_ISR_TCR){
			I2C2->CR2 |= (SIZE_TX_NBYTES(count_bytes) << 16);
		}
	}
	*byte = I2C2->RXDR;

	return CheckError();
};
/******************************************************************************
 * Name:		Запись пакета данных в шину I2C2
 * Input:		Указатель на массив передаваемых данных, кол-во отправляемых байт из этого пакета (может быть 0), адрес устройства (с учётом контрольных битов - чтение/запись, команда/дата), генерировать ли сигналы старт и стоп
 * Return:	Признак выполнения
 * Remark:  Transfer sequence flow for I2C master receiver for N > 255 bytes, NBYTES reload mode
******************************************************************************/
unsigned char WriteI2C2(unsigned char* pointer_package, unsigned short count_bytes, const unsigned char HEADER, unsigned char generated_start, unsigned char generated_stop)
{
	//Сигнал старт
	if(generated_start){
		if(!StartI2C(count_bytes, HEADER, FALSE)){return I2C_ERROR;}
	}
	//Передача байт
	if (!(I2C2->ISR & I2C_ISR_TXE)){return I2C_ERROR;}
	while(count_bytes){
		if(!TransmitByte(*pointer_package, count_bytes)){return I2C_ERROR;}
		pointer_package++;
		count_bytes--;
	}
	//Сигнал стоп
	if(generated_stop){
		if(!StopI2C()){return I2C_ERROR;}
	}
	
	return I2C_OK;
};
/******************************************************************************
 * Name:		Прочитать пакет данных из шины I2C2
* Input:		Указатель на массив для записи в него данных, кол-во считываемых байт в этот пакет (начиная с 0 элемента), адрес устройства (с учётом контрольных битов - чтение/запись, команда/дата)
 * Return:	Результат выполнения
 * Remark:  N <= 255 bytes, Automatic end mode
******************************************************************************/
unsigned char ReadI2C2(unsigned char* pointer_package, unsigned short count_bytes,const unsigned char HEADER, unsigned char generated_start, unsigned char generated_stop)
{
	//сигнал старт
	if(generated_start){
		if(!StartI2C(count_bytes, HEADER, I2C_CR2_RD_WRN)){return I2C_ERROR;}
	}
	//чтение байт
	while(count_bytes){
		if(!ReadByteI2C2(pointer_package, count_bytes)){return I2C_ERROR;}
		pointer_package++;
		count_bytes--;
	}
	//сигнал стоп
	if(generated_stop){
		if(!StopI2C()){return I2C_ERROR;}
	}
	return I2C_OK;
};
