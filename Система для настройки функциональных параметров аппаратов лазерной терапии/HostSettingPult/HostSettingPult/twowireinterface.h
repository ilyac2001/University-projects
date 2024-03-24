#ifndef TWOWIREINTERFACE_H
#define TWOWIREINTERFACE_H
#include <QMap>
//#define CodeCommandStatusRequest 0x06
#define SIZE_PACKAGE 5

#define IndexElementSentPackageCodeCommand               0
#define IndexElementSentPackageNumberInfluencingFactor   1
#define IndexElementSentPackageLowestByteData            2
#define IndexElementSentPackageHighestByteData           3
#define IndexElementSentPackageChecksum                  4

#define START_LOW_BYTE_READ_NAME    0x43 //Название блока на русском языке Не более 16-ти символов ISCII (признак конца строки 0х00)
#define START_HI_BYTE_READ_NAME     0x00 //Название блока на русском языке Не более 16-ти символов ISCII (признак конца строки 0х00)
#define START_LOW_BYTE_READ_VERSION 0x30
#define START_HI_BYTE_READ_VERSION  0x00
#define START_LOW_BYTE_MIN_POWER    0x00
#define START_HI_BYTE_MIN_POWER     0x00
#define START_LOW_BYTE_MAX_POWER    0x01
#define START_HI_BYTE_MAX_POWER     0x00
#define SIZE_BYTE_READ_NAME         16 + 1 //0x0053 Резерв(для кода конца строки)
#define SIZE_BYTE_READ_VERSION      16
 //16-ти битное значение представляет собой частоту модуляции/следования импульсов в [Гц].
#define LOW_BYTE_FREQUENCY          0xDC
#define HI_BYTE_FREQUENCY           0x05
//16-ти битное значение представляет собой мощность в процентах: 0х0000 – 0% (минимальная мощность), 0х03FF – 100% (максимальная мощность). Фактическая мощность в [Вт] рассчитывается головкой. 
#define LOW_BYTE_MIN_POWER          0x00
#define HI_BYTE_MIN_POWER           0x00
#define LOW_BYTE_MAX_POWER          0xFF
#define HI_BYTE_MAX_POWER           0x03
/*минимальная
запрос 09 00 00 00 кс
ответ  09 00 00 данные кс
максимальная
запрос 09 00 01 00 кс
ответ  09 00 01 данные кс*/
#define WRITE_LOW_BYTE_MIN_POWER    START_LOW_BYTE_MIN_POWER
#define WRITE_LOW_BYTE_MAX_POWER    START_LOW_BYTE_MAX_POWER

// Command for head
/**
 * @page Описание протокола двухпроводного интерфейса
 * @brief Команды по протоколу настройки исполнительных блоков (поле данных для протокола обмена с hid)
 Состоит из:
 - CMD_SET_FREQUENCY = 0x00,
 - CMD_SET_POWER =     0x01,
 - CMD_START =         0x02,
 - CMD_STOP =          0x03,
 - CMD_PAUSE =         0x04,
 - CMD_CONTINUE =      0x05,
 - CMD_GET_STATUS =    0x06,
 - CMD_GET_POWER =     0x07,
 - CMD_WRITE_EEPROM =  0x08,
 - CMD_READ_EEPROM =   0x09,
 - CMD_SET_PARAM =     0x0A,
  - CMD_READ_EEPROM =   0x09,
*/
enum TypeCMD
{
    CMD_SET_FREQUENCY = 0x00,
    CMD_SET_POWER =     0x01,
    CMD_START =         0x02,
    CMD_STOP =          0x03,
    CMD_PAUSE =         0x04,
    CMD_CONTINUE =      0x05,
    CMD_GET_STATUS =    0x06, /**<Первая команда, которую следует отправлять при настройке*/
    CMD_GET_POWER =     0x07,
    CMD_WRITE_EEPROM =  0x08,
    CMD_READ_EEPROM =   0x09,
    CMD_SET_PARAM =     0x0A,
};

class TwoWireInterface
{
public:
    TwoWireInterface();
    void SetElementPackage(unsigned char value, unsigned char index_element_sent_package);
    unsigned char GetElementPackage(unsigned char index_element_sent_package);
    unsigned char GetChecksum();
    void SetPackageGetStatus();
    void SetPackageStop();
    void SetPackageStart();
    void SetPackageReadEEPROM(unsigned char LowestByte, unsigned char HighestByte);
    void SetPackageWriteEEPROM(unsigned char LowestByte, unsigned char HighestByte);
    void SetPackageSetFrequency();
    void SetPackageSetMinPower();
    void SetPackageSetMaxPower();
    static const QMap<unsigned char, QString> charMap;
private:
    unsigned char package[SIZE_PACKAGE];
    unsigned char CalcCRC8();
};

#endif // TWOWIREINTERFACE_H
