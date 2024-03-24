#include "CONST.h"
#include "USART.h"
/******************************************************************************
 * Переменные
******************************************************************************/

/******************************************************************************
 * Name:	Инициализация Usart
 * Input:	-
 * Return:	-
 * Remark:  usart1,2 - Преоритет 1; Скорость передачи 9600;	Поле данных 8bit;  1stop бит; Полудуплексный режим; Инверсия сигнала TX; разрешить прерывание при появлении данных в регистре приемника
******************************************************************************/
void UsartInit(){
	//Usart2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 						//включение тактирования USART2
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;	//указание, что ножка альтернативная
	GPIOA->AFR[0] |= AF1_AFRL2;									//альтернаятивная функция USART2_TX
	GPIOA->AFR[0] |= AF1_AFRL3;									//USART2_RX
	USART2->CR2 |= USART_CR2_TXINV;			 					//инверсия сигнала TX
	//USART2->CR2 												по умолчанию 00 - 1 стоп бит
	USART2->BRR = CLOCK_FREQUENCY/9600;							//скорость передачи
	//USART2->CR1 &= ~USART_CR1_M; 								//по умолчанию сообщение 8 бит
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE| USART_CR1_UE  | USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 1);
	// USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1;            // Alternate function RA9
	GPIOA->AFR[1] |= AF1_AFRH9;                       // RA9 - AF1
	GPIOA->MODER |= GPIO_MODER_MODER10_1;           // Alternate function RA10
	GPIOA->AFR[1] |= AF1_AFRH10;                       // RA10 - AF1
	USART1->CR2 |= USART_CR2_TXINV | USART_CR2_STOP_1;                 // Inversion TX, 2 стоп бита из-за возможности передачи старых головок, слабый проц
	USART1->BRR = CLOCK_FREQUENCY/9600;           // Speed
	USART1->CR1 = USART_CR1_TE | USART_CR1_UE | USART_CR1_RE;
	USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;  // DMA

	//DMA for USART1
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel2->CPAR = (uint32_t) (&(USART1->TDR));
	DMA1_Channel2->CCR |= DMA_CCR_DIR | DMA_CCR_MINC;
	DMA1_Channel3->CPAR = (uint32_t) (&(USART1->RDR));
	DMA1_Channel3->CCR |= DMA_CCR_MINC | DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
};

/******************************************************************************
 * Name:		Запись данных в USART
 * Input:		Данные, кол-во отправляемых байт, ссылка на USART в который будет производиться запись (USART1 или USART2)
 * Return:	-
 * Remark:  -
******************************************************************************/
void WriteUSART(unsigned char *pointer, unsigned char count_bytes, USART_TypeDef* USART){
	while(count_bytes--){
		USART->ICR |= USART_ICR_TCCF; /* Clear transfer complete flag */
		USART->TDR = *pointer++;
		while(!(USART->ISR & USART_ISR_TC)){}; //дождаться полной отправки байта
		USART->ICR |= USART_ICR_TCCF; /* Clear transfer complete flag */
	}
}
