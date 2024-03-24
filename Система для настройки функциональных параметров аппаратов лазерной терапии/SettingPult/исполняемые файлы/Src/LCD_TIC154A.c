#include "CONST.h"
#include "LCD_TIC154A.h"
#include "I2C.h"
#include "MyFond.h"
/******************************************************************************
 * Переменные
******************************************************************************/
unsigned char display_buffer_transmit[SIZE_DISPLAY_BUFFER_IN_BYTE];//Данные, что выводятся на дисплей и отображают действительность
unsigned char command_buffer[1];
unsigned char TextColumn[4] = {0, 0, 0, 0};	// переменная хранит координату Х следующего символа строки дисплея, для соответствующей строки
extern const unsigned char clean[];
extern const Character MY_CHARS[];
/******************************************************************************
* Name:		Подготовка и отправка пакета данных контроллеру дисплея (обновить изображение дисплея)
 * Input:		-
 * Return:	Результат работы записи в I2C
 * Remark:  -
******************************************************************************/
unsigned char SendPackageUC1601S(unsigned char package[], unsigned short size_package, const unsigned char HEADER_AND_COMMAND_OR_DATA){
	return WriteI2C2(&package[0],  size_package, HEADER_AND_COMMAND_OR_DATA, TRUE, TRUE);    
}
/******************************************************************************
 * Name:    Обновить полностью буффер дисплея
 * Input:   -
 * Return:  -
 * Remark:  -
 *****************************************************************************/
