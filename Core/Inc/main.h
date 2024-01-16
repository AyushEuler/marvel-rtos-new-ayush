
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l476xx.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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

/* USER CODE END EFP */
#define SHARED_MAGIC (uint32_t)0x27182818
#define IMAGE_MAGIC          0x7777
#define IMAGE_HEADER_VERSION 1
#ifndef IMAGE_CRC
#define IMAGE_CRC 0
#endif
/* Private defines -----------------------------------------------------------*/
//For HARDWARE_VERSION_3_1_1
#define humidityIN_DATA_Pin GPIO_PIN_2
#define humidityIN_DATA_GPIO_Port GPIOB

#define controlSW_LOAD_Pin GPIO_PIN_12
#define controlSW_LOAD_GPIO_Port GPIOB
#define controlSW_CHRG_Pin GPIO_PIN_13
#define controlSW_CHRG_GPIO_Port GPIOB
#define controlSW_BLNC_Pin GPIO_PIN_14
#define controlSW_BLNC_GPIO_Port GPIOB

#define SC_Detection_Pin GPIO_PIN_8
#define SC_Detection_GPIO_Port GPIOB
#define SC_Detection_EXTI_IRQn EXTI9_5_IRQn
#define SC_Reset_Pin GPIO_PIN_9
#define SC_Reset_GPIO_Port GPIOB

#define SC_InputDetection_2949_Pin GPIO_PIN_2
#define SC_InputDetection_2949_GPIO_Port GPIOD
#define SC_InputDetection_2949_EXTI_IRQn EXTI2_IRQn

//For HARDWARE_VERSION_3_0
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOC
#define MPP1_Pin GPIO_PIN_0
#define MPP1_GPIO_Port GPIOA
#define MPP2_Pin GPIO_PIN_1
#define MPP2_GPIO_Port GPIOA
#define MPP3_Pin GPIO_PIN_2
#define MPP3_GPIO_Port GPIOA
#define MPP4_Pin GPIO_PIN_3
#define MPP4_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_5
#define SW2_GPIO_Port GPIOC
#define SW3_Pin GPIO_PIN_0
#define SW3_GPIO_Port GPIOB
#define SW4_Pin GPIO_PIN_1
#define SW4_GPIO_Port GPIOB
#define DHT_DAT_Pin GPIO_PIN_12
#define DHT_DAT_GPIO_Port GPIOB
#define CAN1_EN_Pin GPIO_PIN_8
#define CAN1_EN_GPIO_Port GPIOA
#define MEM_WC_Pin GPIO_PIN_5
#define MEM_WC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void switch_partition_and_reset();
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
