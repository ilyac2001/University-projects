#include "twowireinterface.h"
#include <QMainWindow>
TwoWireInterface::TwoWireInterface()
{
    for(int i = 0;  i < SIZE_PACKAGE; i++)
    {
        this->package[i] = 0;
    }
}
//присваивает значение элементу пакета и пересчитывает КС
void TwoWireInterface::SetElementPackage(unsigned char value, unsigned char index_element_sent_package)
{
    if(index_element_sent_package >= 0 && index_element_sent_package < SIZE_PACKAGE)
    {
        this->package[index_element_sent_package] = value;
        this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
    }
}

unsigned char TwoWireInterface::GetElementPackage(unsigned char index_element_sent_package)
{
    unsigned char test = this->package[index_element_sent_package];
    return this->package[index_element_sent_package];
}

unsigned char TwoWireInterface::GetChecksum()
{
    return this->CalcCRC8();
}


unsigned char TwoWireInterface::CalcCRC8()
{
    unsigned char BitCount, ByteCount, CRC1W, DataByte;

    CRC1W = 0x00;
    for (ByteCount = 0x00; ByteCount < SIZE_PACKAGE - 1; ByteCount++)
    {
        DataByte = this->package[ByteCount];
        for (BitCount = 0x00; BitCount < 8; BitCount++)
        {
            DataByte ^= (CRC1W & 0x01);
            if (DataByte & 0x01)
            {
                CRC1W ^= 0x18;
            }

            CRC1W >>= 1;
            if (DataByte & 0x01)
            {
                CRC1W |= 0x80;
            }
            DataByte >>= 1;
        }
    }
    return CRC1W^0xff;
}