void UpdateLCDBuffer(const unsigned char picture[])
{
	for (unsigned int Counter = 0x0000; Counter < SIZE_DISPLAY_BUFFER_IN_BYTE; Counter++)
	{
		display_buffer_transmit[Counter] = picture[Counter];
	}
}
/******************************************************************************
 * Name:	Закрасить пиксель в буффере дисплея
 * Input:	Координата пиксяля X(0..131) и Y(0..34), цвет пикселя OPTION:(потушить - EXTINGUISH/закрасить - LIGHT)
 * Return:	-
 * Remark:  Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetPixelLCDBuffer(unsigned char X, unsigned char Y, unsigned char color_pixel){
	unsigned int pixel_address = GET_PIXEL_ADDRESS(X, Y);  // Вычислил адрес байта с пикселем в видоепамяти
	if (color_pixel == LIGHT) {
		//Зажечь пиксель
		display_buffer_transmit[pixel_address] |= GET_PIXEL_BIT_MASK(Y);
	}
	else {
		//Потушить писксель
		display_buffer_transmit[pixel_address] &= ~GET_PIXEL_BIT_MASK(Y);
	}
}
/******************************************************************************
* Name:     Закрасить горизонтальную линию в буффере дисплея
* Input:    Координата начала X1(0..131) и Y1(0..34), конца Y2, цвет пикселей линии OPTION:(потушить - EXTINGUISH/закрасить - LIGHT)
* Return:	-
* Remark:   Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetHorizontalLineLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char color_line){
	if(X1 > X2){SWAP(unsigned char, X1,X2);}
	unsigned int cur_address = GET_PIXEL_ADDRESS(X1, Y1);
	const unsigned char BIT_MASK = GET_PIXEL_BIT_MASK(Y1);
	while (X1 <= X2)
	{		
		if (color_line == LIGHT){			
			display_buffer_transmit[cur_address] |= BIT_MASK;
		} 
		else{
			display_buffer_transmit[cur_address] &= ~BIT_MASK;
		}
			cur_address++; //cледующий адрес
			X1++;          //cледующая координата (для завершения цикла)
		}
}
/******************************************************************************
* Name:     Закрасить вертикальную линию в буффере дисплея
* Input:    Координата начала X1(0..131) и Y1(0..34), конца Y2, цвет пикселей линии OPTION:(потушить - EXTINGUISH/закрасить - LIGHT)
* Return:	-
* Remark:   Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetVerticalLineLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char Y2, unsigned char color_line){
	if(Y1 > Y2){SWAP(unsigned char, Y1,Y2);}
	unsigned int cur_address = GET_PIXEL_ADDRESS(X1, Y1);
	unsigned char bit_mask = 0x00;
    unsigned char bit_mask_pointer = GET_PIXEL_BIT_MASK(Y1);
	while(Y1 <= Y2){
		bit_mask |= bit_mask_pointer;
        bit_mask_pointer <<= 1;
        if(!bit_mask_pointer || (Y1 == Y2)){ //конец текущего байта или последняя итерация отрисовки линии
            if (color_line & LIGHT){			
                display_buffer_transmit[cur_address] |= bit_mask;
            } 
            else{
                display_buffer_transmit[cur_address] &= ~bit_mask;
            }
            bit_mask = 0x00;
            bit_mask_pointer = 0x01;
            cur_address += WIDTH;
        }
        Y1++;
	}
}
/******************************************************************************
* Name:     Закрасить линию в буффере дисплея
* Input:    Координата начала X1(0..131) и Y1(0..34), конца X2, Y2, цвет пикселей линии OPTION:(потушить - EXTINGUISH/закрасить - LIGHT)
* Return:	-
* Remark:   Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetLineLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color_line){
	signed int dX, dY;
	unsigned char StepX, StepY;
	signed int Error;
	dX = (signed int)X2 - (signed int)X1;
	dY = (signed int)Y2 - (signed int)Y1;
	//упрощенные алгоритмы для рисования вертикальной и горизонтальной прямой
	if(dY == 0){		
		GetHorizontalLineLCDBuffer(X1, Y1, X2, color_line);
		return;
	} else if (dX == 0){
		GetVerticalLineLCDBuffer(X1, Y1, Y2, color_line);
		return;
	}
	//алгоритм Брезинхема для произвольной линии
	if (dX < 0){ // Конец прямой левее начала
		dX = -dX;
		StepX = 255;
	}
	else{ //конец прямой правее или на одной вертикали с началом
		StepX = 1;
	}

	if (dY < 0){//конец прямой выше начала
		dY = -dY;
		StepY = 255;
	}
	else{// Конец прямой ниже или на уровне начала
		StepY = 1;
	}

	dY <<= 1;
	dX <<= 1;
	GetPixelLCDBuffer(X1, Y1, color_line);
	if (dX > dY){
		//изменение координат по горизонтали больше, чем по вертикали
		Error = dY - (dX >> 1);		//начальное значение ошибки
		while (X1 != X2){//цикл по координате Х			
			if (Error >= 0){				
				Y1 += StepY;	    //переход на следующую строку
				Error -= dX;
			}
			X1 += StepX;	    	//переход на следующий столбец
			Error += dY;
			GetPixelLCDBuffer(X1, Y1, color_line);
		}
	}
	else{
		Error = dX - (dY >> 1);		//начальное значение ошибки
		while (Y1 != Y2){//цикл по координате Y
			if (Error >= 0){				
				X1 += StepX;		//Переход на следующий столбец
				Error -= dY;
			}
			Y1 += StepY;	   	//переход на ледующую строку
			Error += dX;
			GetPixelLCDBuffer(X1, Y1, color_line);
		}
	}
}
/******************************************************************************
 * Name:	Закрасить прямоугольник в буфере дисплея
* Input:	Координаты противоположных углов прямоугольника X1(0..131), Y1(0..34) и X2, Y2, OPTION: цвет пикселя (потушить/закрасить) периметра и площади (потушить/закрасить/пустой)
 * Return:	-
 * Remark:  Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetRectangleLCDBuffer(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char color_perimeter, unsigned char color_square){        
    if((signed int)Y2 - (signed int)Y1 == 0){		
		GetHorizontalLineLCDBuffer(X1, Y1, X2, color_perimeter);
		return;
	} else if ((signed int)X2 - (signed int)X1 == 0){
		GetVerticalLineLCDBuffer(X1, Y1, Y2, color_perimeter);
		return;
	}
    if(!(color_square == EMPTY)){//если требудется закрасить площадь
        //для работы алгоритма, привести точку 1 к левому верхнему углу прямоугольника, а 2 - правому нижнему
        if(X2<X1){SWAP(unsigned char, X1, X2);}
        if(Y2<Y1){SWAP(unsigned char, Y1, Y2);}
        unsigned int cur_address = GET_PIXEL_ADDRESS(X1, Y1);
        unsigned char bit_mask = 0x00;
        unsigned char bit_mask_pointer = GET_PIXEL_BIT_MASK(Y1);
        for(unsigned char i = Y1; i <= Y2; i++){
            bit_mask |= bit_mask_pointer;
            bit_mask_pointer <<= 1;
            if(!bit_mask_pointer){//если дошли до конца байта
                unsigned int last_address = cur_address;
                for(unsigned char j = X1; j <= X2; j++){
                    if (color_square == LIGHT){
                        display_buffer_transmit[cur_address++] |= bit_mask;
                    }
                    else{
                        display_buffer_transmit[cur_address++] &= ~bit_mask;
                    }
                }
                bit_mask = 0x00;
                bit_mask_pointer = 0b00000001;
                cur_address = last_address + WIDTH;
            }
        }
        //закрасить последний байт, когда Y1 == Y2, а bit_mask_pointer != 0
         for(unsigned char j = X1; j <= X2; j++){
             if (color_square == LIGHT){
                 display_buffer_transmit[cur_address++] |= bit_mask;                    
             }
             else{
                 display_buffer_transmit[cur_address++] &= ~bit_mask;
             }
         }
    }
    //вывод периметра прямоугольника
    GetLineLCDBuffer(X1, Y1, X2, Y1, color_perimeter);
    GetLineLCDBuffer(X2, Y1, X2, Y2, color_perimeter);
    GetLineLCDBuffer(X2, Y2, X1, Y2, color_perimeter);
    GetLineLCDBuffer(X1, Y2, X1, Y1, color_perimeter);
}
/******************************************************************************
 * Name:    Создать окружность в видеопамяти
 * Input:   Координаты центра Х [0..131], Y1 [0..33]; радиус; опции; номер канала
 * Return:  Нет
 * Remark:  Координаты должны быть корректными (внутри поля дисплея)
 *****************************************************************************/
