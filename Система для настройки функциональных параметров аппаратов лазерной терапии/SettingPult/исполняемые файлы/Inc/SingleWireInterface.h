#ifndef __SINGLEWIREINTERFACE_H
#define __SINGLEWIREINTERFACE_H
/******************************************************************************
 * Ресурсы
PA2,3	-	TX2, RX2
*Данный файл описывает функции (порядок отправки и принятия сообщений, формаирование пакета) протокола обмена по однопроводному интерфейсу
******************************************************************************/
#define SIZE_USART_BUFFER_TX2_RX2 24 // Размер, исходя из самого большого пакета
#define SIZE_USART_BUFFER_TX2    3
#define TIMEOUT_BYTE_RX2         10 //устанавливается исходя из таймера (100Гц), данное значение не должно превышать 25, (иначе никогда не будет нулевым); каждые 250 ms отправляется новый пакет
#define CodeCommandPultGetButton 0x01
#define CodeCommandPultOutString 0x02
#define CodeCommandPultOutValue  0x03
#define CodeCommandPultPultClear 0x04
/******************************************************************************
* Пользовательские типы данных
******************************************************************************/

/******************************************************************************
* Прототипы функций
******************************************************************************/
//функции работы с usart и буффером приёма передачи
void SingleWireInterfaceInit(void);
unsigned char FillBufferReceivedByte(void);
void CheckTimeBetweenBytes(void);
//функции для работы с пакетом по протоколу
unsigned char CheckChecksumPackage(unsigned char package_accepted);
void SendPackage(unsigned char transmit_byte);
unsigned char* GetStringValue(uint32_t number, uint16_t divider, unsigned char* uint_str);
//глобальные переменные
extern unsigned char usart_buffer_Tx2_Rx2[SIZE_USART_BUFFER_TX2_RX2];   //Заполняется в соответствии с протоколом
extern unsigned char usart_pointer_buffer_Tx2_Rx2;						//Указатель на байт в usart_buffer_Tx2_Rx2
extern unsigned char timeout_byte_Rx2;									//Таймаут между принятыми байтами, если дошел до 0 , то usart_pointer_buffer_Tx2_Rx2 присвоить 0
#endif //__SINGLEWIREINTERFACE_H