void TwoWireInterface::SetPackageGetStatus()
{
    qInfo("package CMD_GET_STATUS");
    package[IndexElementSentPackageCodeCommand] = CMD_GET_STATUS;
    package[IndexElementSentPackageNumberInfluencingFactor] = 0x00; //Номер воздействующего фактора, состояние которого запрашивается. Для головок с одним воздействующим фактором это поле не учитывается
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageStart()
{
    qInfo("package CMD_START");
    package[IndexElementSentPackageCodeCommand] = CMD_START;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageStop()
{
    qInfo("package CMD_STOP");
    package[IndexElementSentPackageCodeCommand] = CMD_STOP;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageReadEEPROM(unsigned char LowestByte, unsigned char HighestByte)
{
    qInfo("package CMD_READ_EEPROM");
    package[IndexElementSentPackageCodeCommand] = CMD_READ_EEPROM;
    package[IndexElementSentPackageLowestByteData] = LowestByte;
    package[IndexElementSentPackageHighestByteData] = HighestByte;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageWriteEEPROM(unsigned char LowestByte, unsigned char HighestByte)
{
    qInfo("package CMD_WRITE_EEPROM");
    package[IndexElementSentPackageCodeCommand] = CMD_WRITE_EEPROM;
    package[IndexElementSentPackageLowestByteData] = LowestByte;
    package[IndexElementSentPackageHighestByteData] = HighestByte;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageSetFrequency()
{
    qInfo("package CMD_SET_FREQUENCY");
    package[IndexElementSentPackageCodeCommand] = CMD_SET_FREQUENCY;
    package[IndexElementSentPackageLowestByteData] = LOW_BYTE_FREQUENCY;
    package[IndexElementSentPackageHighestByteData] = HI_BYTE_FREQUENCY;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageSetMinPower()
{
    qInfo("package CMD_SET_POWER - min");
    package[IndexElementSentPackageCodeCommand] = CMD_SET_POWER;
    package[IndexElementSentPackageLowestByteData] = LOW_BYTE_MIN_POWER;
    package[IndexElementSentPackageHighestByteData] = HI_BYTE_MIN_POWER;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

void TwoWireInterface::SetPackageSetMaxPower()
{
    qInfo("package CMD_SET_POWER - max");
    package[IndexElementSentPackageCodeCommand] = CMD_SET_POWER;
    package[IndexElementSentPackageLowestByteData] = LOW_BYTE_MAX_POWER;
    package[IndexElementSentPackageHighestByteData] = HI_BYTE_MAX_POWER;
    this->package[IndexElementSentPackageChecksum] = this->CalcCRC8();
}

const QMap<unsigned char, QString> TwoWireInterface::charMap = {
    {0x20, " "},
    {0x21, "!"},
    {0x22, "\""},
    {0x23, "#"},
    {0x24, "$"},
    {0x25, "%"},
    {0x26, "&"},
    {0x27, "\""},
    {0x28, "("},
    {0x29, ")"},
    {0x2A, "*"},
    {0x2B, "+"},
    {0x2C, ","},
    {0x2D, "-"},
    {0x2E, "."},
    {0x2F, "/"},
    {0x30, "0"},
    {0x31, "1"},
    {0x32, "2"},
    {0x33, "3"},
    {0x34, "4"},
    {0x35, "5"},
    {0x36, "6"},
    {0x37, "7"},
    {0x38, "8"},
    {0x39, "9"},
    {0x3A, ":"},
    {0x3B, ";"},
    {0x3C, "<"},
    {0x3D, "="},
    {0x3E, ">"},
    {0x3F, "?"},
    {0x40, "@"},
    {0x41, "A"},
    {0x42, "B"},
    {0x43, "C"},
    {0x44, "D"},
    {0x45, "E"},
    {0x46, "F"},
    {0x47, "G"},
    {0x48, "H"},
    {0x49, "I"},
    {0x4A, "J"},
    {0x4B, "K"},
    {0x4C, "L"},
    {0x4D, "M"},
    {0x4E, "N"},
    {0x4F, "O"},
    {0x50, "P"},
    {0x51, "Q"},
    {0x52, "R"},
    {0x53, "S"},
    {0x54, "T"},
    {0x55, "U"},
    {0x56, "V"},
    {0x57, "W"},
    {0x58, "X"},
    {0x59, "Y"},
    {0x5A, "Z"},
    {0x5B, "["},
    {0x5C, "\\"},
    {0x5D, "]"},
    {0x5E, "^"},
    {0x5F, "_"},
    {0x60, "`"},
    {0x61, "a"},
    {0x62, "b"},
    {0x63, "c"},
    {0x64, "d"},
    {0x65, "e"},
    {0x66, "f"},
    {0x67, "g"},
    {0x68, "h"},
    {0x69, "i"},
    {0x6A, "j"},
    {0x6B, "k"},
    {0x6C, "l"},
    {0x6D, "m"},
    {0x6E, "n"},
    {0x6F, "o"},
    {0x70, "p"},
    {0x71, "q"},
    {0x72, "r"},
    {0x73, "s"},
    {0x74, "t"},
    {0x75, "u"},
    {0x76, "v"},
    {0x77, "w"},
    {0x78, "x"},
    {0x79, "y"},
    {0x7A, "z"},
    {0x7B, "{"},
    {0x7C, "|"},
    {0x7D, "}"},
    {0x7E, "~"},
    {0x7F, ""},
    {0x80, "€"},
    {0x80, "Ђ"},
    {0x81, "Ѓ"},
    {0x82, "‚"},
    {0x83, "ѓ"},
    {0x84, "„"},
    {0x85, "…"},
    {0x86, "†"},
    {0x87, "‡"},
    {0x89, "‰"},
    {0x8A, "Љ"},
    {0x8B, "‹"},
    {0x8C, "Њ"},
    {0x8D, "Ќ"},
    {0x8E, "Ћ"},
    {0x8F, "Џ"},
    {0x90, "ђ"},
    {0x91, "‘"},
    {0x92, "’"},
    {0x93, "“"},
    {0x94, "”"},
    {0x95, "•"},
    {0x96, "–"},
    {0x97, "—"},
    {0x98, "?"},
    {0x99, "™"},
    {0x9A, "љ"},
    {0x9B, "›"},
    {0x9C, "њ"},
    {0x9D, "ќ"},
    {0x9E, "ћ"},
    {0x9F, "џ"},
    {0xA0, " "},
    {0xA1, "Ў"},
    {0xA2, "ў"},
    {0xA3, "Ј"},
    {0xA4, "¤"},
    {0xA5, "Ґ"},
    {0xA6, "¦"},
    {0xA7, "§"},
    {0xA8, "Ё"},
    {0xA9, "©"},
    {0xAA, "Є"},
    {0xAB, "«"},
    {0xAC, "¬"},
    {0xAD, "­"},
    {0xAE, "®"},
    {0xAF, "Ї"},
    {0xB0, "°"},
    {0xB1, "±"},
    {0xB2, "І"},
    {0xB3, "і"},
    {0xB4, "ґ"},
    {0xB5, "µ"},
    {0xB6, "¶"},
    {0xB7, "·"},
    {0xB8, "ё"},
    {0xB9, "№"},
    {0xBA, "є"},
    {0xBB, "»"},
    {0xBC, "ј"},
    {0xBD, "Ѕ"},
    {0xBE, "ѕ"},
    {0xBF, "ї"},
    {0xC0, "А"},
    {0xC1, "Б"},
    {0xC2, "В"},
    {0xC3, "Г"},
    {0xC4, "Д"},
    {0xC5, "Е"},
    {0xC6, "Ж"},
    {0xC7, "З"},
    {0xC8, "И"},
    {0xC9, "Й"},
    {0xCA, "К"},
    {0xCB, "Л"},
    {0xCC, "М"},
    {0xCD, "Н"},
    {0xCE, "О"},
    {0xCF, "П"},
    {0xD0, "Р"},
    {0xD1, "С"},
    {0xD2, "Т"},
    {0xD3, "У"},
    {0xD4, "Ф"},
    {0xD5, "Х"},
    {0xD6, "Ц"},
    {0xD7, "Ч"},
    {0xD8, "Ш"},
    {0xD9, "Щ"},
    {0xDA, "Ъ"},
    {0xDB, "Ы"},
    {0xDC, "Ь"},
    {0xDD, "Э"},
    {0xDE, "Ю"},
    {0xDF, "Я"},
    {0xE0, "а"},
    {0xE1, "б"},
    {0xE2, "в"},
    {0xE3, "г"},
    {0xE4, "д"},
    {0xE5, "е"},
    {0xE6, "ж"},
    {0xE7, "з"},
    {0xE8, "и"},
    {0xE9, "й"},
    {0xEA, "к"},
    {0xEB, "л"},
    {0xEC, "м"},
    {0xED, "н"},
    {0xEE, "о"},
    {0xEF, "п"},
    {0xF0, "р"},
    {0xF1, "с"},
    {0xF2, "т"},
    {0xF3, "у"},
    {0xF4, "ф"},
    {0xF5, "х"},
    {0xF6, "ц"},
    {0xF7, "ч"},
    {0xF8, "ш"},
    {0xF9, "щ"},
    {0xFA, "ъ"},
    {0xFB, "ы"},
    {0xFC, "ь"},
    {0xFD, "э"},
    {0xFE, "ю"},
    {0xFF, "я"}
};