void GetCircleLCDBuffer(unsigned char X, unsigned char Y, unsigned char R, unsigned char color_line)
{
	signed short Xc, Yc, p;

	Xc = 0;
	Yc = R;
	p = 1 - (R << 1);

	while (Xc <= Yc)
	{
		GetPixelLCDBuffer(X + Xc, Y - Yc, color_line);	// 1
		GetPixelLCDBuffer(X + Yc, Y - Xc, color_line);	// 1
		GetPixelLCDBuffer(X - Xc, Y - Yc, color_line);	// 2
		GetPixelLCDBuffer(X - Yc, Y - Xc, color_line);	// 2
		GetPixelLCDBuffer(X - Xc, Y + Yc, color_line);	// 3
		GetPixelLCDBuffer(X - Yc, Y + Xc, color_line);	// 3
		GetPixelLCDBuffer(X + Xc, Y + Yc, color_line);	// 4
		GetPixelLCDBuffer(X + Yc, Y + Xc, color_line);	// 4

		if (p < 0)
		{
			p += (Xc++ << 2) + 6;
		}
		else
		{
			p += ((Xc++ - Yc--)<<2) + 10;
		}
	}
}
/******************************************************************************
* Name:	Привести код ASCII к своей таблице символов
* Input:	код символа
 * Return:	Код символа таблицы ASCIIS
 * Remark:  -
******************************************************************************/
unsigned char BringAndGetASCIISymbol(unsigned int byte_code_char){
    unsigned char local_ASCII_symbol = ' ' - 0x20;		// Пробел
    switch (byte_code_char & 0xFF00)
	{
		case 0x0000:
		{
			// Латиница
			local_ASCII_symbol = (unsigned char)(byte_code_char - 0x20);
			break;
		}
		case 0xD000:
		{
			// Первая половина таблицы симоволов кирилицы
			local_ASCII_symbol = (unsigned char)(byte_code_char - 0xCFF0);
			break;
		}
		case 0xD100:
		{
			// Вторая половина таблицы симоволов кирилицы
			local_ASCII_symbol = (unsigned char)(byte_code_char - 0xD0B0);
			break;
		}
		default:
		{
			// Ошибка символа
			local_ASCII_symbol = ' ' - 0x20;		// Пробел
			break;
		}
	}
    return local_ASCII_symbol;
}
/******************************************************************************
 * Name:	Записать символ в буфере дисплея
* Input:	Координаты Х [0..131], номер строки Y1 [0..3], сам символ (код ASCII); OPTION: цвет пикселя (потушить/закрасить)
 * Return:	-
 * Remark:  Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void GetCharLCDBuffer(unsigned char X, unsigned char Y, unsigned int byte_code_char, unsigned char color_char, unsigned char underline){
	unsigned short start_address_display_buffer = GET_STRING_ADDRESS(X,Y);
	unsigned char local_ASCII_symbol = BringAndGetASCIISymbol(byte_code_char);
    
    for (unsigned char i = 0; i < MY_CHARS[local_ASCII_symbol].character_length; i++) {
        if (color_char == LIGHT) {		
			display_buffer_transmit[start_address_display_buffer + i] = MY_CHARS[local_ASCII_symbol].upper_bytes[i];			
            if(underline){
                display_buffer_transmit[start_address_display_buffer + WIDTH + i] = MY_CHARS[local_ASCII_symbol].lower_bytes[i] | CHAR_LOW_BYTE_UNDERSCORE;
            } else {
                display_buffer_transmit[start_address_display_buffer + WIDTH + i] = MY_CHARS[local_ASCII_symbol].lower_bytes[i];
            }
		} else{
			display_buffer_transmit[start_address_display_buffer + i] = ~MY_CHARS[local_ASCII_symbol].upper_bytes[i];
			display_buffer_transmit[start_address_display_buffer + WIDTH + i] = ~MY_CHARS[local_ASCII_symbol].lower_bytes[i];
        }
    }
}
/******************************************************************************
* Name:		Записать строку в буфере дисплея
* Input:	Координаты Х [0..131], номер строки Y1 [0..3], сам символ (код ASCII); OPTION: цвет пикселя (потушить/закрасить), подчеркнуть (true/false)
* Return:	-
* Remark:  Левый верхний угол дисплея - начало координат(0,0), координаты должны быть коорректны, возможна ситуация выхода за границы дисплея
******************************************************************************/
void GetStringLCDBuffer(unsigned char X, unsigned char Y_string_number, unsigned char* pointer_string, unsigned char color_text, unsigned char underline){
    while(*pointer_string){ //пока не последний символ = символу конца строки 0x00
        GetCharLCDBuffer(X, Y_string_number, *pointer_string, color_text, underline);
        X += MY_CHARS[BringAndGetASCIISymbol(*pointer_string)].character_length;
        pointer_string++;
    }
    TextColumn[Y_string_number] = X;
}

