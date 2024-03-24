#ifndef __LCD_TIC154A_H
#define __LCD_TIC154A_H
/******************************************************************************
 * Ресурсы
PB2 - LCD_RST
******************************************************************************/
#define LCD_RST_OFF()		GPIOB->BSRR = GPIO_BSRR_BS_2
#define LCD_RST_ON()		GPIOB->BRR = GPIO_BRR_BR_2
// Параметры дисплея
#define	WIDTH  132                     
#define	HEIGTH 64
#define SIZE_DISPLAY_BUFFER_IN_BYTE (HEIGTH * WIDTH / 8)
// Передача данных в контроллер дисплея
#define	TIC154A_HEADER          0x70
#define	COMMAND_WRITE			0x00
#define	DATA_WIRTE				0x02
#define COMMAND_HEADER_TIC154A  (COMMAND_WRITE | TIC154A_HEADER)
#define DATA_HEADER_TIC154A     (DATA_WIRTE | TIC154A_HEADER)
//Command
#define SYSTEM_RESET			(COMMAND_WRITE | 0b0011100010)
//опции для функций графики
#define EXTINGUISH 0b00000000 //выбор цвета пикселей: потушить - белый
#define LIGHT	   0b00000001 //выбор цвета пикселей: зажечь - чёрный
#define EMPTY      0b11111111 //выбор цвета заливки фигуры: пустой - потушить - белый
#define FLOOD      LIGHT //выбор цвета заливки фигуры: залитый - зажечь - чёрный
//макросы для заполнения буфера дисплея
#define SWAP(type, a, b) type tmp = a; a = b; b = tmp;
#define BIT_IN_BYTE 8
#define HEIGTH_STRING_DISPLAYIN_BYTE 2
#define GET_PIXEL_ADDRESS(X, Y) ((Y/BIT_IN_BYTE)*((unsigned int)(WIDTH)) + X)
#define GET_PIXEL_BIT_MASK(Y)	(0x01 << (Y % BIT_IN_BYTE))
#define GET_STRING_ADDRESS(X, Y_string_number) ((unsigned short)(Y_string_number * HEIGTH_STRING_DISPLAYIN_BYTE))*WIDTH + X
#define CHAR_LOW_BYTE_UNDERSCORE 0x20
/******************************************************************************
* Пользовательские типы данных
******************************************************************************/

/******************************************************************************
* Прототипы функций
******************************************************************************/
unsigned char LCDInit(void);
unsigned char SendPackageUC1601S(unsigned char package[], unsigned short size_package, const unsigned char HEADER_AND_COMMAND_OR_DATA);
//функции графики
void UpdateLCDBuffer(const unsigned char picture[]);
void InvertStringLCDBuffer(unsigned char Y_string_number);
    //графические примитивы
void GetPixelLCDBuffer(unsigned char X, unsigned char Y, unsigned char color);
void GetLineLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color);
void GetRectangleLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color_perimeter, unsigned char color_square);
void GetCircleLCDBuffer(unsigned char X, unsigned char Y, unsigned char R, unsigned char color_line);
    //функции работы с текстом
void GetStringLCDBuffer(unsigned char X, unsigned char Y_string_number, unsigned char* pointer_string, unsigned char color_text, unsigned char underline);
void InvertStringLCDBuffer(unsigned char Y_string_number);
//переменные
extern unsigned char display_buffer_transmit[SIZE_DISPLAY_BUFFER_IN_BYTE];
void GetVerticalLineLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char Y2, unsigned char color_line);
/////TEST////
unsigned char TestUpdateDisplayAndBufferDisplay(const unsigned char NUMBER_TEST);
////////////
#endif //__LCD_TIC154A_H
