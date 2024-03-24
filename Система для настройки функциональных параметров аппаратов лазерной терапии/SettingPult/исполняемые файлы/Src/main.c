/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/******************************************************************************
 * Основной файл проекта stm32f072xb
* Ресурсы
 PA4,5,6,7,8	- BTN
 PA2,3			- TX2, RX2 - (USART2 - однопроводный интерфейс)
 PA9,10 		- TX1, RX1 - (USART1 - двухпроводный интерфейс)
 PB10,11,2		- LCD_SCL (AF1-I2C2_SCL), LCD_CDA (AF1-I2C2_SDA), LCD_RST
 PA11,12 		- USB_D-, USB_D+
 PB4,5,6		- K1 (led), K2 (signal), K3
 PA13,14		- SWDDAT, SWDCLK
******************************************************************************/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CONST.h"
#include "USART.h"
#include "I2C.h"
#include "LCD_TIC154A.h"
#include "SingleWireInterface.h"
#include "usbd_customhid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//TIM16, исходя из частоты 24000000
#define PSC16 6
#define ARR16 40000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NUMBER_INTERRUPTS_PER_SECOND_TIM16 (CLOCK_FREQUENCY / PSC16 / ARR16) //прерывание 100 раз в секунду
//BTN
#define BTN_REGISTER_MASK 0x01F0 //IDR4,5,6,7,8
#define BTN_PORT() (GPIOA->IDR & BTN_REGISTER_MASK)
//первые 5 бит являются состоянием кнопок
#define BTN_PRESS() ((~BTN_PORT() & BTN_REGISTER_MASK) >> 4) //1 - положение кнопки внизу, 0 - отпущена
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile TypeFlag flag;
volatile unsigned char trash_reg;								// Мусорный регистр
extern const unsigned char cats[], clean[];
uint8_t usb_buffer_data[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];
unsigned char usart_buffer_Tx1[SIZE_USART_BUFFER_TX1_RX1];		//Заполняется данными от USB
unsigned char usart_buffer_Rx1[SIZE_USART_BUFFER_TX1_RX1*2];	//Заполняется данными от USART1
uint8_t pointer_usart_buffer_Rx1 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/******************************************************************************
* Обработчик прерывания TIM16 (100 Гц). Приоритет 0
* Проверка на длительное не получение байт
******************************************************************************/
void TIM16_IRQHandler(void){
	TIM16->SR &= ~TIM_SR_UIF;
	CheckTimeBetweenBytes();
   }
