#ifndef __I2C_H
#define __I2C_H
/******************************************************************************
 * Ресурсы
Аппаратные
  PB10,11,2		- LCD_SCL (AF1-I2C2_SCL), LCD_CDA (AF1-I2C2_SDA), LCD_RST
Програмные
	TIM7 для задержки и проверке времени на передачу байта
******************************************************************************/
#define AF1 0b0001
#define AF1_AFRH10 (AF1 << (4 * 2))
#define AF1_AFRH11 (AF1 << (4 * 3))

#define MAX_SIZE_NBYTES 255 //CR2 The number of bytes to be transmitted/received is programmed there.
#define SIZE_TX_NBYTES(count_bytes) ((count_bytes > MAX_SIZE_NBYTES) ? MAX_SIZE_NBYTES : count_bytes)

/* Передача данных между функциями */
#define I2C_ERROR FALSE                                       // Случилась ошибка обмена по I2C
#define I2C_OK    TRUE                                        // Ошибок обмена по I2C нет
#define TIMEOUT_TRANSMIT_BYTE 2								  // Таймаут на передачу байта или утсановки сагнала Start и Stop, используется для сравнения с TIM7
/******************************************************************************
* Пользовательские типы данных
******************************************************************************/

/******************************************************************************
* Прототипы функций
******************************************************************************/
void I2C2Init(void);
unsigned char WriteI2C2 (unsigned char* pointer_package, unsigned short count_byte, const unsigned char HEADER,unsigned char generated_start, unsigned char generated_stop);
unsigned char ReadI2C2 (unsigned char* pointer_package, unsigned short count_bytes,const unsigned char HEADER, unsigned char generated_start, unsigned char generated_stop);
#endif //__I2C_H