/******************************************************************************
* Name:	Инвертировать строку в буфере дисплея
* Input:	номер строки Y1 [0..3]
* Return:	-
* Remark:  Левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void InvertStringLCDBuffer(unsigned char Y_string_number){    
    unsigned short start_address_display_buffer = GET_STRING_ADDRESS(0, Y_string_number);	
    
    for (unsigned char i = 0; i < WIDTH; i++) {
        display_buffer_transmit[start_address_display_buffer + i] = ~display_buffer_transmit[start_address_display_buffer + i];
		display_buffer_transmit[start_address_display_buffer + WIDTH + i] = ~display_buffer_transmit[start_address_display_buffer + WIDTH + i];
    }    
}

/******************************************************************************
* Name:     Пересчитать координаты
* Input:	Ссылка на координаты X1, Y1, X2, Y2
* Return:   Результат работы - изменение координат (аналогично 4-м вложенным циклам for)
* Remark:   Вызывется в фукнции TestUpdateDisplayAndBufferDisplay для перебора всех координат, левый верхний угол дисплея - начало координат(0,0)
******************************************************************************/
void TestRecalculateCoordinates(unsigned char coordinates[]){
    if(coordinates[2] != WIDTH - 1){
        coordinates[2]++;
	}    
	if((coordinates[2] == WIDTH - 1) & (coordinates[3] !=  HEIGTH - 1)){
		coordinates[2] = 0;
		coordinates[3]++;
	}
	if((coordinates[2] == WIDTH - 1) & (coordinates[3] ==  HEIGTH - 1)){//конец линии дошёл до конца дисплея (правый нижний угол)					
        coordinates[0]++;
        if(coordinates[0] == WIDTH - 1){
            coordinates[0] = 0;
            coordinates[1]++;
        }        
		coordinates[2] = coordinates[0];
        coordinates[3] = coordinates[1];        
		if((coordinates[0] == WIDTH - 1) & (coordinates[1] ==  HEIGTH - 1)){
            //и начало и конец линии побывали во всех координатах дисплея
            for(unsigned char i = 0; i < 4; i++){
                coordinates[i] = 0;
            }
        }
	}
}
/******************************************************************************
 * Name:		Инициализация контроллера дисплея, отправка ему пакета
 * Input:		-
 * Return:	Признак выполнения
 * Remark:  -
******************************************************************************/
unsigned char InitUC1601S(){
	unsigned char result = FALSE;
	//Init по датащиту (порядок команд)
	//START -> Header -> Command = System Reset -> STOP
	command_buffer[0] = SYSTEM_RESET;
	result |= SendPackageUC1601S(&command_buffer[0], 1, COMMAND_HEADER_TIC154A);
	TIM7->CNT = 0x0000;
	TIM7->ARR = 2; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	while(TIM7->CR1 & TIM_CR1_CEN){}//задержка 2 мс по датащиту
	//START -> Header -> Command/Data -> Command/Data -> ... -> Stop
	unsigned char init_command[] = {0x25, 0x2E, 0x89, 0xEB, 0xC6, 0xF2, 0x00, 0xF3, 63, 0x85, 0xAF, 0x81, 105};
	result |= SendPackageUC1601S(init_command, (sizeof(init_command) / sizeof(unsigned char)), COMMAND_HEADER_TIC154A);
	return result;
}
/******************************************************************************
 * Name:		Инициализация дисплея
 * Input:		-
 * Return:	Признак выполнения
 * Remark:  -
******************************************************************************/
unsigned char LCDInit(){
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
	//reset по датащиту (тайминги)
	LCD_RST_OFF();
	TIM7->CNT = 0x0000;
	TIM7->ARR = 10; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	while(TIM7->CR1 & TIM_CR1_CEN){}//задержка 10 мс по датащиту //после включения питания
	
	LCD_RST_ON();
	TIM7->CNT = 0x0000;
	TIM7->ARR = 2; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	while(TIM7->CR1 & TIM_CR1_CEN){}//задержка 2 мс по датащиту //reset
		
	LCD_RST_OFF();
	TIM7->CNT = 0x0000;
	TIM7->ARR = 10; //время в мс, которое будет работать таймер
	TIM7->CR1 |= TIM_CR1_CEN;
	while(TIM7->CR1 & TIM_CR1_CEN){}//задержка 10 мс по датащиту
	//обмен командами инициализации
	return InitUC1601S();
}

