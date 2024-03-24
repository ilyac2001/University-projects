#include "CONST.h"
#include "SingleWireInterface.h"
#include "USART.h"
/******************************************************************************
 * Переменные
******************************************************************************/
unsigned char usart_buffer_Tx2_Rx2[SIZE_USART_BUFFER_TX2_RX2];	//Заполняется в соответствии с протоколом
unsigned char usart_pointer_buffer_Tx2_Rx2;
uint8_t timeout_byte_Rx2 = TIMEOUT_BYTE_RX2; 					//Значение устанавливается в TIMEOUT_BYTE_RX2 при каждом приёме нового байта (каждые 1.5 ms); декрементируется по таймеру (100Гц) 10 ms
/******************************************************************************
 * Name:		Очистить буффер приёма передачи
 * Input:		Байт, найчиная с которого нужно очистить буффер
 * Return:	-
 * Remark:  -
******************************************************************************/
void ClearUsartBufferTx2Rx2(unsigned char start_byte_cleanup){
	for (unsigned char i = start_byte_cleanup; i < SIZE_USART_BUFFER_TX2_RX2; i++){
		usart_buffer_Tx2_Rx2[i] = 0x00;
	}	
}
/******************************************************************************
* Name:		Проверка котрольной суммы принятого пакета
* Input:	Признак пакета, пакет передачи - 1 или приёма - 0
* Return:	результат проверки, сошлась - 1, не сошлась -0
* Remark:  -
******************************************************************************/
unsigned char CheckChecksumPackage(unsigned char transmit_package){
	uint8_t cheksum = 0;
    uint8_t size_package;
    if(transmit_package){ //так как размер пакета приёма и передчи разные
        size_package = SIZE_USART_BUFFER_TX2;
    }else{
        size_package = SIZE_USART_BUFFER_TX2_RX2;
    }
	for(int i = 0; i < size_package - 1;  i++){//сумма всех байт (длинна паке), кроме контрольной суммы (минус 1)
		cheksum += usart_buffer_Tx2_Rx2[i];
	}
	if(cheksum == usart_buffer_Tx2_Rx2[usart_buffer_Tx2_Rx2[1] - 1]){
		return TRUE;
	}
	else {
		usart_pointer_buffer_Tx2_Rx2 = 0;
		return TRUE;
	}
}
/******************************************************************************
 * Name:		Проверка на длительное не получение байт
 * Input:		-
 * Return:	Результат работы - утсановка указателя в первый байт буффера приёма
 * Remark:  Вызывается по таймеру (100Гц), если успеет дойти до 0, то следущий байт точно будет первый
******************************************************************************/
void CheckTimeBetweenBytes(){
	if(timeout_byte_Rx2 > 0){
		if(!(--timeout_byte_Rx2)){ //если 100 ms не принимал байт
			usart_pointer_buffer_Tx2_Rx2 = 0;
		}
	}
}
/******************************************************************************
 * Name:	Приём байта, заполнение буффера приёма передачи
 * Input:	-
 * Return:	результат работы - заполняет буффер приёма 1 байтом, как принят полностью пакет - возвращает TRUE
 * Remark:  -
******************************************************************************/
unsigned char FillBufferReceivedByte(){
	timeout_byte_Rx2 = TIMEOUT_BYTE_RX2; //обновление таймаута между приёмами байт
	
	usart_buffer_Tx2_Rx2[usart_pointer_buffer_Tx2_Rx2] = ((uint8_t)(USART2->RDR));
	
	if(++usart_pointer_buffer_Tx2_Rx2 == SIZE_USART_BUFFER_TX2_RX2){//принят полностью пакет
		usart_pointer_buffer_Tx2_Rx2 = 0;
		return TRUE;
	}
	return FALSE;
}
/******************************************************************************
 * Name:		подготовка пакета для отправки ответа
 * Input:		-
 * Return:	результат работы - заполняет буффер передачи (пакет)
 * Remark:  запоняется в соответствии с протоколом
******************************************************************************/
void PreparePackageForSend(unsigned char transmit_byte){
	//usart_buffer_Tx2_Rx2[0] - принятая команда, данный байт остаётся без изменений
	usart_buffer_Tx2_Rx2[1] = transmit_byte;
	usart_buffer_Tx2_Rx2[2] = usart_buffer_Tx2_Rx2[0] + usart_buffer_Tx2_Rx2[1];
	ClearUsartBufferTx2Rx2(SIZE_USART_BUFFER_TX2);
}
/******************************************************************************
 * Name:	Отправка пакета
 * Input:	-
 * Return:	отправляет пакет, если сошлась контрольная сумма, иначе отправки не будет. обнуление pointer, чтобы принимать новый пакет
 * Remark:  -
******************************************************************************/
void SendPackage(unsigned char transmit_byte){
	PreparePackageForSend(transmit_byte);
	if(CheckChecksumPackage(TRUE)){
		//начало отправки по байтам
		WriteUSART(&usart_buffer_Tx2_Rx2[0], SIZE_USART_BUFFER_TX2, USART2);
		//пакет передан, готов принимать новый
	}
	usart_pointer_buffer_Tx2_Rx2 = 0;
}
/******************************************************************************
 *  Name:	Разбор принятого пакета с числовыми данными
 * Input:	Число, делитель, массив, в который будут записаны обработаные данные
 * Return:	Результат работы - изменение массива, преобразование числа и делителя в строку символов
 * Remark:  -
******************************************************************************/
unsigned char* GetStringValue(uint32_t number, uint16_t divider, unsigned char* uint_str){
    static unsigned char result_str[SIZE_USART_BUFFER_TX2_RX2] = {'0', '0', '0', '0', '0', ',', '0', '0', '0', 0x00}; //нулевой элемент - старший разряд
    uint32_t j;
    uint32_t value;
    j = 10000;
    value = number / divider;
    for(uint8_t i = 0; i < 5; i++){
        result_str[i] = (uint8_t)((value / j) % 10 + 0x30); //в таблице шрифтоф с 0x30 начинаются цифры
        j /= 10;
    }
    value = number % divider;
    value = value * 1000 / divider; //1000 - т.к. 3 знака после запятой
    j = 100;
    for(uint8_t i = 6; i < 9; i++){
        result_str[i] = (uint8_t)((value / j) % 10 + 0x30);
        j /= 10;
    }
    j = 9;
    while(*uint_str)
    {
    	result_str[j] = *uint_str;
    	uint_str++;
    	j++;
    }
    return &result_str[0];
}

void SingleWireInterfaceInit(){
		ClearUsartBufferTx2Rx2(0);
}