/******************************************************************************
* Обработчик прерывания USART2. Приоритет 2
* Приём байта по однопроводному интерфейсу и проверка принят ли полностью пакет
* Установка флага о полностью принятом пакете
******************************************************************************/
void USART2_IRQHandler(void){
	USART2->ICR |= USART_ICR_ORECF;
	if(flag.package_accepted_swi)
	{ //был принят пакет, сейчас идет передача ответа (не обрабатывать принимаемые данные)
		trash_reg = ((uint8_t)(USART2->RDR));
	}
	else
	{
		timeout_byte_Rx2 = TIMEOUT_BYTE_RX2;
		flag.package_accepted_swi = FillBufferReceivedByte();
	}
}
/******************************************************************************
* DMA interrupt
******************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
  DMA1->IFCR = DMA_IFCR_CTCIF3;
  flag.package_send_usb = TRUE;
}
/******************************************************************************
 * Name:    Transmit package through DMA
 * Input:   no
 * Return:  no
 * Remark:  no
******************************************************************************/
void WriteUSART1_DMA(void)
{
	USART1->ICR = USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_ORECF;    // Clear errors

	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;                             // disable DMA

	DMA1_Channel2->CMAR = (uint32_t)(&(usart_buffer_Tx1[0]));      // source
	DMA1_Channel3->CMAR = (uint32_t)(&(usart_buffer_Rx1[0]));      // receiver

	DMA1_Channel2->CNDTR = SIZE_USART_BUFFER_TX1_RX1;    // quantity bytes for transmit
	DMA1_Channel3->CNDTR = SIZE_USART_BUFFER_TX1_RX1*2;  // quantity bytes for receive

	DMA1_Channel3->CCR |= DMA_CCR_EN;
	DMA1_Channel2->CCR |= DMA_CCR_EN;               // enable transmitting
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  //инициализация переферии
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	//инициализация кнопок
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0 | GPIO_PUPDR_PUPDR8_0;
	//инициализация TIM16
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16->PSC = PSC16 - 1;
	TIM16->ARR = ARR16 - 1;
	TIM16->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM16_IRQn);
	NVIC_SetPriority(TIM16_IRQn, 0);
	TIM16->CR1 |= TIM_CR1_CEN;
	SingleWireInterfaceInit();
	UsartInit();
	I2C2Init();
	//иницаиализация переменных
	flag.flag_reg = 0;
	flag.LCDError = ~LCDInit();
	//заставка
	GetStringLCDBuffer(25 , 1, (unsigned char*)&"Setting Pult", LIGHT, FALSE);
	GetStringLCDBuffer(25 , 2, (unsigned char*)&"V1.0", LIGHT, FALSE);
	for(uint8_t i = 0; i < 4; i++)
	{
		InvertStringLCDBuffer(i);
	}
	flag.LCDError |= ~SendPackageUC1601S(&display_buffer_transmit[0], sizeof(display_buffer_transmit) / sizeof(unsigned char), DATA_HEADER_TIC154A);
	usb_buffer_data[0] = 0x01;		  //при отправке в usb обязательный первый байт, report_id
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(flag.package_accepted_swi)
	  {
			if(CheckChecksumPackage(FALSE))
			{ //проверка принятого пакета, если сошлась, тогда изменить
				//переменные для CodeCommandPultOutValue
				uint32_t number = 0x00;
				uint16_t divider = 0x00;
				uint8_t j = 0;//для работы алгоритма
				unsigned flag_unknown_command_no_send_package = FALSE;
				switch (usart_buffer_Tx2_Rx2[0]) //ответом на принятую команду всегдая является состояние кнопок
				{
					case CodeCommandPultGetButton:
					{
						__NOP();
						break;
					}
					case CodeCommandPultOutString:
					{
						GetStringLCDBuffer(usart_buffer_Tx2_Rx2[1], usart_buffer_Tx2_Rx2[2], (unsigned char*)&usart_buffer_Tx2_Rx2[3], LIGHT, FALSE);
						break;
					}
					case CodeCommandPultOutValue:
					{
						for(uint8_t i = 3; i < 7; i++)
						{//первые 4 байта по протоколу - число
							number |= usart_buffer_Tx2_Rx2[i] << 8 * j;
							j++;
						}
						j = 0;
						for(uint8_t i = 7; i < 9; i++)
						{//первые 2 байта по протоколу - делитель
							divider |= usart_buffer_Tx2_Rx2[i] << 8 * j;
							j++;
						}
						GetStringLCDBuffer(usart_buffer_Tx2_Rx2[1], usart_buffer_Tx2_Rx2[2], GetStringValue(number, divider, (unsigned char*)&usart_buffer_Tx2_Rx2[9]), LIGHT, FALSE);
						break;
					}
					case CodeCommandPultPultClear:
					{
						UpdateLCDBuffer(&clean[0]);
						break;
					}
					default:
					{
						flag_unknown_command_no_send_package = TRUE;
						UpdateLCDBuffer(&clean[0]);
						GetStringLCDBuffer(0,0, (unsigned char*)&"ERROR!", LIGHT, TRUE);
						GetStringLCDBuffer(0,1, (unsigned char*)&"Unknown command", LIGHT, FALSE);
						break;
					}
				}
				//строгий порядок ответа после принятия пакета и обработки буфера дисплея: вывести картинку на дисплей, отправить ответ, подождать, установить флаг, что можно принимать запрос
				if(usart_buffer_Tx2_Rx2[0] != CodeCommandPultGetButton)
				{
					flag.LCDError |= ~SendPackageUC1601S(&display_buffer_transmit[0], sizeof(display_buffer_transmit) / sizeof(unsigned char), DATA_HEADER_TIC154A);
				}
				if(!flag_unknown_command_no_send_package)
				{
					SendPackage((unsigned char)(BTN_PRESS()));
					HAL_Delay(1);
				}
			}
			flag.package_accepted_swi = FALSE;
		}

		if(flag.LCDError)
		{
			flag.LCDError = ~LCDInit();
			flag.LCDError |= ~SendPackageUC1601S(&display_buffer_transmit[0], sizeof(display_buffer_transmit) / sizeof(unsigned char), DATA_HEADER_TIC154A);
		}

		if(flag.package_accepted_usb)
		{
			for(uint8_t i = 0; i < SIZE_USART_BUFFER_TX1_RX1; i++)
			{
				usart_buffer_Tx1[i] = usb_buffer_data[i + 1]; //report id не нужно передавать в usart
			}
			WriteUSART1_DMA();

			flag.package_accepted_usb = FALSE;

			UpdateLCDBuffer(&cats[0]);
			flag.LCDError |= ~SendPackageUC1601S(&display_buffer_transmit[0], sizeof(display_buffer_transmit) / sizeof(unsigned char), DATA_HEADER_TIC154A);
		}

		if(flag.package_send_usb)
		{
			usb_buffer_data[0] = 0x01;
			for(uint8_t i = 0; i < SIZE_USART_BUFFER_TX1_RX1; i++)
			{
				usb_buffer_data[i + 1] = usart_buffer_Rx1[i + SIZE_USART_BUFFER_TX1_RX1];
			}
			USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&usb_buffer_data[0], SIZE_DATA_REPORT_COUNT_OUT + 1);
			flag.package_send_usb = FALSE;
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
