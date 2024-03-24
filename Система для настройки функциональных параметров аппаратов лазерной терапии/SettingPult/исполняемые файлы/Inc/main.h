/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef union
{
	struct
	{
		unsigned package_accepted_swi	 :1;	//Флаг приёма передачи пакета, 1 - принят обрабатывается, 0 - запрет на принятие, подготовка и отправка ответа
		unsigned package_send_usb		 :1;
		unsigned package_accepted_usb	 :1;
		unsigned LCDError				 :1;	//Флаг, что произошла ошибка передачи пакета контроллеру дисплея, 1 -> нужно инициализировать дисплей заного
	};
	unsigned char flag_reg;							// Регистр флагов
} TypeFlag;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t usb_buffer_data[];
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
//TwoWireInterface - пульт настройки используется в качестве ретранслятора от USB в USART1
#define SIZE_USART_BUFFER_TX1_RX1 5  //по протоколу обмена БЛОК-ГОЛОВКА_V1.3
//значение репорта на 1 больше, т.к. передаётся report_id
//значение USBD_CUSTOMHID_OUTREPORT_BUF_SIZE - заведомо больше буфера приёма\передачи, @file: usbd_conf.h
#define SIZE_DATA_REPORT_COUNT_IN (SIZE_USART_BUFFER_TX1_RX1 + 1) //записывается в Usb HID report descriptor
#define SIZE_DATA_REPORT_COUNT_OUT (SIZE_USART_BUFFER_TX1_RX1 + 1)////записывается в Usb HID report descriptor
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
