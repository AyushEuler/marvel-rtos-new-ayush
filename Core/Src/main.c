/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "can.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "stm32l4xx_it.h"
#include "pvd.h"
#include "iwdg.h"
#include "wwdg.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tx_api.h"
#include <stdio.h>
#include "onBoardIOs.h"
#include "bmsConfiguration.h"
#include "initializeOperations.h"
#include "dataProcessing.h"
#include "FIRFilter.h"
#include "onBoardMemory.h"
#include "canControls.h"
#include "onBoardFlash.h"
#include "flashControls.h"
#include "watchDogControls.h"
#include "threadxMain.h"
#include "bmsCANMatrix.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t activeAPP = 0;

typedef struct 
{
	uint32_t magic;
	uint32_t flags;
} shared_memory_t;
typedef struct __attribute__((packed))
{
	uint16_t image_magic;
	uint16_t image_hdr_version;
	uint32_t crc;
	uint32_t data_size;
	uint8_t image_type;
	uint8_t version_major;
	uint8_t version_minor;
	uint8_t version_patch;
	uint32_t active_app;
	uint32_t reserved;
	char git_sha[8];
} image_hdr_t;
/* USER CODE END PTD */
image_hdr_t image_hdr __attribute__((section(".image_hdr"))) ={
  .image_magic  = IMAGE_MAGIC ,
  .image_hdr_version = IMAGE_HEADER_VERSION ,
  .crc = IMAGE_CRC ,
  .data_size = 0 ,
  .image_type = 0 ,
  .version_major = 1 ,
  .version_minor = 0 ,
  .version_patch = 0 ,
#ifdef APP1 
  .active_app = 1,
#endif
#ifdef APP2
  .active_app = 2 ,
#endif
.git_sha = GIT_SHA ,
};
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SENSITIVITY 0.003125f
#define BL_SWITCH_PARTITION  (uint32_t)(1 << 0)
// #define SW 1

extern uint8_t externalShortCircuitDetectionFlag;

uint16_t adc2ValArray[2] = {0};
int flag = 0;
uint32_t msTime = 0;
float voltage = 0.0;
float refVoltage = 0.0;
float current = 0.0f;
uint64_t Systemfrequency=0;
char msg[50] = {0};
extern coulombCountingOutput_t ccOutputDataEEPROM;

FIRFilter_t filteredVoltage;
FIRFilter_t filteredRefVoltage;
FIRFilter_t filteredCurrent;

bmsMainData_t mdata = {0};
bmsStatus_t status = BMS_ERROR;
shared_memory_t sharedMemory __attribute__((section(".shared_memory")));

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  
  /* USER CODE BEGIN 1 */
  
  bmsStatus_t status = BMS_ERROR;
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  initializePVD();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_CAN1_Init();
  MX_I2C1_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
#ifdef WWDOG
  MX_WWDG_Init();
#else  
  // MX_IWDG_Init();
#endif

  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);


  HAL_TIM_Base_Start(&htim2);

#ifdef ENABLE_LTC2949_SHORT_CIRCUIT
  if(HAL_OK != HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED))
  {
    Error_Handler();
  }

  HAL_ADC_Start(&hadc2);
#endif

  // FIRFilterInit(&filteredVoltage);
  // FIRFilterInit(&filteredRefVoltage);
  // FIRFilterInit(&filteredCurrent);

  pipelineInit();
  readFromPipeline(&mdata);

  RETRY(RETRY_CAN_INIT, status, initializeCAN(communicationCAN));
  RETRY(RETRY_CMU_INIT, status, initializeCMU(&mdata));
  RETRY(RETRY_CCM_INIT, status, initializeCCM());
  RETRY(RETRY_FLASH_READ, status, readDataFromFlash(FLASH_CONFIG_SECTION, FLASH_CONFIG_ADDRESS, defaultFlashConfigData, FLASH_CONFIG_TOTAL_LENGTH));

  if(status != BMS_OK)
  {
    //update flash error

  }

  activeAPP = image_hdr.active_app;
  initializeEEPROM(&mdata);
  
  Systemfrequency     = HAL_RCC_GetSysClockFreq();
  CoreDebug->DEMCR   |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT         = 0;
  DWT->CTRL          |= DWT_CTRL_CYCCNTENA_Msk;

  /* USER CODE END 2 */
  
  tx_kernel_enter();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{ 
  TIM3->CCR1 = 0;
  TIM3->CCR2 = 0;
  TIM3->CCR3 = 0;
  TIM3->CCR4 = 0;
  externalShortCircuitDetectionFlag = 1;
}

/* USER CODE END 4 */


 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }

  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
    systemReset();
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

void switch_partition_and_reset()
{
	sharedMemory.flags |= BL_SWITCH_PARTITION;
	NVIC_SystemReset();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
